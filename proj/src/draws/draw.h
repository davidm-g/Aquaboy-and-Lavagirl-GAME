#include "../devices/graphics/vbe.h"
#include "../visuals/sprite.h"
#include <lcom/lcf.h>

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);
int erase_sprite(Sprite *sprite, xpm_map_t xpm);
int print_background (xpm_map_t xpm);
