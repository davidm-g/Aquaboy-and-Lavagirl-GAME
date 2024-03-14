#include "keyboard.h"
#include "i8042.h"
#include "utils.h"
#include <lcom/lcf.h>
#include <stdint.h>

int hook_id = 1;
uint8_t kbd_outbuf;

void(kbc_ih)() {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf() == 0) {
      if (util_sys_inb(OUT_BUF, &kbd_outbuf) == 0) {
        return;
      }
    }
    attempts--;
  }
}

int(keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int can_read_outbuf() {
  uint8_t st = 0;
  // read status
  if (util_sys_inb(ST_REGISTER, &st) != 0) {
    return 1;
  }
  // empty out_buf
  if ((st & OBF) == 0) {
    return 1;
  }
  // timeout error
  if ((st & TIM_ERR) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  // parity error
  if ((st & PAR_ERR) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  
  // mouse data in buffer
  if ((st & AUX) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }

  return 0;
}
