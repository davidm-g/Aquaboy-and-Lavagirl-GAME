
#ifndef SPRITE_H
#define SPRITE_H
#include <lcom/lcf.h>
#include <stdarg.h> // va_* macros are defined here

typedef struct {
  uint16_t x, y;
  uint16_t width, height;
  int16_t xspeed, yspeed, xaccell;
  bool isOnGround;
  uint32_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t map, uint16_t x, uint16_t y, int16_t xspeed, int16_t yspeed);
void destroy_sprite(Sprite *sp);

typedef struct {
  Sprite *sp;      // standard sprite
  int aspeed;      // no. frames per pixmap
  int cur_aspeed;  // no. frames left to next change
  uint8_t num_fig; // number of pixmaps
  int cur_fig;     // current pixmap
  uint32_t **map;  // array of pointers to pixmaps
} AnimSprite;

AnimSprite *create_animSprite(uint8_t no_pic, xpm_map_t pic1, ...);
int animate_animSprite(AnimSprite *sp, uint32_t *base);
void destroy_animSprite(AnimSprite *sp);

uint16_t get_posx(Sprite *sp);
uint16_t get_posy(Sprite *sp);
void set_posx(Sprite *sp, uint16_t x);
void set_posy(Sprite *sp, uint16_t y);
#endif
