#include "draw.h"
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

uint32_t *background_map_game = NULL;
xpm_image_t background_img;
uint32_t *background_map_menu = NULL;
xpm_image_t background_img_menu;
uint32_t *background_map_leaderboard = NULL;
xpm_image_t background_img_leaderboard;
extern Sprite *lavaboy;
extern Sprite *cursor;
extern Sprite *walls[2];
extern Sprite *start;
extern Sprite *exit_button;
extern int16_t mouse_x;
extern int16_t mouse_y;
extern MenuState menuState;
int draw_sprite(Sprite *sprite) {
  uint32_t *original_map = sprite->map;
  for (uint16_t i = 0; i < sprite->height; i++) {
    for (uint16_t j = 0; j < sprite->width; j++) {
      if ((*(sprite->map)) == 1) {
        sprite->map++;
        continue;
      }
      if (vg_draw_pixel(sprite->x + j, sprite->y + i, *(sprite->map)) != 0)
        return 1;
      sprite->map++;
    }
  }
  sprite->map = original_map;
  return 0;
}
/*
int erase_sprite(Sprite *sprite, xpm_map_t xpm) {
  static uint32_t *map;
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
      if (vg_draw_pixel(sprite->x + j, sprite->y + i, *map) != 0)
        return 1;
      map++;
    }
    map += get_hres() - sprite->width;
  }
  return 0;
}
*/
int print_background_game(xpm_map_t xpm) {
  if (background_map_game == NULL) {
    background_map_game = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &background_img);
    if (background_map_game == NULL) {
      printf("Error loading background\n");
      return 1;
    }
  }
  uint32_t *original_map = background_map_game;
  for (uint16_t i = 0; i < background_img.height; i++) {
    for (uint16_t j = 0; j < background_img.width; j++) {
      if (vg_draw_pixel(j, i, *background_map_game) != 0) {
        printf("Error drawing pixel\n");
        return 1;
      }
      background_map_game++;
    }
  }
  background_map_game = original_map;
  return 0;
}
int print_background_menu(xpm_map_t xpm) {
  if (background_map_menu == NULL) {
    background_map_menu = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &background_img_menu);
    if (background_map_menu == NULL) {
      printf("Error loading background\n");
      return 1;
    }
  }
  uint32_t *original_map = background_map_menu;
  for (uint16_t i = 0; i < background_img_menu.height; i++) {
    for (uint16_t j = 0; j < background_img_menu.width; j++) {
      if (vg_draw_pixel(j, i, *background_map_menu) != 0) {
        printf("Error drawing pixel\n");
        return 1;
      }
      background_map_menu++;
    }
  }
  background_map_menu = original_map;
  return 0;
}
int print_background_leaderboard(xpm_map_t xpm) {
  if (background_map_leaderboard == NULL) {
    background_map_leaderboard = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &background_img_leaderboard);
    if (background_map_leaderboard == NULL) {
      printf("Error loading background\n");
      return 1;
    }
  }
  uint32_t *original_map = background_map_leaderboard;
  for (uint16_t i = 0; i < background_img_leaderboard.height; i++) {
    for (uint16_t j = 0; j < background_img_leaderboard.width; j++) {
      if (vg_draw_pixel(j, i, *background_map_leaderboard) != 0) {
        printf("Error drawing pixel\n");
        return 1;
      }
      background_map_leaderboard++;
    }
  }
  background_map_leaderboard = original_map;
  return 0;
}
void draw_temp_background(){
  for (uint16_t i = 0; i < get_vres(); i++) {
    for (uint16_t j = 0; j < get_hres(); j++) {
      if (vg_draw_pixel(j, i, 0x5e2d18) != 0) {
        printf("Error drawing pixel\n");
        return ;
      }
    }
  }
}
void draw_frame() {
  //draw_temp_background();
  switch (menuState)
  {
  case START:
    print_background_menu((xpm_map_t) Menu_xpm);
    draw_sprite(start);
    draw_sprite(exit_button);
    break;
  case GAME:
      print_background_game((xpm_map_t) background_xpm);
    draw_sprite(lavaboy);
    for (int i = 0; i < 2; i++) {
      draw_sprite(walls[i]);
    }
    
    break;
  default:
    break;
  }
  draw_sprite(cursor);
}

