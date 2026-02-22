/*
 * Código de Teste para USB Host Shield + Mouse
 * 
 * Use este código para:
 * 1. Verificar se o USB Host Shield está funcionando
 * 2. Testar se o mouse está sendo detectado
 * 3. Ver os dados brutos que o mouse envia
 * 
 * Como usar:
 * 1. Abra este arquivo na Arduino IDE
 * 2. Faça upload para o Arduino Leonardo
 * 3. Abra o Monitor Serial (9600 baud)
 * 4. Conecte o mouse no USB Host Shield
 * 5. Mova o mouse e clique os botões
 * 6. Observe a saída no Monitor Serial
 */

#include <SPI.h>
#include <usbhub.h>
#include <hidboot.h>

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

class MouseRptParser : public MouseReportParser {
protected:
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
};

MouseRptParser Prs;

void setup() 
{
    Serial.begin(9600);
    Serial.println("\n\n==================================");
    Serial.println("Teste de USB Host Shield + Mouse");
    Serial.println("==================================");
    Serial.println();
    
    Serial.print("Inicializando USB Host Shield...");
    
    if (Usb.Init() == -1) {
        Serial.println(" FALHOU!");
        Serial.println();
        Serial.println("ERRO: USB Host Shield n\u00e3o inicializou!");
        Serial.println();
        Serial.println("Verifique:");
        Serial.println("1. Shield est\u00e1 bem encaixado no Arduino");
        Serial.println("2. Tr\u00eas pontes est\u00e3o soldadas (5V, 3.3V, 5V)");
        Serial.println("3. Shield compat\u00edvel com Arduino Leonardo");
        Serial.println();
        
        // Pisca LED rapidamente indicando erro
        while(1) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
    
    Serial.println(" OK!");
    Serial.println();
    
    HidMouse.SetReportParser(0, &Prs);
    
    Serial.println("Aguardando mouse ser conectado...");
    Serial.println("(Conecte o mouse na porta USB do shield)");
    Serial.println();
    
    // LED aceso indicando sucesso
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() 
{
    Usb.Task();
}

void MouseRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    Serial.println("-----------------------------------");
    Serial.println("Dados Brutos do Mouse:");
    Serial.print("Tamanho: ");
    Serial.print(len);
    Serial.println(" bytes");
    Serial.print("Bytes: ");
    
    for (uint8_t i = 0; i < len; i++) {
        if (buf[i] < 0x10) Serial.print("0");
        Serial.print(buf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    
    // Tenta interpretar como formato padr\u00e3o
    Serial.println();
    Serial.println("Interpreta\u00e7\u00e3o (formato padr\u00e3o):");
    Serial.print("  Byte 0 (bot\u00f5es): ");
    Serial.print(buf[0], BIN);
    
    if (buf[0] & 0x01) Serial.print(" [Bot\u00e3o Esquerdo]");
    if (buf[0] & 0x02) Serial.print(" [Bot\u00e3o Direito]");
    if (buf[0] & 0x04) Serial.print(" [Bot\u00e3o Meio]");
    if (buf[0] & 0x08) Serial.print(" [Bot\u00e3o 4]");
    if (buf[0] & 0x10) Serial.print(" [Bot\u00e3o 5]");
    Serial.println();
    
    if (len >= 2) {
        Serial.print("  Byte 1 (delta X): ");
        Serial.print((int8_t)buf[1]);
        Serial.println();
    }
    
    if (len >= 3) {
        Serial.print("  Byte 2 (delta Y): ");
        Serial.print((int8_t)buf[2]);
        Serial.println();
    }
    
    if (len >= 4) {
        Serial.print("  Byte 3 (scroll): ");
        Serial.print((int8_t)buf[3]);
        Serial.println();
    }
    
    Serial.println("-----------------------------------");
    Serial.println();
}

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    Serial.print("Movimento: X=");
    Serial.print(mi->dX);
    Serial.print(", Y=");
    Serial.println(mi->dY);
}

void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi)
{
    Serial.println("\u2193 Bot\u00e3o ESQUERDO pressionado");
}

void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi)
{
    Serial.println("\u2191 Bot\u00e3o ESQUERDO solto");
}

void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi)
{
    Serial.println("\u2193 Bot\u00e3o DIREITO pressionado");
}

void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi)
{
    Serial.println("\u2191 Bot\u00e3o DIREITO solto");
}

void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi)
{
    Serial.println("\u2193 Bot\u00e3o MEIO pressionado");
}

void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi)
{
    Serial.println("\u2191 Bot\u00e3o MEIO solto");
}
