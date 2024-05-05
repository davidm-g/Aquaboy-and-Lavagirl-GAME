#include "model.h"
int global_counter = 0;
uint8_t kbd_outbuf;
bool change = false;
Sprite *lavaboy;
Sprite *cursor;
Sprite *walls[2];
SystemState systemState = RUNNING;
extern int16_t mouse_x;
extern int16_t mouse_y;
extern int byte_counter;
void(timer_int_handler)() {
  global_counter++;
}

void load_sprites() {
  lavaboy = create_sprite((xpm_map_t) LAVABOY_xpm, 300, 300, 0, 0);
  cursor = create_sprite((xpm_map_t) hand_xpm, 0, 0, 0, 0);
  walls[0] = (create_sprite((xpm_map_t) wall_xpm, 100, 500, 0, 0));
  walls[1] = (create_sprite((xpm_map_t) wall2_xpm, 350, 500, 0, 0));
}

void destroy_sprites(){
  destroy_sprite(lavaboy);
  destroy_sprite(cursor);
  for(int i = 0; i < 2; i++) {
    destroy_sprite(walls[i]);
  }
}
int checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  for(int i = 0; i < 2; i++)
      if(sp == walls[i]) return 0;
    for(int i = 0; i < 2; i++){
      if(!(x >= (walls[i]->x + walls[i]->width) || // boneco à direita da parede
        (y + sp->height) <= walls[i]->y || // boneco por cima da parede
        (x + sp->width) <= walls[i]->x || // boneco à esquerda da parede
        y >= (walls[i]->y + walls[i]->height))) // boneco por baixo da parede
        return 1;
    }
  return 0;
}
void update_timer(){
  timer_int_handler();
  if (change) {
    flip_screen();
    change = false;
  }
}

void update_keyboard(){
  kbc_ih();
  int16_t new_x, new_y;
  if(kbd_outbuf == ESC_BREAK){
    systemState = EXIT;
  }
  else if (kbd_outbuf == W_KEY) {
    new_x = get_posx(lavaboy);
    new_y = get_posy(lavaboy) - 10;
    if (checkCollision(lavaboy, new_x, new_y) == 0 && (new_y >= 0)) {
      set_posy(lavaboy, new_y);
      change = true;
    }
    
  }
  else if (kbd_outbuf == A_KEY) {
    new_x = get_posx(lavaboy) - 10;
    new_y = get_posy(lavaboy);
    if (checkCollision(lavaboy, new_x, new_y) == 0 && (new_x >= 0)) {
      set_posx(lavaboy, new_x);
      change = true;
    }
  }
  else if (kbd_outbuf == S_KEY) {
    new_x = get_posx(lavaboy);
    new_y = get_posy(lavaboy) + 10;
    if (checkCollision(lavaboy, new_x, new_y) == 0 && ((new_y + lavaboy->height) <= get_vres())) {
      set_posy(lavaboy, new_y);
      change = true;
    }
  }
  else if (kbd_outbuf == D_KEY) {
    new_x = get_posx(lavaboy) + 10;
    new_y = get_posy(lavaboy);
    if (checkCollision(lavaboy, new_x, new_y) == 0 && ((new_x + lavaboy->width) <= get_hres())) {
      set_posx(lavaboy, new_x);
      change = true;
    }
  }
  draw_frame();
}

void update_mouse(){
  mouse_ih();
  bytes_to_packet();
  if(byte_counter==3){
    packet_parse();
    byte_counter=0;
    change=true;
    draw_frame();
  }
}

