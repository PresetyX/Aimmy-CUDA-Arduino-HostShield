#ifndef HIDCUSTOM_H
#define HIDCUSTOM_H

#include <hidboot.h>
#include "ImprovedMouse.h"

#define CHECK_BIT(var, pos) ((var)&pos)

// Estrutura customizada para informações do mouse
// Ajuste os tipos conforme seu mouse específico
struct MYMOUSEINFO
{
  uint8_t buttons;   // Botões do mouse
  uint16_t dX;       // Delta X (movimento horizontal)
  uint16_t dY;       // Delta Y (movimento vertical)
  int8_t wheel;      // Scroll wheel
};

class MouseRptParser : public MouseReportParser
{
  union
  {
    MYMOUSEINFO mouseInfo;
    uint16_t bInfo[sizeof(MYMOUSEINFO)];
  } prevState;

protected:
  void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);

  void OnMouseMove(MYMOUSEINFO *mi);
  void OnWheelMove(MYMOUSEINFO *mi);
};

#endif
