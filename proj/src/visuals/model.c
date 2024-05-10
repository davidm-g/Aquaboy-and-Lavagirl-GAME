#include "model.h"
int global_counter = 0;
uint8_t kbd_outbuf;
bool change = false;
Sprite *lavaboy;
Sprite *cursor;
Sprite *walls[2];
SystemState systemState = RUNNING;
extern struct packet packet;
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

void destroy_sprites() {
  destroy_sprite(lavaboy);
  destroy_sprite(cursor);
  for (int i = 0; i < 2; i++) {
    destroy_sprite(walls[i]);
  }
}
int checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  for (int i = 0; i < 2; i++) {
    if (!(x >= (walls[i]->x + walls[i]->width) || // boneco à direita da parede
          (y + sp->height) <= walls[i]->y ||      // boneco por cima da parede
          (x + sp->width) <= walls[i]->x ||       // boneco à esquerda da parede
          y >= (walls[i]->y + walls[i]->height))) // boneco por baixo da parede
      return 1;
  }
  return 0;
}
void update_timer() {
  timer_int_handler();
  if (change) {
    draw_frame();
    flip_screen();
    change = false;
  }
}

void update_keyboard() {
  kbc_ih();
  if (kbd_outbuf == ESC_BREAK)
    systemState = EXIT;
  if (move(lavaboy) == 0)
    change = true;
}

void update_mouse() {
  mouse_ih();
  bytes_to_packet();
  if (byte_counter == 3) {
    packet_parse();
    byte_counter = 0;
    change = true;
    move_cursor(&packet);
  }
}

int move(Sprite *sp) {
  uint16_t x = sp->x;
  uint16_t y = sp->y;

  if (kbd_outbuf == W_KEY) {
    if (y > 10) {
      y -= 10;
    }
  }
  else if (kbd_outbuf == S_KEY) {
    if (y + sp->height + 10 < get_vres()) {
      y += 10;
    }
  }
  else if (kbd_outbuf == A_KEY) {
    if (x > 10) {
      x -= 10;
    }
  }
  else if (kbd_outbuf == D_KEY) {
    if (x + sp->width + 10 < get_hres()) {
      x += 10;
    }
  }
  if (checkCollision(sp, x, y) == 0) {
    sp->x = x;
    sp->y = y;
    return 0;
  }
  return 1;
}

void move_cursor(struct packet *pp) {
  int16_t x = cursor->x + pp->delta_x;
  int16_t y = cursor->y - pp->delta_y;
  if (x < 0)
    x = 0;
  else if (x > CURSOR_MAX_X)
    x = CURSOR_MAX_X;
  if (y < 0)
    y = 0;
  else if (y > CURSOR_MAX_Y)
    y = CURSOR_MAX_Y;
  cursor->x = x;
  cursor->y = y;
}
