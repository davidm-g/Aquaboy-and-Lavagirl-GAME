#include "sprite.h"

uint16_t get_posx(Sprite *sp) {
  return sp->x;
}

uint16_t get_posy(Sprite *sp) {
  return sp->y;
}

void set_posx(Sprite *sp, uint16_t x) {
  sp->x = x;
}

void set_posy(Sprite *sp, uint16_t y) {
  sp->y = y;
}

Sprite *create_sprite(xpm_map_t map, uint16_t x, uint16_t y, int16_t xspeed, int16_t yspeed) {
  xpm_image_t img;
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  sp->isOnGround = false;
  sp->xaccell = 0;
  sp->map = (uint32_t *) xpm_load(map, XPM_8_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }
  sp->width = img.width;
  sp->height = img.height;
  return sp;
}
void destroy_sprite(Sprite *sp) {
  if (sp == NULL) {
    return;
  }
  if (sp->map) {
    free(sp->map);
  }
  free(sp);
  sp = NULL;
  return;
}

AnimSprite *create_animSprite(uint8_t no_pic, xpm_map_t pic1, ...) {
  AnimSprite *asp = malloc(sizeof(AnimSprite));
  // create a standard sprite with first pixmap
  asp->sp = create_sprite(pic1, 0, 0, 0, 0);
  // allocate array of pointers to pixmaps
  asp->map = malloc((no_pic) * sizeof(char *));
  // initialize the first pixmap
  asp->map[0] = asp->sp->map;
  // continues in next transparency

  // initialize the remainder with the variable arguments
  // iterate over the list of arguments
  va_list ap;
  va_start(ap, pic1);
  for (int i = 1; i < no_pic; i++) {
    xpm_map_t tmp = va_arg(ap, xpm_map_t);
    xpm_image_t img;
    asp->map[i] = (uint32_t *) xpm_load(tmp, XPM_8_8_8_8, &img);
    if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height) { // failure: release allocated memory
      for (int j = 1; j < i; j++)
        free(asp->map[i]);
      free(asp->map);
      destroy_sprite(asp->sp);
      free(asp);
      va_end(ap);
      return NULL;
    }
  }
  va_end(ap);
  asp->num_fig = no_pic;
  return asp;
}

void destroy_animSprite(AnimSprite *asp) {
  for (int i = 0; i < asp->num_fig; i++) {
    free(asp->map[i]);
  }
  free(asp->map);
  destroy_sprite(asp->sp);
  free(asp);
  asp = NULL;
  return;
}
