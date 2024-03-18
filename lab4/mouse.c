#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>

int mouse_hook_id = 2;
uint8_t kbd_outbuf;
int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int write_to_kbc(uint8_t port, uint8_t cmdbyte) {
  uint8_t st;
  int attempts = 10;
  while (attempts > 0) {
    if (util_sys_inb(ST_REGISTER, &st) != 0) {
      return 1;
    } /* assuming it returns OK */ /* loop while 8042 input buffer is not empty */
    if ((st & IBF) == 0) {         // if input buffer is not full
      if (sys_outb(port, cmdbyte) != 0) {
        return 1;
      } /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int read_value_data_from_kbc(uint8_t port, uint8_t *output) {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf() == 0) {
      if (util_sys_inb(OUT_BUF, output) == 0) { // read data from output buffer
        return 0;
      }
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int can_read_outbuf() {
  uint8_t st = 0;
  // read status
  if (util_sys_inb(ST_REGISTER, &st) != 0) {
    return 1;
  }
  // empty out_buf
  if ((st & OBF) == 0){
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
  // mouse data not in buffer
  if ((st & AUX) == 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  
  return 0;
}
int send_cmd_mouse(uint8_t cmd){ //function to send commands to mouse
    uint8_t response =0X00;
    int attempts=10;
    while(attempts>0){
      if(write_to_kbc(KBC_CMD_REG,REQUEST_MOUSE)) return 1; //write 0xD4 to port 0x64
      if(write_to_kbc(WRITE_CMD_BYTE,cmd)) return 1; //	write the code for the command to port 0x60 
      if(read_value_data_from_kbc(OUT_BUF,&response)) return 1; //read the acknowledgement byte 
      if(response==ACK) return 0; // when ack, ggwp
        attempts--;
    }
    return 1;
}
void(mouse_ih)() {
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