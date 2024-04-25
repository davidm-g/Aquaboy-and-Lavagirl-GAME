#include <lcom/lcf.h>
#include "devices/utils.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/graphics/video.h"
#include "devices/graphics/vbe.h"
#include "visuals/xpms/background.xpm"
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

int (proj_main_loop)(int argc, char **argv) {
    map_phys_virt(0x115);
    if(set_graphic_mode(0x115)!=0) return 1;
    if(print_xpm((xpm_map_t) background_xpm, 0, 0)!=0) return 1;
    if(wait_esc_key()!=0) return 1;
    if(vg_exit()!=0) return 1; 
    return 0; 
}
