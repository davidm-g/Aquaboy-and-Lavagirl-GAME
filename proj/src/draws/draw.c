#include "draw.h"
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
extern int level_time;
extern int global_counter;
extern int changesprite;
extern int spriteindex;
uint32_t *background_map_game = NULL;
xpm_image_t background_img;
uint32_t *background_map_menu = NULL;
xpm_image_t background_img_menu;
uint32_t *background_map_leaderboard = NULL;
xpm_image_t background_img_leaderboard;
extern Sprite *boy;
extern SpriteState boyState;
extern Sprite *boys[6];
extern SpriteState girlState;
extern Sprite *girls[6];
extern Sprite *cursor;
extern Sprite *start;
extern Sprite *exit_button;
extern Sprite *leaderboard_button;
extern Sprite *doorblue;
extern Sprite *doorred;
extern Sprite *greenleverright;
extern Sprite *redleverleft;
extern Sprite *rightToxic;
extern Sprite *centerToxic;
extern Sprite *leftToxic;
extern Sprite *leftFire;
extern Sprite *centerFire;
extern Sprite *rightFire;
extern Sprite *leftWater;
extern Sprite *centerWater;
extern Sprite *rightWater;
extern Sprite *opendoor;
extern Sprite *walls20[1200];
extern Sprite *num[10];
extern int *levelArray;

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

int draw_sprite_pos(Sprite *sprite, int x, int y) {
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
void draw_time(){
  int time = level_time;
  int offset = 50;
  int num_digits = 0;
  if(time==0){
    return;
  }
  for(int t = time; t > 0; t /= 10){
    num_digits++;
  }
  for (int i = 0; i < num_digits; i++) {
        int digit = time % 10;
        draw_sprite_pos(num[digit], 680 + (num_digits - i - 1) * offset, 20);
        time /= 10;
    }
}
void draw_frame() {
  //draw_temp_background();
  switch (menuState)
  {
  case START:
  level_time = 0;
    print_background_menu((xpm_map_t) Menu_xpm);
    draw_sprite(start);
    draw_sprite(exit_button);
    draw_sprite(leaderboard_button);
    break;
  case GAME:
    print_background_game((xpm_map_t) background_xpm);
    
    int i, x, y;
    /*
    for (int i = 0; i < 2; i++) {
      draw_sprite(walls[i]);
    }
    */
    for (i = 0; i < 1200; i++) {
      x = (i % 40) * 20;
      y = (i / 40) * 20;
      if (levelArray[i] == 1) {
        draw_sprite(walls20[i]);
      }
      else if (levelArray[i] == 2) {
        draw_sprite_pos(greenleverright, x, y);
      }
      else if (levelArray[i] == 3) {
        if ((boys[0]->x >= x && boys[0]->x <= x + doorblue->width) && 
          (boys[0]->y >= y && boys[0]->y <= y + doorblue->height)) {
          boyState = WINNING;
          draw_sprite_pos(opendoor, x, y);
        }
        else
          draw_sprite_pos(doorblue, x, y);
      }
      else if (levelArray[i] == 4) {
        if ((girls[0]->x >= x && girls[0]->x <= x + doorred->width) && 
          (girls[0]->y >= y && girls[0]->y <= y + doorred->height)) {
          girlState = WINNING;
          draw_sprite_pos(opendoor, x, y);
        }
        else
          draw_sprite_pos(doorred, x, y);
      }
      else if (levelArray[i] == 5) {
        draw_sprite_pos(redleverleft, x, y);
      }
      else if (levelArray[i] == 6) {
        draw_sprite_pos(leftToxic, x, y);
      }
      else if (levelArray[i] == 7) {
        draw_sprite_pos(centerToxic, x, y);
      }
      else if (levelArray[i] == 8) {
        draw_sprite_pos(rightToxic, x, y);
      }
      else if (levelArray[i] == 9) {
        draw_sprite_pos(leftFire, x, y);
      }
      else if (levelArray[i] == 10) {
        draw_sprite_pos(centerFire, x, y);
      }
      else if (levelArray[i] == 11) {
        draw_sprite_pos(rightFire, x, y);
      }
      else if (levelArray[i] == 12) {
        draw_sprite_pos(leftWater, x, y);
      }
      else if (levelArray[i] == 13) {
        draw_sprite_pos(centerWater, x, y);
      }
      else if (levelArray[i] == 14) {
        draw_sprite_pos(rightWater, x, y);
      }
    }
    // Girl Draw Section
    if (girlState == NORMAL)
      draw_sprite(girls[0]);
    else {
      if (global_counter - changesprite >= 10) {
        changesprite = global_counter;
        if (spriteindex == 0) spriteindex = 2;
        else spriteindex = 0;
      }
      if (girlState == WALKRIGHT)
        draw_sprite_pos(girls[1 + spriteindex], girls[0]->x, girls[0]->y);
      else if (girlState == WALKLEFT)
        draw_sprite_pos(girls[2 + spriteindex], girls[0]->x, girls[0]->y);
      else if (girlState == WINNING)
        draw_sprite_pos(girls[5], girls[0]->x, girls[0]->y);
    }
    // Boy Draw Section
    if (boyState == NORMAL)
      draw_sprite(boys[0]);
    else {
      if (global_counter - changesprite >= 10) {
        changesprite = global_counter;
        if (spriteindex == 0) spriteindex = 2;
        else spriteindex = 0;
      }
      if (boyState == WALKRIGHT)
        draw_sprite_pos(boys[1 + spriteindex], boys[0]->x, boys[0]->y);
      else if (boyState == WALKLEFT)
        draw_sprite_pos(boys[2 + spriteindex], boys[0]->x, boys[0]->y);
      else if (boyState == WINNING)
        draw_sprite_pos(boys[5], boys[0]->x, boys[0]->y);
    }
    draw_time();
    break;
  case LEADERBOARD:
    print_background_leaderboard((xpm_map_t) leaderboard_xpm);
    break;
  default:
    break;
  }
  draw_sprite(cursor);
}
