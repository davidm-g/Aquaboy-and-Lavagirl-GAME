#include "draw.h"
#include "visuals/xpms/LAVABOY.xpm"
#include "visuals/xpms/LAVABOY2.xpm"
#include "visuals/xpms/hand.xpm"
#include "visuals/xpms/wall.xpm"
#include "visuals/xpms/wall2.xpm"
#include <lcom/lcf.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

static uint32_t *background_map = NULL;
Sprite *lavaboy;
Sprite *cursor;
Sprite *walls[2];

void load_sprites() {
  lavaboy = create_sprite((xpm_map_t) LAVABOY_xpm, 300, 300, 0, 0);
  cursor = create_sprite((xpm_map_t) hand_xpm, 0, 0, 0, 0);
  walls[0] = (create_sprite((xpm_map_t) wall_xpm, 100, 500, 0, 0));
  walls[1] = (create_sprite((xpm_map_t) wall2_xpm, 350, 500, 0, 0));
}

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
  static uint32_t *map;
  uint32_t *original_map = sprite->map;
  if (background_map != NULL) {
    map = background_map;
  }
  else {
    xpm_image_t background_img;
    map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &background_img);
  }
  map += sprite->y * get_hres() + sprite->x;
  for (uint16_t i = 0; i < sprite->height; i++) {
    for (uint16_t j = 0; j < sprite->width; j++) {
      if ((*(sprite->map)) == 1) {
        sprite->map++;
        map++;
        continue;
      }
      if (vg_draw_pixel(sprite->x + j, sprite->y + i, *map) != 0)
        return 1;
      map++;
      sprite->map++;
    }
    map += get_hres() - sprite->width;
  }
  sprite->map = original_map;
  return 0;
}

int checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  // Broad Phase
  for (int i = 0; i < 2; i++) {
    if (x > (walls[i]->x + walls[i]->width) || // boneco à direita da parede
        (y + sp->height) < walls[i]->y ||      // boneco por cima da parede
        (x + sp->width) < walls[i]->x ||       // boneco à esquerda da parede
        y > (walls[i]->y + walls[i]->height))  // boneco por baixo da parede
      return 0;

    // Calculate intersecting rectangle
    uint16_t left = max(x, walls[i]->x);
    uint16_t right = min(x + sp->width, walls[i]->x + walls[i]->width);
    uint16_t top = max(y, walls[i]->y);
    uint16_t bottom = min(y + sp->height, walls[i]->y + walls[i]->height);

    // Narrow Phase
    for (uint16_t i = top; i < bottom; i++) {
      for (uint16_t j = left; j < right; j++) {
        uint16_t color1 = *(sp->map + (i - y) * sp->width + (j - x));
        uint16_t color2 = *(walls[i]->map + (i - walls[i]->y) * walls[i]->width + (j - walls[i]->x));
        if (color1 != 1 && color2 != 1) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int print_background(xpm_map_t xpm) {
  xpm_image_t background_img;
  background_map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &background_img);
  if (background_map == NULL)
    return 1;
  uint32_t *original_map = background_map;
  for (uint16_t i = 0; i < background_img.height; i++) {
    for (uint16_t j = 0; j < background_img.width; j++) {
      if (vg_draw_pixel(j, i, *background_map) != 0)
        return 1;
      background_map++;
    }
  }
  background_map = original_map;
  return 0;
}
