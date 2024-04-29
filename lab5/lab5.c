// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "i8042.h"
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "timer.h"
#include "utils.h"
#include "vbe.h"
#include "video.h"
extern uint8_t kbd_outbuf;
extern vbe_mode_info_t vmi_p;
extern uint8_t  global_counter;
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_graphic_mode(mode) != 0)
    return 1;
  sleep(delay);
  if (vg_exit() != 0)
    return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  map_phys_virt(mode);
  if (set_graphic_mode(mode) != 0)
    return 1;
  if (vg_draw_rectangle(x, y, width, height, color) != 0)
    return 1;
  if (wait_esc_key() != 0)
    return 1;
  if (vg_exit() != 0)
    return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  map_phys_virt(mode);
  if (set_graphic_mode(mode) != 0)
    return 1;
  uint16_t vertical = vmi_p.YResolution / no_rectangles;
  uint16_t horizontal = vmi_p.XResolution / no_rectangles;
  uint32_t color;
  for (uint16_t i = 0; i < no_rectangles; i++) {   // row
    for (uint16_t j = 0; j < no_rectangles; j++) { // column
      if (vmi_p.MemoryModel == DIRECT_COLOR) {
        uint32_t red = red_value(first_red(first), j, step);
        uint32_t green = green_value(first_green(first), i, step);
        uint32_t blue = blue_value(first_blue(first), i, j, step);
        color = direct_mode(red, green, blue);
      }
      else {
        color = index_indexed_mode(i, j, no_rectangles, first, step);
      }
      if (vg_draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color) != 0)
        return 1;
    }
  }
  if (wait_esc_key() != 0)
    return 1;
  if (set_graphic_mode(0) != 0)
    return 1;
  return 0;
}


int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  map_phys_virt(0x105);
  if (set_graphic_mode(0x105) != 0)
    return 1;
  if (print_xpm(xpm, x, y) != 0)
    return 1;
  if (wait_esc_key() != 0)
    return 1;
  if (vg_exit() != 0)
    return 1;
  return 0;
}


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  map_phys_virt(0x105);
  if (set_graphic_mode(0x105) != 0)
    return 1;
  if (print_xpm(xpm, xi, yi) != 0)
    return 1;
  bool horizontal = false;
  if (xi != xf) {
    horizontal = true;
  }
  int16_t displacement;
  bool normal;
  normal = getSpeedDir(xi, yi, xf, yf, speed, &displacement);

  uint8_t kbd_bit_no = 0x01, timer_bit_no = 0x00;
  int ipc_status, r;
  message msg;
  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if ((keyboard_subscribe_int(&kbd_bit_no)) != 0)
    return 1;
  timer_set_frequency(0, fr_rate);
  xpm_image_t img; 
  xpm_load(xpm, XPM_INDEXED, &img); //get pixmap from XPM

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
            vg_draw_rectangle(xi, yi, img.width, img.height, 0);
            if (normal) { // we move "displacement" bytes
              if (horizontal) {
                xi += displacement;
                if ((xf - xi) * displacement < 0)
                  xi = xf; // if we passed the final position
              }
              else {
                yi += displacement;
                if ((yf - yi) * displacement < 0)
                  yi = yf; // if we passed the final position
              }
            }
            else { // we wait "-speed" frames to move 1 byte
              if (global_counter % (-speed) == 0) {
                if (horizontal) {
                  xi += displacement;
                  if ((xf - xi) * displacement < 0)
                    xi = xf; // if we passed the final position
                }
                else {
                  yi += displacement;
                  if ((yf - yi) * displacement < 0)
                    yi = yf; // if we passed the final position
                }
              }
            }
            if (print_xpm(xpm, xi, yi) != 0)
              return 1;
          }
          if (msg.m_notify.interrupts & kbd_bit_no)
            kbc_ih();
          break;
        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  if (vg_exit() != 0)
    return 1;

  if (timer_unsubscribe_int() != 0)
    return 1;

  return keyboard_unsubscribe_int();
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);
  return 1;
}
