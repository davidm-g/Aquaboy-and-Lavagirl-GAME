#ifndef __MODEL_H
#define __MODEL_H
#include "../devices/graphics/vbe.h"
#include "../draws/draw.h"
#include "defines.h"
#include "devices/graphics/video.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/timer/i8254.h"
#include "devices/timer/timer.h"
#include "devices/utils.h"
#include "sprite.h"
#include "visuals/xpms/LAVABOY.xpm"
#include "visuals/xpms/LAVABOY2.xpm"
#include "visuals/xpms/hand.xpm"
#include "visuals/xpms/wall.xpm"
#include "visuals/xpms/wall2.xpm"
#include <lcom/lcf.h>
typedef enum {
  RUNNING,
  EXIT
} SystemState;

void(timer_int_handler)();
void load_sprites();
int checkCollision(Sprite *sp, uint16_t x, uint16_t y);
void update_timer();
void update_keyboard();
void update_mouse();
void destroy_sprites();
int move(Sprite *sp);
void move_cursor(struct packet *pp);
#endif
