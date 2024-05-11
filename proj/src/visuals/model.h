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
Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y);
uint8_t getSideCollision(Sprite *sp, Sprite *wall);
void update_timer();
void update_keyboard();
void update_mouse();
void destroy_sprites();
void move_cursor(struct packet *pp);
void action_handler(Sprite *sp);
int update(Sprite *sp);
void updateSpeed(Sprite *sp);
int updatePosition(Sprite *sp);
int move_x(Sprite *sp);
int move_y(Sprite *sp);
void set_ground(Sprite *sp);
void jump(Sprite *sp);
void gravity(Sprite *sp);
void left(Sprite *sp);
void right(Sprite *sp);
#endif
