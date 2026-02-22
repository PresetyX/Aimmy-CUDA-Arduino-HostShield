/*
 * Exemplos de Configuração para Diferentes Mouses
 * 
 * Copie a configuração apropriada para seu hidcustom.h
 */

// ============================================================================
// EXEMPLO 1: Mouse Genérico USB (8-bit)
// ============================================================================
// Use para:
// - Mouses USB básicos/baratos
// - Mouses de escritório
// - Mouses que não são "gamer"
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;   // Botões: 1=Left, 2=Right, 4=Middle, 8=Button4, 16=Button5
  int8_t dX;         // Movimento horizontal (-127 a +127)
  int8_t dY;         // Movimento vertical (-127 a +127)
  int8_t wheel;      // Scroll (-127 a +127)
};


// ============================================================================
// EXEMPLO 2: Logitech G102 / G203 (16-bit)
// ============================================================================
// Use para:
// - Logitech G102, G203
// - Logitech G402, G403
// - Mouses que suportam movimentos rápidos/precisos
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;
  int16_t dX;        // Movimento horizontal 16-bit (-32768 a +32767)
  int16_t dY;        // Movimento vertical 16-bit
  int8_t wheel;
};


// ============================================================================
// EXEMPLO 3: Logitech G502 (16-bit com padding)
// ============================================================================
// Use para:
// - Logitech G502
// - Logitech G602
// - Mouses que enviam dados extras
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;
  int16_t dX;
  int16_t dY;
  int8_t wheel;
  uint8_t padding[3];  // Bytes extras que o G502 envia
};


// ============================================================================
// EXEMPLO 4: Razer DeathAdder V2 (8-bit com botões extras)
// ============================================================================
// Use para:
// - Razer DeathAdder
// - Razer Viper
// - Razer Mamba
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;        // Botões principais (1-5)
  int8_t dX;
  int8_t dY;
  int8_t wheel;
  uint8_t extraButtons;   // Botões extras (6-7)
};


// ============================================================================
// EXEMPLO 5: Mouse Gamer Genérico com Alta Precisão
// ============================================================================
// Use para:
// - Mouses acima de 3200 DPI
// - Mouses que movem muito rápido
// - Quando movimento parece "travado" com int8_t
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;
  int16_t dX;        // 16-bit para capturar movimentos rápidos
  int16_t dY;
  int8_t wheel;
};


// ============================================================================
// EXEMPLO 6: Microsoft Basic Optical Mouse
// ============================================================================
// Use para:
// - Microsoft Basic Optical
// - Mouses Microsoft simples
// ============================================================================

struct MYMOUSEINFO
{
  uint8_t buttons;
  int8_t dX;
  int8_t dY;
  int8_t wheel;
};


// ============================================================================
// COMO USAR ESTES EXEMPLOS
// ============================================================================
/*

1. Identifique seu mouse na lista acima

2. Copie a struct MYMOUSEINFO correspondente

3. Cole em Arduino_HostShield/hidcustom.h, substituindo a struct existente:

   // Em hidcustom.h:
   #include <hidboot.h>
   #include "ImprovedMouse.h"
   
   #define CHECK_BIT(var, pos) ((var)&pos)
   
   // COLE AQUI A STRUCT DO EXEMPLO:
   struct MYMOUSEINFO
   {
     // ... config do exemplo ...
   };
   
   class MouseRptParser : public MouseReportParser
   {
     // ... resto do código permanece igual ...
   };

4. Faça upload do código novamente no Arduino

5. Teste o mouse:
   - Movimente suavemente: deve seguir corretamente
   - Movimente rapidamente: não deve travar ou inverter
   - Teste todos os botões
   - Teste o scroll

6. Se ainda não funcionar:
   - Ative o modo debug (veja TROUBLESHOOTING.md)
   - Analise a saída do Monitor Serial
   - Ajuste a struct conforme os bytes observados

*/


// ============================================================================
// CONFIGURAÇÃO AVANÇADA - INVERTENDO MOVIMENTO
// ============================================================================
/*

Se o mouse move ao contrário (esquerda vira direita, cima vira baixo):

1. NÃO mude a struct

2. Em Arduino_HostShield.ino, modifique OnMouseMove:

void MouseRptParser::OnMouseMove(MYMOUSEINFO *mi)
{
    delta[0] = -mi->dX;  // Note o sinal negativo
    delta[1] = -mi->dY;  // Note o sinal negativo
}

Ou apenas um eixo:

void MouseRptParser::OnMouseMove(MYMOUSEINFO *mi)
{
    delta[0] = mi->dX;   // X normal
    delta[1] = -mi->dY;  // Apenas Y invertido
}

*/


// ============================================================================
// CONFIGURAÇÃO AVANÇADA - SENSIBILIDADE
// ============================================================================
/*

Para ajustar sensibilidade (fazer mouse mover mais rápido/devagar):

void MouseRptParser::OnMouseMove(MYMOUSEINFO *mi)
{
    // Multiplica movimento por 2 (2x mais rápido):
    delta[0] = mi->dX * 2;
    delta[1] = mi->dY * 2;
    
    // Divide movimento por 2 (2x mais devagar):
    // delta[0] = mi->dX / 2;
    // delta[1] = mi->dY / 2;
}

Atenção: Isso pode causar "jitter" se usado com valores muito altos!

*/


// ============================================================================
// CONFIGURAÇÃO AVANÇADA - SCROLL INVERTIDO
// ============================================================================
/*

Se scroll funciona ao contrário:

void MouseRptParser::OnWheelMove(MYMOUSEINFO *mi)
{
    Mouse.move(0, 0, -mi->wheel);  // Note o sinal negativo
}

*/
