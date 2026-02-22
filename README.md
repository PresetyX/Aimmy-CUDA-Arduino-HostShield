# Aimmy CUDA Arduino - Versão USB Host Shield

🇧🇷 **Versão adaptada do [Aimmy-CUDA-Arduino](https://github.com/whoswhip/Aimmy-CUDA-Arduino) para funcionar com USB Host Shield**

## ⚠️ AVISO IMPORTANTE

**Este projeto é para fins educacionais e de pesquisa. O uso de cheats/hacks em jogos online é contra os termos de serviço da maioria dos jogos e pode resultar em banimento permanente.**

---

## 🔧 O que é este projeto?

Esta é uma adaptação do Aimmy Arduino Edition que **funciona com USB Host Shield**, permitindo:

- **Entrada única de mouse**: Seu mouse físico conecta no Host Shield, que repassa os movimentos para o PC
- **Sem detecção de segundo mouse**: Valorant e outros jogos com anti-cheat só vêem UM dispositivo HID
- **Comunicação via HID pura**: Sem porta COM, usando apenas HID Reports
- **Botões extras funcionando**: Suporte para MB4 e MB5 (botões laterais)

### 🎯 Diferença da versão original

O repositório original `whoswhip/Aimmy-CUDA-Arduino` **NÃO funciona com USB Host Shield**. Esta versão implementa:

1. **Parser customizado de mouse** (`hidcustom.h`) - Lê dados do seu mouse físico
2. **Biblioteca ImprovedMouse** - Emulação HID com 5 botões
3. **Integração com Aimmy** - Recebe comandos do Aimmy via Serial

---

## 📦 Hardware Necessário

### Componentes obrigatórios:

| Item | Especificação | Preço aprox. |
|------|----------------|---------------|
| **Arduino Leonardo R3** | Com chip ATmega32U4 | R$ 50-80 |
| **USB Host Shield 2.0** | Compatível com Leonardo | R$ 40-70 |
| **Mouse USB** | Qualquer mouse USB padrão | Seu mouse atual |
| **Cabo Micro USB** | Para conectar Arduino ao PC | R$ 10-20 |

### Onde comprar no Brasil:
- **Mercado Livre**: Procure por "Arduino Leonardo R3" e "USB Host Shield"
- **AliExpress**: Preços menores, mas demora 30-60 dias
- **Lojas especializadas**: Baú da Eletrônica, FilipeFlop, Usinainfo

---

## 🔌 Montagem do Hardware

### Passo 1: Soldar as pontes no USB Host Shield

⚠️ **IMPORTANTE**: O USB Host Shield precisa de 3 pontes soldadas para funcionar!

```
[J1] = 5V    → Solde a ponte
[J2] = 3.3V  → Solde a ponte  
[J3] = 5V    → Solde a ponte
```

**Como soldar:**
1. Use um ferro de solda de 30-40W
2. Aplique solda nas 3 pontes marcadas no shield
3. Verifique se as conexões estão sólidas

📹 **Referência visual**: [Imagem das pontes](https://chome.nerpa.tech/usb-host-shield-hardware-manual/)

### Passo 2: Montar o shield no Arduino

```
Arduino Leonardo R3
        ↑
  [Encaixe o Shield aqui]
        ↑
USB Host Shield 2.0
```

1. Alinhe os pinos do shield com os do Arduino
2. Pressione firmemente até encaixar completamente
3. Verifique se todos os pinos estão inseridos

### Passo 3: Conexões

```
[Mouse USB] → [Porta USB do Host Shield] → [Arduino Leonardo] → [PC via Micro USB]
```

---

## 💻 Instalação do Software

### 1. Instalar Arduino IDE

1. Baixe o [Arduino IDE 1.8.19](https://downloads.arduino.cc/arduino-1.8.19-windows.exe)
2. Instale normalmente
3. Abra a Arduino IDE

### 2. Instalar Biblioteca USB Host Shield 2.0

**Método 1 - Pelo gerenciador de bibliotecas (recomendado):**

1. Na Arduino IDE, vá em: `Sketch` → `Incluir Biblioteca` → `Gerenciar Bibliotecas...`
2. Na barra de busca, digite: `USB Host Shield Library 2.0`
3. Clique em **Instalar** na biblioteca de mesmo nome
4. Aguarde a instalação concluir

**Método 2 - Manual:**

1. Baixe: [USB_Host_Shield_2.0.zip](https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip)
2. Extraia o arquivo
3. Renomeie a pasta de `USB_Host_Shield_2.0-master` para `USB_Host_Shield_2.0`
4. Mova para: `C:\Users\SEU_USUARIO\Documents\Arduino\libraries\`
5. Reinicie a Arduino IDE

### 3. Fazer Upload do Código

1. Conecte o Arduino Leonardo ao PC via cabo Micro USB
2. Na Arduino IDE:
   - **Ferramentas** → **Placa** → **Arduino Leonardo**
   - **Ferramentas** → **Porta** → Selecione a porta COM do Arduino
3. Abra o arquivo `Arduino_HostShield.ino`
4. Clique em **Upload** (→)
5. Aguarde a mensagem: `Done uploading.`

### 4. Testar o Arduino

Após o upload:

1. O LED do Arduino deve piscar e depois apagar (indicação de sucesso)
2. Conecte seu mouse na porta USB do Host Shield
3. Abra o **Monitor Serial**: `Ferramentas` → `Monitor Serial`
4. Configure para **57600 baud**
5. Mova o mouse - o PC deve detectar movimento normalmente

---

## ⚙️ Ajuste Fino (Se necessário)

### Problema: Mouse não funciona corretamente

Alguns mouses (especialmente gamer de alta precisão) usam formato de dados diferente.

**Solução:**

1. Abra `Arduino_HostShield/hidcustom.h`
2. Localize a estrutura `MYMOUSEINFO`:

```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;   // Botões
  uint16_t dX;       // Delta X (movimento horizontal)
  uint16_t dY;       // Delta Y (movimento vertical)  
  int8_t wheel;      // Scroll
};
```

3. **Para mouses de 8-bit** (maioria dos mouses comuns), mude para:

```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;
  int8_t dX;    // Mudado de uint16_t para int8_t
  int8_t dY;    // Mudado de uint16_t para int8_t
  int8_t wheel;
};
```

4. **Para depurar seu mouse específico:**

   a. Abra `C:\Users\SEU_USUARIO\Documents\Arduino\libraries\USB_Host_Shield_2.0\settings.h`
   
   b. Mude:
   ```cpp
   #define ENABLE_UHS_DEBUGGING 0
   ```
   para:
   ```cpp
   #define ENABLE_UHS_DEBUGGING 1
   ```
   
   c. Faça upload novamente no Arduino
   
   d. Abra o Monitor Serial (57600 baud)
   
   e. Mova o mouse e anote os bytes que aparecem:
   ```
   Byte 0: Botões
   Byte 1: Delta X
   Byte 2: Delta Y  
   Byte 3: Scroll
   ```
   
   f. Ajuste `MYMOUSEINFO` conforme o formato do seu mouse

### Mouses testados com sucesso:

- ✅ Logitech G102/G203
- ✅ Logitech G502  
- ✅ Razer DeathAdder V2
- ✅ Mouses genéricos USB
- ❌ Alguns mouses wireless (podem ter incompatibilidade)

---

## 🚀 Uso com Aimmy CUDA

Depois de configurar o Arduino:

1. Baixe o Aimmy CUDA original do [repositório oficial](https://github.com/whoswhip/Aimmy-CUDA-Arduino/releases)
2. **NÃO precisa modificar o Aimmy** - ele continua enviando comandos via Serial
3. Configure no Aimmy:
   - **Mouse Movement**: Arduino
   - **COM Port**: A porta do seu Arduino Leonardo
   - **Baud Rate**: 57600
4. O Aimmy enviará comandos como:
   - `m10,5` - Mover mouse 10px direita, 5px baixo
   - `c` - Click
   - `p` - Press
   - `r` - Release

---

## 📝 Como Funciona

### Fluxo de dados:

```
1. Mouse Físico → USB Host Shield
   └─> Lê movimentos e botões via HID Report

2. Arduino processa:
   ├─> Movimentos do mouse físico
   └─> Comandos do Aimmy via Serial

3. Arduino envia para PC:
   └─> HID Report único (PC vê apenas 1 mouse)
```

### Vantagem sobre versão sem Host Shield:

| Sem Host Shield | Com Host Shield |
|----------------|------------------|
| Mouse real conectado no PC | Mouse conectado no Arduino |
| Arduino = 2º mouse | Arduino = mouse único |
| Valorant detecta 2 dispositivos | Valorant vê 1 dispositivo |
| ❌ Pode ser detectado | ✅ Não detectado |

---

## 🔍 Troubleshooting

### Problema: "Arduino not detected"

**Solução:**
1. Instale drivers CH340: [Driver Download](https://www.arduined.eu/ch340-windows-10-driver-download/)
2. Reinicie o PC
3. Tente outra porta USB (use USB 2.0, não 3.0)

### Problema: "OSC did not start"

**Solução:**
1. Verifique se as 3 pontes estão soldadas no shield
2. Verifique se o shield está bem encaixado
3. Teste com outro Arduino se possível

### Problema: Mouse movendo errado

**Solução:**
1. Ajuste `MYMOUSEINFO` conforme seção "Ajuste Fino"
2. Teste com mouse USB simples primeiro
3. Use modo debug para ver formato dos dados

### Problema: Botões MB4/MB5 não funcionam

**Solução:**
Alguns jogos não reconhecem MB4/MB5 via HID. Você pode:
1. Usar biblioteca Keyboard para emular teclas
2. Modificar `ImprovedMouse.cpp` para mapear para teclas

---

## 📂 Estrutura do Repositório

```
Aimmy-CUDA-Arduino-HostShield/
├── Arduino_HostShield/
│   ├── Arduino_HostShield.ino    # Código principal
│   ├── hidcustom.h               # Parser do mouse físico
│   ├── ImprovedMouse.h           # Header da emulação HID
│   └── ImprovedMouse.cpp         # Implementação HID
├── docs/
│   ├── wiring_diagram.png       # Diagrama de conexões
│   └── solder_bridges.jpg       # Foto das pontes
└── README.md                    # Este arquivo
```

---

## 👥 Créditos

- **whoswhip** - [Aimmy-CUDA-Arduino original](https://github.com/whoswhip/Aimmy-CUDA-Arduino)
- **SunOner** - [HID_Arduino implementation](https://github.com/SunOner/HID_Arduino)
- **felis** - [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0)
- **UnknownCheats Forum** - Documentação técnica e discussões

---

## ⚠️ Aviso Legal

Este projeto é **exclusivamente para fins educacionais e de pesquisa**.

- O uso de cheats em jogos online viola os Termos de Serviço
- Você pode ser **banido permanentemente**
- Este projeto não garante que você não será detectado
- Use por sua própria conta e risco
- Os autores não se responsabilizam por banimentos ou danos

---

## 💬 Suporte

Para dúvidas e problemas:

1. 🐛 **Issues**: Abra uma issue neste repositório
2. 💬 **Discussões**: Use a aba Discussions
3. 📚 **Documentação**: Leia o [UnknownCheats thread](https://www.unknowncheats.me/forum/valorant/642071-arduino-hid-mouse-free-libraries.html)

**NÃO peça ajuda no Discord do Aimmy - eles não dão suporte para esta versão customizada.**

---

## 📝 Licença

MIT License - Veja LICENSE para detalhes

---

**🇧🇷 Feito no Brasil | Made in Brazil**
