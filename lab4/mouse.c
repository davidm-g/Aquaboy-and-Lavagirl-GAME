#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>

int mouse_hook_id = 2;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}