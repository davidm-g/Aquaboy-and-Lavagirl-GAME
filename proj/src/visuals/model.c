#include "model.h"
int global_counter = 0;
Sprite *lavaboy;
Sprite *cursor;
Sprite *wall;
Sprite *wall2;
void load_sprites() {
    lavaboy = create_sprite((xpm_map_t) LAVABOY_xpm, 300, 300, 0, 0);
    cursor = create_sprite((xpm_map_t) hand_xpm, 0, 0, 0, 0);
    wall = create_sprite((xpm_map_t) wall_xpm, 100, 500, 0, 0);
    wall2 = create_sprite((xpm_map_t) wall2_xpm, 350, 500, 0, 0);
}
void(timer_int_handler)() {
  global_counter++;
}

