#ifndef __DRAW_H
#define __DRAW_H
#include "../devices/graphics/vbe.h"
#include "../visuals/sprite.h"
#include <lcom/lcf.h>
#include "../visuals/model.h"
#include "visuals/xpms/background.xpm"
int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);
int print_background (xpm_map_t xpm);
int erase_sprite(Sprite *sprite, xpm_map_t xpm);
void draw_frame();
void draw_mouse();
#endif
