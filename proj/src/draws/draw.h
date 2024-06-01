#ifndef __DRAW_H
#define __DRAW_H
#include "../devices/graphics/vbe.h"
#include "../visuals/model.h"
#include "../visuals/sprite.h"
#include "visuals/xpms/background.xpm"
#include "visuals/xpms/gameover.xpm"
#include "visuals/xpms/leaderboard.xpm"
#include "visuals/xpms/menu.xpm"
#include "visuals/xpms/winscreen.xpm"
#include <lcom/lcf.h>

int draw_sprite(Sprite *sprite);
int draw_sprite_pos(Sprite *sprite, int x, int y);
int print_background_game(xpm_map_t xpm);
void draw_frame();
int draw_digits(int number, int x_pos, int y_pos, int offset);
void draw_leaderboard();
#endif
