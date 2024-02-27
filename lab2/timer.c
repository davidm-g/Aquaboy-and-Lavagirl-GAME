#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  //printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(st == NULL)
    return 1;
  if(timer < 0 || timer > 2)
    return 1;
  int timer_address;
  uint8_t word;
  switch (timer)
  {
  case 0:
    timer_address=TIMER_0;
    word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(0));
    break;
  case 1:
    timer_address=TIMER_1;
    word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(1));
    break;
  case 2:
    timer_address=TIMER_2;
    word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(2));
    break;
  default:
    return 1;
  }
  sys_outb(TIMER_CTRL, word);  //Writes Read-back command to Control Register
  int r = util_sys_inb(timer_address, st); //read the configuration of the specified timer and writes it to st
  

  return r;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
    union timer_status_field_val val;
    //put in val the configuration fields stated by field, by masking st.
    switch (field)
    {
    case tsf_all:
      val.byte=st;
      break;
    case tsf_initial:
      val.in_mode= (TIMER_LSB_MSB & st) >>4;
      break;
    case tsf_mode:
      val.count_mode= (st&(BIT(1)|BIT(2)|BIT(3)))>>1;
      break;
    case tsf_base:
      val.bcd= (st & TIMER_BCD);
      break;
    default:
      return 1;
      break;
    }
    timer_print_config(timer,field,val);
  return 0;
}
