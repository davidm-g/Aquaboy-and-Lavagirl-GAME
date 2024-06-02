
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

uint16_t get_posx(Sprite *sp);
uint16_t get_posy(Sprite *sp);
void set_posx(Sprite *sp, uint16_t x);
void set_posy(Sprite *sp, uint16_t y);
#endif
