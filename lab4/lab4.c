// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "timer.h"
// Any header files included below this line should have been created by you
#include "i8042.h"
#include "mouse.h"
extern struct packet packet;
extern uint8_t kbd_outbuf;
extern int byte_counter;
extern int global_counter;
typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
} SystemState;

SystemState state = START;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no = 0x02;
  int ipc_status, r;
  message msg;
  if (mouse_subscribe_int(&bit_no) != 0)
    return 1;
  send_cmd_mouse(ENABLE_DATA); //user version of mouse_enable_data_reporting
  while (cnt > 0) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & bit_no) {
            mouse_ih();
            bytes_to_packet(); //put the byte in the packet, making sure its syncronized
            if(byte_counter==3){ //we can now assemble the packet
              packet_parse();
              mouse_print_packet(&packet);
              byte_counter=0;
              cnt--;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  send_cmd_mouse(DISABLE_DATA); //disable data reporting
  return mouse_unsubscribe_int();

}

int(mouse_test_async)(uint8_t idle_time) {
  uint8_t mouse_bit_no = 0x02, timer_bit_no = 0x00;
  int ipc_status, r, last_key_time = 0;
  message msg;
  send_cmd_mouse(ENABLE_DATA);
  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if ((mouse_subscribe_int(&mouse_bit_no)) != 0) {
    return 1;
  }

  while ((global_counter - last_key_time) < (idle_time * 60)) {
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
          if (msg.m_notify.interrupts & mouse_bit_no) {
            last_key_time = global_counter;
            mouse_ih();
            bytes_to_packet(); //put the byte in the packet, making sure its syncronized
            if(byte_counter==3){ //we can now assemble the packet
              packet_parse();
              mouse_print_packet(&packet);
              byte_counter=0;
            }
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
  send_cmd_mouse(DISABLE_DATA); //disable data reporting
  if (timer_unsubscribe_int() != 0)
    return 1;

  return mouse_unsubscribe_int();
}

uint16_t x_len_total = 0;
uint16_t y_len_total = 0;
void (update_state_machine)(uint8_t tolerance,uint8_t x_len) {

    switch (state) {

      case START:

          // transição I
          // se só o left-button estiver pressionado
          if (packet.lb && !packet.rb && !packet.mb) {
            state = UP;
          }

          break;

      case UP:
          //TODO: transições II, III e F
          break;

      case VERTEX:
          //TODO: transições IV e F
          break;

      case DOWN:
          //TODO: transições V, VI e F
          break;

      case END:
          break;
    }

    
}
int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t bit_no = 0x02;
  int ipc_status, r;
  message msg;
  if (mouse_subscribe_int(&bit_no) != 0)
    return 1;
  send_cmd_mouse(ENABLE_DATA);
   //user version of mouse_enable_data_reporting
  while (state != END) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & bit_no) {
            mouse_ih();
            bytes_to_packet(); //put the byte in the packet, making sure its syncronized
            if(byte_counter==3){ //we can now assemble the packet
              packet_parse();
              mouse_print_packet(&packet);
              update_state_machine(tolerance,x_len);
              byte_counter=0;
              
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  send_cmd_mouse(DISABLE_DATA); //disable data reporting
  return mouse_unsubscribe_int();
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
