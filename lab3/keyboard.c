#include "keyboard.h"
#include "i8042.h"
#include "utils.h"
#include <lcom/lcf.h>
#include <stdint.h>
bool complete = false;
int hook_id = 1;
bool make;
int size = 0;
uint8_t bytes[];

void(complete_flag_true)() {
  complete = true;
}

void(kbc_ih)() {
  uint8_t st;
  if(read_kbc_output() != 0) {
    return;
  }  
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

bool check_for_errors(uint8_t st){
  if ((st & TIM_ERR) == 1 || (st & PAR_ERR) == 1) {
    return true;
  }
  return false;
}

bool check_for_wait(uint8_t st) {
  if ((st & OBF) == 0) {
    return true;
  }
  return false;
}

int read_kbc_output(){
    int attempts = 10;
    uint8_t st;
    while(attempts > 0){
      if(util_sys_inb(ST_REGISTER, &st)!=0){
        return 1;
      }
      if (!((st & OBF) || (st & PAR_ERR) || (st & TIM_ERR))) {
        if (util_sys_inb(OUT_BUF, bytes[size]) == 0) {
          size++;
          return 0;
        }
      }
      tickdelay(micros_to_ticks(20000));
      attempts--;
    }
    return 1;
}