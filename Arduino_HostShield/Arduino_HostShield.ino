#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <usbhub.h>
#include <string.h>
#include "hidcustom.h"

// Delta para movimento do mouse
signed char delta[3] = {0, 0, 0};

// Objetos USB
USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb, true);
MouseRptParser Prs;

void HandleButtonChange(uint8_t prevState, uint8_t newState, uint8_t button);

void setup()
{
    // Comunicação serial para receber comandos do Aimmy
    Serial.begin(57600);
    Serial.setTimeout(1);
    
    // Inicializa USB Host Shield
    if (Usb.Init() == -1) {
        // Falha na inicialização - LED piscando rápido
        while(1) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
    
    // Configura o parser do mouse
    HidMouse.SetReportParser(0, &Prs);
    
    // Inicializa a emulação de mouse HID
    Mouse.begin();
    
    // LED indicando sucesso
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    // Reseta delta a cada loop
    memset(delta, 0, sizeof(delta));

    // Processa eventos USB (mouse físico)
    Usb.Task();

    // Lê comandos do Aimmy via Serial
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        ParseSerialCommand(command);
    }

    // Move o mouse virtual com o delta acumulado
    Mouse.move(delta[0], delta[1], delta[2]);
}

// Parser customizado para o mouse físico
void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    MYMOUSEINFO *pmi = reinterpret_cast<MYMOUSEINFO*>(buf);

    // Processa mudanças nos botões
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_LEFT);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_RIGHT);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_MIDDLE);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_PREV);
    HandleButtonChange(prevState.mouseInfo.buttons, pmi->buttons, MOUSE_NEXT);

    // Processa movimento
    if (pmi->dX || pmi->dY)
    {
        OnMouseMove(pmi);
    }

    // Processa scroll
    if (pmi->wheel)
    {
        OnWheelMove(pmi);
    }
    
    prevState.mouseInfo.buttons = pmi->buttons;
    prevState.bInfo[0] = buf[0];
}

void MouseRptParser::OnMouseMove(MYMOUSEINFO *mi)
{
    delta[0] = mi->dX;
    delta[1] = mi->dY;
}

void MouseRptParser::OnWheelMove(MYMOUSEINFO *mi)
{
    Mouse.move(0, 0, mi->wheel);
}

// Processa comandos recebidos do Aimmy
void ParseSerialCommand(const String& command)
{
    if (command == "c")
    {
        Mouse.click();
    }
    else if (command == "r")
    {
        Mouse.release();
    }
    else if (command == "p")
    {
        Mouse.press();
    }
    else if (command.startsWith("m"))
    {
        ExecuteMouseMoveCommand(command);
    }
}

void ExecuteMouseMoveCommand(const String& command)
{
    String moveCommand = command;
    moveCommand.replace("m", "");

    int commaIndex = moveCommand.indexOf(',');
    int x = moveCommand.substring(0, commaIndex).toInt();
    int y = moveCommand.substring(commaIndex + 1).toInt();

    Mouse.move(x, y);
}

void HandleButtonChange(uint8_t prevState, uint8_t newState, uint8_t button)
{
    bool prevPressed = CHECK_BIT(prevState, button);
    bool newPressed = CHECK_BIT(newState, button);

    if (prevPressed != newPressed)
    {
        if (newPressed)
        {
            Mouse.press(button);
        } 
        else
        {
            Mouse.release(button);
        }
    }
}
