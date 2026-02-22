# 🖌️ Guia de Compatibilidade de Mouses

## 📋 Lista de Mouses Testados

### ✅ Mouses 100% Compatíveis

| Marca | Modelo | Formato de Dados | Observações |
|-------|--------|------------------|---------------|
| **Logitech** | G102 / G203 | 16-bit (uint16_t dX/dY) | Funciona perfeitamente com config padrão |
| **Logitech** | G502 | 16-bit com padding | Requer `uint8_t padding[3]` extra |
| **Razer** | DeathAdder V2 | 8-bit (int8_t dX/dY) | Mudar para int8_t na struct |
| **Genérico** | Mouses USB básicos | 8-bit | Maioria funciona com int8_t |
| **Microsoft** | Basic Optical Mouse | 8-bit | Config int8_t |

### ⚠️ Mouses com Problemas Conhecidos

| Marca | Modelo | Problema | Solução |
|-------|--------|----------|----------|
| **Logitech** | G Pro Wireless | Wireless pode não funcionar | Use dongle USB unifying |
| **Razer** | Viper Ultimate | Wireless instável | Conecte via cabo |
| **Corsair** | Dark Core RGB | Protocolo customizado | Não compatível |

### ❌ Mouses Incompatíveis

| Marca | Modelo | Razão |
|-------|--------|-------|
| **Apple** | Magic Mouse | Protocolo proprietário Bluetooth |
| **Trackballs** | Diversos | Formato de dados diferente |
| **Mouses Bluetooth** | Maioria | USB Host Shield é apenas USB, não BT |

---

## 🔧 Configurações por Tipo de Mouse

### Tipo 1: Mouse Comum (8-bit)

**Características:**
- Mouse USB genérico
- Não-gamer
- Preço baixo

**Configuração em `hidcustom.h`:**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;   // Byte 0
  int8_t dX;         // Byte 1 - Movimento X
  int8_t dY;         // Byte 2 - Movimento Y
  int8_t wheel;      // Byte 3 - Scroll
};
```

**Formato de dados:**
```
Byte 0: 00000001 = Botão esquerdo
Byte 1: 05       = Move 5px direita
Byte 2: FB       = Move 5px cima (-5)
Byte 3: 01       = Scroll up
```

---

### Tipo 2: Mouse Gamer 16-bit (Logitech G-series)

**Características:**
- Alta precisão (acima de 3200 DPI)
- Logitech G102, G203, G402, G502
- Movimentos rápidos detectam valores maiores que 127

**Configuração em `hidcustom.h`:**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;   // Byte 0
  int16_t dX;        // Bytes 1-2 - Movimento X (16-bit)
  int16_t dY;        // Bytes 3-4 - Movimento Y (16-bit)
  int8_t wheel;      // Byte 5 - Scroll
};
```

**Formato de dados:**
```
Byte 0: 00000001     = Botão esquerdo
Bytes 1-2: 0140      = Move 320px direita (0x0140 = 320)
Bytes 3-4: FEC0      = Move 320px cima (-320)
Byte 5: 01           = Scroll up
```

---

### Tipo 3: Mouse Gamer com Padding (Logitech G502)

**Características:**
- G502, G602, alguns modelos G-series
- Envia bytes extras de dados

**Configuração em `hidcustom.h`:**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;      // Byte 0
  int16_t dX;           // Bytes 1-2
  int16_t dY;           // Bytes 3-4
  int8_t wheel;         // Byte 5
  uint8_t padding[3];   // Bytes 6-8 (dados extras)
};
```

---

### Tipo 4: Razer com Botões Extras

**Características:**
- DeathAdder, Mamba, Viper
- Até 7 botões

**Configuração em `hidcustom.h`:**
```cpp
struct MYMOUSEINFO
{
  uint8_t buttons;      // Byte 0 - Botões 1-5
  int8_t dX;            // Byte 1
  int8_t dY;            // Byte 2
  int8_t wheel;         // Byte 3
  uint8_t extraButtons; // Byte 4 - Botões 6-7
};
```

---

## 🔍 Como Descobrir o Formato do Seu Mouse

### Passo 1: Ativar Debug

1. Abra: `C:\Users\SEU_USUARIO\Documents\Arduino\libraries\USB_Host_Shield_2.0\settings.h`

2. Mude:
```cpp
#define ENABLE_UHS_DEBUGGING 0
```
para:
```cpp
#define ENABLE_UHS_DEBUGGING 1
```

3. Faça upload do código novamente

### Passo 2: Analisar Saída

Abra Monitor Serial (9600 baud) e observe:

**Exemplo de saída:**
```
Start

Mouse Report (8 bytes):
01 05 FB 00 00 00 00 00
^  ^  ^  ^  ^  ^  ^  ^
|  |  |  |  |  |  |  +- Byte 7
|  |  |  |  |  |  +---- Byte 6
|  |  |  |  |  +------- Byte 5 (scroll)
|  |  |  |  +---------- Byte 4
|  |  |  +------------- Byte 3 (dY low byte)
|  |  +---------------- Byte 2 (dX low byte)
|  +------------------- Byte 1 (buttons)
+---------------------- Byte 0 (report ID)
```

### Passo 3: Testar Movimentos

**Teste 1: Movimento horizontal rápido**
```
Mova mouse rapidamente para a direita
Se Byte 1 > 7F (127): Mouse é 16-bit
Se Byte 1 <= 7F: Mouse é 8-bit
```

**Teste 2: Botões**
```
Clique cada botão e anote qual byte muda:
Botão 1 (esquerdo):  Byte 0 = 01
Botão 2 (direito):   Byte 0 = 02
Botão 3 (meio):      Byte 0 = 04
Botão 4 (lateral 1): Byte 0 = 08
Botão 5 (lateral 2): Byte 0 = 10
```

**Teste 3: Scroll**
```
Role o scroll para cima/baixo
Anote qual byte muda (geralmente Byte 3 ou 5)
```

### Passo 4: Ajustar Estrutura

Baseado nos testes, crie sua struct em `hidcustom.h`:

```cpp
// Template genérico - ajuste conforme seus testes
struct MYMOUSEINFO
{
  uint8_t buttons;      // Sempre Byte 0
  
  // Para 8-bit:
  int8_t dX;
  int8_t dY;
  
  // OU para 16-bit:
  // int16_t dX;
  // int16_t dY;
  
  int8_t wheel;         // Byte que mudou no Teste 3
  
  // Se tiver bytes extras:
  // uint8_t padding[N];  // N = bytes extras
};
```

---

## 📊 Tabela de Referência Rápida

| Sintoma | Tipo | Config dX/dY |
|---------|------|-------------|
| Movimento lento, valores até 127 | 8-bit | `int8_t` |
| Movimento rápido quebra ou inverte | 16-bit | `int16_t` |
| Mouse funciona mas trava às vezes | Padding faltando | Adicione `uint8_t padding[N]` |
| Botões 4/5 não funcionam | Botões em byte extra | Adicione `uint8_t extraButtons` |
| Scroll ao contrário | Sinal invertido | Use `-mi->wheel` no código |

---

## 🧪 Teste de Validação

Após configurar, teste:

1. ✅ **Movimento suave** em todas as direções
2. ✅ **Movimento rápido** sem travamentos
3. ✅ **Todos os botões** respondem corretamente
4. ✅ **Scroll** funciona nas duas direções
5. ✅ **Precisão** mantida em todos DPIs

Se algum falhar, volte ao modo debug e ajuste.

---

## 📝 Contribua!

Testou um mouse não listado? **Abra uma Issue** com:

```markdown
**Marca/Modelo:** Logitech G305
**Funciona:** Sim/Não
**Config usada:**
```cpp
struct MYMOUSEINFO {
  // sua config aqui
};
```
**Observações:** [qualquer detalhe importante]
```

Sua contribuição ajuda outros usuários!

---

**🖌️ Happy mousing!**
