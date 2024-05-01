#include "draw.h"
#include <lcom/lcf.h>

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y) {
  if (x < 0 || y < 0 || x + sprite->width > get_hres() || y + sprite->height > get_vres())
    return 1;
  sprite->x = x;
  sprite->y = y;
  uint32_t *original_map = sprite->map;
  for (uint16_t i = 0; i < sprite->height; i++) {
    for (uint16_t j = 0; j < sprite->width; j++) {
      if ((*(sprite->map)) == 1) {
        sprite->map++;
        continue;
      }
      if (vg_draw_pixel(x + j, y + i, *(sprite->map)) != 0)
        return 1;
      sprite->map++;
    }
  }
  sprite->map = original_map;
  return 0;
}

int erase_sprite(Sprite *sprite, xpm_map_t xpm) {
  xpm_image_t img;
  static uint32_t *map;
  map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
  map += sprite->y * get_hres() + sprite->x;
  for (uint16_t i = 0; i < sprite->height; i++) {
    for (uint16_t j = 0; j < sprite->width; j++) {
      if (vg_draw_pixel(sprite->x + j, sprite->y + i, *map) != 0)
        return 1;
      map++;
    }
    map += get_hres() - sprite->width;
  }
  return 0;
}
