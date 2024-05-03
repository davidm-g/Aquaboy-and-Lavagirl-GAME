#include "devices/graphics/video.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/timer/i8254.h"
#include "devices/timer/timer.h"
#include "devices/utils.h"
#include <lcom/lcf.h>
// #include "devices/graphics/vbe.h"
#include "draws/draw.h"
#include "visuals/model.h"
#include "visuals/xpms/background.xpm"
#include "defines.h"
extern int global_counter;
extern uint8_t kbd_outbuf;
extern uint32_t* background_map;

extern Sprite *lavaboy;
extern Sprite *cursor;
extern Sprite *walls [2];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char **argv) {
  map_phys_virt(VIDEO_MODE);
  if (set_graphic_mode(VIDEO_MODE) != 0)
    return 1;
  if (print_background((xpm_map_t) background_xpm) != 0)
    return 1;
  load_sprites();
  // if(vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00)!=0) return 1;
  if (lavaboy == NULL){
    return 1;
  }
  if (draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy)) != 0)
    return 1;
  if (draw_sprite(cursor, get_posx(cursor), get_posy(cursor)) != 0)
    return 1;
  for(int i = 0; i < 2; i++) {
    if (draw_sprite(walls[i], get_posx(walls[i]), get_posy(walls[i])) != 0)
      return 1;
  }
  // timer_set_frequency(0, 60);
  uint8_t kbd_bit_no = 0x01, timer_bit_no = 0x00,mouse_bit_no = 0x02;
  int ipc_status, r;
  message msg;
  bool change = false;
  buffer_copy();

  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if ((keyboard_subscribe_int(&kbd_bit_no)) != 0)
    return 1;
  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;
  send_cmd_mouse(SET_STREAM_MODE);
  send_cmd_mouse(ENABLE_DATA);
  while (kbd_outbuf != ESC_BREAK) {
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
            if (change) {
              buffer_copy();
              change = false;
            }
          }
          if (msg.m_notify.interrupts & kbd_bit_no) {
            kbc_ih();
            if (kbd_outbuf == W_KEY) {
              change = true;
              if (erase_sprite(lavaboy, (xpm_map_t) background_xpm) != 0)
                return 1;
              if (draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy) - 10))
                draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy));
            }
            else if (kbd_outbuf == A_KEY) {
              change = true;
              if (erase_sprite(lavaboy, (xpm_map_t) background_xpm) != 0)
                return 1;
              if (draw_sprite(lavaboy, get_posx(lavaboy) - 10, get_posy(lavaboy)) != 0)
                draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy));
            }
            else if (kbd_outbuf == S_KEY) {
              change = true;
              if (erase_sprite(lavaboy, (xpm_map_t) background_xpm) != 0)
                return 1;
              if (draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy) + 10) != 0)
                draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy));
            }
            else if (kbd_outbuf == D_KEY) {
              change = true;
              if (erase_sprite(lavaboy, (xpm_map_t) background_xpm) != 0)
                return 1;
              if (draw_sprite(lavaboy, get_posx(lavaboy) + 10, get_posy(lavaboy)) != 0)
                draw_sprite(lavaboy, get_posx(lavaboy), get_posy(lavaboy));
            }
          }
          if(msg.m_notify.interrupts & mouse_bit_no){

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

  if (timer_unsubscribe_int() != 0)
    return 1;
  if (keyboard_unsubscribe_int())
    return 1;
  if(mouse_unsubscribe_int())
    return 1;
  send_cmd_mouse(DISABLE_DATA);
  destroy_sprite(lavaboy);
  destroy_sprite(cursor);
  if (vg_exit() != 0)
    return 1;

  return 0;
}
