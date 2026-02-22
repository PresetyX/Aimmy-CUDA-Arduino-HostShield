# 🔌 Guia Visual de Conexões

## 📍 Visão Geral do Sistema

```
┌───────────────────────┐
│   Mouse Físico USB      │
│   (Logitech, Razer,    │
│    qualquer mouse)      │
└─────────┬─────────────┘
          │ Cabo USB
          ↓
┌─────────┴─────────────┐
│  USB Host Shield 2.0    │
│  [■] Porta USB (topo)  │
│                         │
│  Chip: MAX3421E         │
│  Pontes: J1, J2, J3     │
└─────────┬─────────────┘
          │ Pinos encaixados
          ↓
┌─────────┴─────────────┐
│  Arduino Leonardo R3    │
│                         │
│  Chip: ATmega32U4       │
│  Porta: Micro USB       │
└─────────┬─────────────┘
          │ Cabo Micro USB
          ↓
┌─────────┴─────────────┐
│    Computador / PC      │
│                         │
│  Windows 10/11          │
│  Aimmy CUDA rodando     │
└───────────────────────┘
```

---

## 🔨 Passo 1: Preparar USB Host Shield

### Localizar as pontes de solda

```
Vista superior do USB Host Shield:

          [Porta USB]
              │
    ┌───────┴───────┐
    │               ○    │
    │ MAX3421E      ○    │
    │               ○    │
    │                    │
    │  J1 J2 J3  <- Pontes│
    │  ●  ●  ●          │
    │  ●  ●  ●  (soldar)│
    │                    │
    └────────────────────┘
```

### Identificar as pontes

```
J1 (esquerda)   = 5V   → Alimenta lógica 5V
J2 (centro)     = 3.3V → Alimenta lógica 3.3V
J3 (direita)    = 5V   → Alimenta porta USB
```

### Como soldar

```
Vista lateral da ponte:

ANTES (não soldado):

  Pad A    Pad B
    ○        ○
    │        │
    └────────┘


DEPOIS (soldado):

  Pad A    Pad B
    ●────────●
    │  Solda  │
    └──────────┘
```

**Procedimento:**

1. Aqua o ferro de solda (30-40W)
2. Toque os dois pads simultaneamente por 2-3s
3. Aplique solda até formar uma "ponte" entre os pads
4. Retire o ferro e deixe esfriar
5. Repita para as 3 pontes

**Verificação:**

Com multímetro em modo continuidade:
- Toque as duas pontas nos pads
- Deve emitir beep (indica conexão)
- Faça para as 3 pontes

---

## 🔌 Passo 2: Montar o Shield no Arduino

### Vista explodida

```
        USB Host Shield
        (lado dos componentes para cima)
              │
              ↓
        ┌────────────────────┐
        │ [■] Porta USB      │
        │                    │
        │ MAX3421E           │
        │                    │
        │ Pinos (fêmea)       │
        └───────┬────────────┘
                │
                │ Encaixe
                ↓
        ┌───────┴─────────────┐
        │ Pinos (macho)      │
        │                    │
        │ Arduino Leonardo   │
        │                    │
        │ ATmega32U4         │
        │ [■] Porta MicroUSB │
        └────────────────────┘
```

### Alinhamento dos pinos

```
Vista superior (montado):

┌──────────────────────────────┐
│      USB Host Shield        │
│  [■]                        │
│ Porta                       │
│  USB                         │
│                              │
├──────────────────────────────┤ <- Encaixe
│      Arduino Leonardo        │
│                              │
│  [■] <- Porta Micro USB     │
└──────────────────────────────┘
```

**Importante:**
- Alinhe os pinos cuidadosamente
- Pressione firmemente mas com cuidado
- Todos os pinos devem entrar completamente
- Não force se não alinhar (pode entortar pinos)

---

## 🔌 Passo 3: Conectar o Mouse

### Conexão física

```
Mouse USB ----[cabo]----> [Porta USB do Shield]
                                │
                                │
                           USB Host Shield
                                │
                                │
                          Arduino Leonardo
                                │
                           [Micro USB]
                                │
                          Cabo Micro USB
                                │
                          -----[USB]-----
                                │
                          Porta USB do PC
```

### Ordem de conexão correta

```
1⃣ Primeiro:  Encaixe o Shield no Arduino
              ↓
2⃣ Segundo:   Conecte Arduino ao PC (Micro USB)
              ↓
3⃣ Terceiro:  Faça upload do código
              ↓
4⃣ Quarto:    Conecte o mouse no Shield
              ↓
5⃣ Pronto!    Teste o mouse
```

---

## ⚡ Alimentação Elétrica

### Fluxo de energia

```
PC (5V USB)
    │
    ↓ Cabo Micro USB
    │
Arduino Leonardo (regulador interno)
    │
    ├─> Lógica Arduino (5V)
    │
    └─> USB Host Shield
        │
        ├─> Lógica 5V (J1)
        ├─> Lógica 3.3V (J2)
        └─> Porta USB (J3)
            │
            └─> Mouse USB (5V)
```

**Consumo típico:**
- Arduino Leonardo: ~50mA
- USB Host Shield: ~30mA
- Mouse USB: ~100mA (varia)
- **Total: ~180mA**

Porta USB 2.0 fornece 500mA, então é suficiente.

---

## 📶 Comunicação de Dados

### Fluxo de dados

```
1. Mouse -> USB -> Host Shield
   (Movimentos físicos)

2. Host Shield -> SPI -> Arduino
   (Dados digitalizados)
   Pinos usados: 10, 11, 12, 13

3. Arduino processa:
   - Lê mouse via USB Host Shield
   - Lê comandos do Aimmy via Serial
   - Combina ambos

4. Arduino -> HID USB -> PC
   (Mouse virtual unificado)

5. PC -> Serial -> Arduino
   (Comandos do Aimmy: movimento, click)
```

### Protocolos usados

```
┌───────────────────────────────────────┐
│ Mouse Físico                              │
└───────────┬───────────────────────────┘
            │ USB HID Protocol
            │ (HID Report Descriptor)
┌───────────┴───────────────────────────┐
│ USB Host Shield (MAX3421E)              │
└───────────┬───────────────────────────┘
            │ SPI (Serial Peripheral Interface)
            │ MISO, MOSI, SCK, SS
┌───────────┴───────────────────────────┐
│ Arduino Leonardo (ATmega32U4)           │
│   - Processa dados                       │
│   - Combina movimentos                   │
└────┬────────────────────┬─────────────┘
     │                      │
     │ USB HID              │ Serial (UART)
     │ (Mouse virtual)      │ 57600 baud
     │                      │
┌────┴────────────────────┴─────────────┐
│ PC / Computador                          │
│   - Vê apenas 1 mouse (HID)              │
│   - Aimmy envia comandos (Serial)        │
└──────────────────────────────────────────┘
```

---

## 🔍 Verificação de Conexões

### Checklist visual

```
☐ USB Host Shield bem encaixado no Arduino
☐ Todas as 3 pontes soldadas no Shield
☐ Arduino conectado ao PC via Micro USB
☐ LED do Arduino aceso (indica alimentação)
☐ Mouse conectado na porta USB do Shield
☐ LED do mouse aceso (se tiver)
☐ Device Manager mostra "Arduino Leonardo" (COM port)
☐ Não há "Unknown Device" no Device Manager
```

### Teste elétrico

Com multímetro:

```
1. Arduino desconectado
2. Medir continuidade:
   - Entre pinos do Shield e Arduino
   - Verificar se todos estão conectados
   
3. Arduino conectado:
   - Medir 5V entre pino 5V e GND
   - Medir 3.3V entre pino 3.3V e GND
```

---

## ⚠️ Problemas Comuns de Conexão

### Shield não inicializa

**Sintoma:** LED pisca rápido, "OSC did not start"

**Causas:**
```
❌ Pontes não soldadas
❌ Shield mal encaixado
❌ Pinos entortados
❌ Shield defeituoso
```

**Solução:**
```
✅ Verificar solda das 3 pontes
✅ Reencaixar shield firmemente
✅ Inspecionar pinos (devem estar retos)
✅ Testar com outro shield se possível
```

### Mouse não detectado

**Sintoma:** Mouse conectado mas não funciona

**Causas:**
```
❌ Ponte J3 não soldada (alimentação USB)
❌ Mouse incompatível (Bluetooth, wireless)
❌ Porta USB do shield defeituosa
```

**Solução:**
```
✅ Verificar solda de J3 especialmente
✅ Testar com mouse USB com fio simples
✅ Verificar se LED do mouse acende
```

### PC não vê Arduino

**Sintoma:** Device Manager não mostra porta COM

**Causas:**
```
❌ Drivers CH340 não instalados
❌ Cabo Micro USB defeituoso (só carga)
❌ Porta USB do PC sem energia suficiente
```

**Solução:**
```
✅ Instalar drivers CH340
✅ Testar outro cabo Micro USB
✅ Usar porta USB 2.0 traseira do PC
```

---

## 📊 Diagrama de Pinagem

### Arduino Leonardo (ATmega32U4)

```
        ┌─────────────────────────────┐
        │  Arduino Leonardo R3       │
        │                            │
        │  D13 ●● SCK  (SPI)        │
        │  D12 ●● MISO (SPI)        │
        │  D11 ●● MOSI (SPI)        │
        │  D10 ●● SS   (SPI)        │
        │  D9  ●●                  │
        │  D8  ●● INT  (Shield)    │
        │  D7  ●● GPX  (Shield)    │
        │                            │
        │  5V  ●●                  │
        │  GND ●●                  │
        │  3V3 ●●                  │
        │                            │
        │       [■] Micro USB        │
        └─────────────────────────────┘
```

### USB Host Shield Pinout

```
Pinos conectados ao Arduino:

SPI (Comunicação):
- Pin 10 (SS)   -> Shield CS
- Pin 11 (MOSI) -> Shield MOSI
- Pin 12 (MISO) -> Shield MISO
- Pin 13 (SCK)  -> Shield SCK

Controle:
- Pin 7 (GPX)   -> Shield GPX
- Pin 8 (INT)   -> Shield INT
- Pin 9 (RST)   -> Shield RST

Alimentação:
- 5V   -> Shield VCC
- GND  -> Shield GND
```

---

## 🛠️ Ferramentas Necessárias

### Para montagem

```
✅ Ferro de solda (30-40W)
✅ Solda (com fluxo)
✅ Suporte para solda
✅ Esponja úmida
✅ Multímetro (opcional, recomendado)
✅ Lupa (opcional, ajuda a ver pontes)
```

### Para diagnóstico

```
✅ Multímetro (continuidade, voltagem)
✅ Cabo Micro USB extra (para testar)
✅ Mouse USB simples (para testar)
```

---

## ✅ Próximos Passos

Após montar o hardware:

1. 💻 [Instalar software](../README.md#-instalação-do-software)
2. 🔧 [Configurar mouse](MOUSE_COMPATIBILITY.md)
3. 🔍 [Troubleshooting](TROUBLESHOOTING.md) se necessário
4. 🚀 [Usar com Aimmy](../README.md#-uso-com-aimmy-cuda)

---

**🔌 Boa montagem!**
