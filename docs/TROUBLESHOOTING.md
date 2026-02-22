# 🔧 Guia Completo de Troubleshooting

## 🚨 Problemas Comuns e Soluções

---

## 1. Arduino não é detectado pelo PC

### Sintomas:
- Arduino IDE não mostra nenhuma porta COM
- Device Manager mostra "Unknown Device"
- LED do Arduino não acende

### Soluções:

#### A. Instalar Drivers CH340/CH341

```bash
1. Baixe o driver CH340:
   Windows: https://www.arduined.eu/ch340-windows-10-driver-download/
   
2. Extraia o arquivo ZIP
3. Execute "SETUP.EXE" como Administrador
4. Reinicie o computador
5. Reconecte o Arduino
```

#### B. Testar em outra porta USB

- Use porta USB 2.0 (não 3.0)
- Evite hubs USB
- Teste portas USB traseiras do PC (geralmente mais estáveis)

#### C. Verificar cabo USB

- Alguns cabos são apenas para carga (não têm fios de dados)
- Teste com outro cabo Micro USB
- Certifique-se que o cabo está bem conectado

---

## 2. Erro "OSC did not start" ou "Init fail"

### Sintomas:
- LED do Arduino pisca rapidamente
- Monitor Serial mostra "OSC did not start"
- USB Host Shield não inicializa

### Soluções:

#### A. Verificar pontes de solda

⚠️ **As 3 pontes SÃO OBRIGATÓRIAS!**

```
Verifique no USB Host Shield:

J1 [5V]   → [ ●===● ]  Deve estar soldada
J2 [3.3V] → [ ●===● ]  Deve estar soldada  
J3 [5V]   → [ ●===● ]  Deve estar soldada
```

**Como soldar corretamente:**
1. Use ferro de solda 30-40W
2. Aqua a ponte por 2-3 segundos
3. Aplique solda até formar uma "bolinha" conectando os dois pads
4. Deixe esfriar naturalmente
5. Teste continuidade com multímetro

#### B. Verificar encaixe do shield

```
1. Remova o USB Host Shield do Arduino
2. Inspecione os pinos:
   - Devem estar retos (não tortos)
   - Sem oxidação (não devem estar verdes)
   
3. Limpe os pinos se necessário:
   - Use álcool isopropílico
   - Ou borracha escolar
   
4. Reencaixe firmemente:
   - Pressione uniformemente
   - Todos os pinos devem entrar completamente
```

#### C. Testar shield sozinho

```cpp
// Código de teste simples
#include <SPI.h>
#include <usbhub.h>

USB Usb;

void setup() {
  Serial.begin(9600);
  Serial.println("Testando USB Host Shield...");
  
  if (Usb.Init() == -1) {
    Serial.println("FALHA: Shield não inicializou!");
    while(1);
  }
  
  Serial.println("SUCESSO: Shield funcionando!");
}

void loop() {
  Usb.Task();
}
```

Faça upload deste código. Se mostrar "FALHA", o problema é no shield.

---

## 3. Mouse não funciona ou movimenta errado

### Sintomas:
- Mouse conectado mas não move
- Movimento invertido ou multiplicado
- Scroll não funciona
- Botões não respondem

### Soluções:

#### A. Ativar modo Debug

**Passo 1:** Edite o arquivo `settings.h`:

```cpp
// Localize em:
// C:\Users\SEU_USUARIO\Documents\Arduino\libraries\USB_Host_Shield_2.0\settings.h

// Mude isto:
#define ENABLE_UHS_DEBUGGING 0

// Para isto:
#define ENABLE_UHS_DEBUGGING 1
```

**Passo 2:** Faça upload no Arduino novamente

**Passo 3:** Abra Monitor Serial (9600 baud)

**Passo 4:** Conecte o mouse e observe a saída:

```
Exemplo de saída:

OSC did not start
// OU
Start
Mouse Report:
  Byte 0: 00    <- Botões (00 = nenhum pressionado)
  Byte 1: 05    <- Delta X
  Byte 2: FF    <- Delta Y  
  Byte 3: 00    <- Scroll
  Byte 4: 00    <- Padding
```

#### B. Ajustar estrutura MYMOUSEINFO

Baseado no debug, ajuste `hidcustom.h`:

**Para mouses comuns (8-bit):**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;
  int8_t dX;      // 8-bit signed
  int8_t dY;      // 8-bit signed
  int8_t wheel;
};
```

**Para mouses gamer (16-bit):**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;
  int16_t dX;     // 16-bit signed
  int16_t dY;     // 16-bit signed
  int8_t wheel;
};
```

**Para Logitech G-series:**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;
  int16_t dX;
  int16_t dY;
  int8_t wheel;
  uint8_t padding[3];  // Alguns mouses têm bytes extras
};
```

#### C. Testar com mouse simples

Se tiver problemas:
1. Teste primeiro com mouse USB genérico/barato
2. Se funcionar, o problema é compatibilidade com seu mouse gamer
3. Ajuste a estrutura conforme o debug

---

## 4. Botões laterais (MB4/MB5) não funcionam

### Sintomas:
- Botões 1, 2, 3 (esquerdo, direito, meio) funcionam
- Botões 4 e 5 (laterais) não respondem no jogo

### Solução: Mapear para teclas

Alguns jogos não reconhecem MB4/MB5 via HID. Solucione mapeando para teclas:

```cpp
// Adicione no início do Arduino_HostShield.ino:
#include <Keyboard.h>

// Em setup(), adicione:
void setup() {
    // ... código existente ...
    Keyboard.begin();
}

// Modifique HandleButtonChange:
void HandleButtonChange(uint8_t prevState, uint8_t newState, uint8_t button)
{
    bool prevPressed = CHECK_BIT(prevState, button);
    bool newPressed = CHECK_BIT(newState, button);

    if (prevPressed != newPressed)
    {
        // Botões 1, 2, 3 - funcionam como mouse
        if (button == MOUSE_LEFT || button == MOUSE_RIGHT || button == MOUSE_MIDDLE)
        {
            if (newPressed)
                Mouse.press(button);
            else
                Mouse.release(button);
        }
        // Botões 4 e 5 - mapeados para teclas
        else if (button == MOUSE_PREV)
        {
            if (newPressed)
                Keyboard.press('x');  // Mude para sua tecla preferida
            else
                Keyboard.release('x');
        }
        else if (button == MOUSE_NEXT)
        {
            if (newPressed)
                Keyboard.press('z');  // Mude para sua tecla preferida
            else
                Keyboard.release('z');
        }
    }
}
```

Depois configure no jogo para reconhecer as teclas 'X' e 'Z'.

---

## 5. Scroll wheel não funciona

### Sintomas:
- Mouse move normalmente
- Botões funcionam
- Scroll não responde

### Soluções:

#### A. Verificar posição do byte wheel

No debug, observe qual byte muda quando você rola o scroll:

```
Scroll para cima:   Byte 3: 01
Scroll para baixo:  Byte 3: FF
```

Se for **Byte 4** ou outro:

```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;  // Byte 0
  int16_t dX;       // Bytes 1-2
  int16_t dY;       // Bytes 3-4
  int8_t wheel;     // Byte 5  <- Ajuste aqui!
};
```

#### B. Inverter direção do scroll

Se scroll funciona mas ao contrário:

```cpp
void MouseRptParser::OnWheelMove(MYMOUSEINFO *mi)
{
    Mouse.move(0, 0, -mi->wheel);  // Note o sinal negativo
}
```

---

## 6. Lag ou movimento travado

### Sintomas:
- Mouse move, mas com atraso
- Movimento não é suave
- Cursor "pula"

### Soluções:

#### A. Aumentar polling rate

```cpp
// Em setup():
void setup()
{
    Serial.begin(115200);  // Aumente de 57600 para 115200
    // ... resto do código ...
}
```

**Atenção:** Atualize também no Aimmy!

#### B. Remover delay desnecessário

Verifique se não há `delay()` no loop principal.

#### C. Otimizar USB.Task()

```cpp
void loop()
{
    static unsigned long lastTask = 0;
    
    // Processa USB a cada 1ms
    if (millis() - lastTask >= 1)
    {
        Usb.Task();
        lastTask = millis();
    }
    
    // ... resto do código ...
}
```

---

## 7. Valorant ainda detecta

### Sintomas:
- Setup funcionando perfeitamente
- Mas Vanguard/anti-cheat detecta

### Realidade:

⚠️ **Nenhum método é 100% indetectável!**

O USB Host Shield **reduz drasticamente** a chance de detecção porque:
- ✅ PC vê apenas 1 mouse (não 2)
- ✅ Usa HID puro (não COM port)
- ✅ Não cria device suspeito

**Porém:**
- ❌ Padrões de movimento podem ser detectados
- ❌ Comportamento "muito perfeito" é suspeito  
- ❌ Reports manuais de outros jogadores

### Dicas para reduzir detecção:

1. **Não seja óbvio**
   - Não faça flicks instantâneos
   - Adicione randomização no Aimmy
   - Erre de propósito às vezes

2. **Use configurações legit**
   - FOV pequeno (4-8°)
   - Smoothing alto
   - Não use 100% headshot

3. **Não use em ranked/competitivo**
   - Maior chance de reports
   - Replays são analisados

4. **Evite padrões**
   - Não snap sempre no mesmo lugar
   - Varie seu gameplay

---

## 8. Compilação com erros

### Erro: "Mouse.h: No such file or directory"

**Solução:**
```
1. Arduino IDE → Ferramentas → Placa
2. Certifique-se que está: "Arduino Leonardo"
3. NÃO use "Arduino Uno" ou outras placas
```

### Erro: "'MOUSE_LEFT' was not declared"

**Solução:**
```
Verifique se ImprovedMouse.h está na mesma pasta que o .ino
Caminho correto:
Arduino_HostShield/
  ├── Arduino_HostShield.ino
  ├── hidcustom.h
  ├── ImprovedMouse.h
  └── ImprovedMouse.cpp
```

### Erro: "Usb.h: No such file"

**Solução:**
```
Biblioteca USB Host Shield não instalada.
Veja seção "Instalar Biblioteca" no README.md
```

---

## 9. Ferramentas de diagnóstico

### A. Teste de LED

Adicione no `loop()` para debug visual:

```cpp
void loop()
{
    // Pisca LED quando recebe dados serial
    if (Serial.available() > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        // ... processa comando ...
        digitalWrite(LED_BUILTIN, LOW);
    }
}
```

### B. Monitor de comandos

Para ver o que o Aimmy está enviando:

```cpp
void ParseSerialCommand(const String& command)
{
    Serial.print("Comando recebido: ");
    Serial.println(command);
    
    // ... resto do código ...
}
```

### C. Teste manual via Serial Monitor

No Monitor Serial, teste comandos:

```
m10,5     <- Move 10px direita, 5px baixo
c         <- Click
p         <- Press
r         <- Release
```

---

## 📞 Suporte Adicional

Se nenhuma solução funcionou:

1. 🐛 **Abra uma Issue** com:
   - Modelo exato do Arduino
   - Modelo do USB Host Shield
   - Modelo do mouse
   - Saída completa do Monitor Serial (modo debug)
   - Fotos das conexões

2. 📚 **Leia documentação original**:
   - [USB Host Shield Manual](https://chome.nerpa.tech/usb-host-shield-hardware-manual/)
   - [Library GitHub](https://github.com/felis/USB_Host_Shield_2.0)

3. 🔍 **Pesquise no UnknownCheats**:
   - [Arduino HID Mouse Thread](https://www.unknowncheats.me/forum/valorant/642071-arduino-hid-mouse-free-libraries.html)

---

**✅ Boa sorte com seu setup!**
