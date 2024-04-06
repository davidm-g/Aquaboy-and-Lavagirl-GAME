#include <lcom/lcf.h>

#include "keyboard.h"
#include "timer.h"
#include <i8042.h>
#include <i8254.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

extern int sys_inb_counter;
extern uint8_t kbd_outbuf;
extern int global_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t kbd_bit_no = 0x01, bytes[2];
  int ipc_status, r, size = 0;
  message msg;
  bool make = false;

  if ((keyboard_subscribe_int(&kbd_bit_no)) != 0) {
    return 1;
  }

  while (kbd_outbuf != ESC_BREAK) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kbd_bit_no) {
            kbc_ih();
            if (size == 0) {
              bytes[0] = kbd_outbuf;
              size++;
              if (kbd_outbuf == 0xE0) {
                continue;
              }
              else {
                if ((kbd_outbuf & BIT(7)) != 0) {
                  make = false;
                }
                else {
                  make = true;
                }
              }
            }
            else {
              bytes[1] = kbd_outbuf;
              size++;
              if ((kbd_outbuf & BIT(7)) != 0) {
                make = false;
              }
              else {
                make = true;
              }
            }
            if (kbd_print_scancode(make, size, bytes) != 0) {
              return 1;
            }
            size = 0;
          }
          break;
        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  kbd_print_no_sysinb(sys_inb_counter);

  return keyboard_unsubscribe_int();
}

int(kbd_test_poll)() {
  uint8_t bytes[2];
  int size = 0;
  bool make = false;
  while (kbd_outbuf != ESC_BREAK) {
    if (can_read_outbuf() == 0) {
      if (util_sys_inb(OUT_BUF, &kbd_outbuf) != 0) {
        return 1;
      }
      if (size == 0) {
        bytes[0] = kbd_outbuf;
        size++;
        if (kbd_outbuf == 0xE0) {
          continue;
        }
        else {
          if ((kbd_outbuf & BIT(7)) != 0) {
            make = false;
          }
          else {
            make = true;
          }
        }
      }
      else {
        bytes[1] = kbd_outbuf;
        size++;
        if ((kbd_outbuf & BIT(7)) != 0) {
          make = false;
        }
        else {
          make = true;
        }
      }
      if (kbd_print_scancode(make, size, bytes) != 0) {
        return 1;
      }
      size = 0;
    }
  }
  if (restore_interrupts())
    return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t kbd_bit_no = 0x01, timer_bit_no = 0x00, bytes[2];
  int ipc_status, r, size = 0, last_key_time = 0;
  message msg;
  bool make = false;
  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if ((keyboard_subscribe_int(&kbd_bit_no)) != 0) {
    return 1;
  }

  while (kbd_outbuf != ESC_BREAK && ((global_counter - last_key_time) < (n * 60))) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_bit_no) {
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & kbd_bit_no) {
            last_key_time = global_counter;
            kbc_ih();
            if (size == 0) {
              bytes[0] = kbd_outbuf;
              size++;
              if (kbd_outbuf == 0xE0) {
                continue;
              }
              else {
                if ((kbd_outbuf & BIT(7)) != 0) {
                  make = false;
                }
                else {
                  make = true;
                }
              }
            }
            else {
              bytes[1] = kbd_outbuf;
              size++;
              if ((kbd_outbuf & BIT(7)) != 0) {
                make = false;
              }
              else {
                make = true;
              }
            }
            if (kbd_print_scancode(make, size, bytes) != 0) {
              return 1;
            }
            size = 0;
          }
          break;
        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  kbd_print_no_sysinb(sys_inb_counter);

  if (timer_unsubscribe_int() != 0)
    return 1;

  return keyboard_unsubscribe_int();
}
