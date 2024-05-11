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

Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  for (int i = 0; i < 2; i++) {
    if (!(x >= (walls[i]->x + walls[i]->width) || // boneco à direita da parede
          (y + sp->height) <= walls[i]->y ||      // boneco por cima da parede
          (x + sp->width) <= walls[i]->x ||       // boneco à esquerda da parede
          y >= (walls[i]->y + walls[i]->height))) // boneco por baixo da parede
      return walls[i];
  }
  return NULL;
}
void update_timer() {
  timer_int_handler();
  if (update(lavaboy) != 0)
    change = true;
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
  action_handler(lavaboy);
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

void action_handler(Sprite *sp) {
  if (kbd_outbuf == W_MAKE)
    jump(sp);
  else if (kbd_outbuf == A_MAKE || kbd_outbuf == D_BREAK)
    left(sp);
  else if (kbd_outbuf == D_MAKE || kbd_outbuf == A_BREAK)
    right(sp);
}

int update(Sprite *sp) {
  updateSpeed(sp);
  return updatePosition(sp);
}

void updateSpeed(Sprite *sp) {
  if (sp->xaccell != 0) {
    if (sp->xspeed + sp->xaccell >= MAX_SPEED)
      sp->xspeed = MAX_SPEED;
    else if (sp->xspeed + sp->xaccell <= -MAX_SPEED)
      sp->xspeed = -MAX_SPEED;
    else
      sp->xspeed += sp->xaccell;
  }
  else {
    if (sp->xspeed > 0) {
      sp->xspeed--;
    }
    else if (sp->xspeed < 0) {
      sp->xspeed++;
    }
  }
  gravity(sp);
}

int updatePosition(Sprite *sp) {

  int change = move_y(sp);
  change += move_x(sp);

  return change;
}

int move_x(Sprite *sp) {
  int x = sp->x;

  if (sp->xspeed > 0) { // moving right
    x += sp->xspeed;
    if (x > get_hres() - sp->width)
      x = get_hres() - sp->width;
    Sprite *wall = checkCollision(sp, x, sp->y);
    if (wall == NULL) {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else if (wall->y - (sp->y + sp->height) < -2) {
      x = wall->x - sp->width;
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
  }
  else if (sp->xspeed < 0) { // moving left
    x += sp->xspeed;
    if (x < 0)
      x = 0;
    Sprite *wall = checkCollision(sp, x, sp->y);
    if (wall == NULL) {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else if (wall->y - (sp->y + sp->height) < -2) {
      x = wall->x + wall->width;
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
  }
  return 0;
}

int move_y(Sprite *sp) {
  int16_t y = sp->y;

  if (sp->yspeed > 0) { // moving down
    y += sp->yspeed;
    if (y > get_vres() - sp->height)
      y = get_vres() - sp->height;
    Sprite *wall = checkCollision(sp, sp->x, sp->y);
    if (wall == NULL) {
      if (sp->y != y) {
        sp->y = y;
        if (y == get_vres() - sp->height) {
          set_ground(sp);
        }
        return 1;
      }
      set_ground(sp);
    }
    else if (wall->x - (sp->x + sp->width) < -2 || wall->x + wall->width - sp->x > 2) {
      if (!sp->isOnGround) {
        y = wall->y - sp->height;
        if (sp->y != y) {
          sp->y = y;
          set_ground(sp);
          return 1;
        }
        set_ground(sp);
      }
      sp->yspeed = 0;
    }
    else {
      if (sp->y != y) {
        sp->y = y;
        return 1;
      }
    }
  }
  else if (sp->yspeed < 0) { // moving up
    y += sp->yspeed;
    if (y < 0)
      y = 0;
    Sprite *wall = checkCollision(sp, sp->x, y);
    if (wall == NULL) {
      if (sp->y != y) {
        sp->y = y;
        return 1;
      }
    }
    else {
      y = wall->y + wall->height;
      sp->y = y;
      if (sp->yspeed < -12)
        set_ground(sp);
      return 1;
    }
  }
  return 0;
}

void jump(Sprite *sp) {
  if (sp->isOnGround) {
    sp->yspeed = -15;
    sp->isOnGround = false;
  }
}

void set_ground(Sprite *sp) {
  sp->isOnGround = true;
  sp->yspeed = 0;
}

void right(Sprite *sp) {
  if (sp->xaccell <= 0)
    sp->xaccell += 2;
}

void left(Sprite *sp) {
  if (sp->xaccell >= 0)
    sp->xaccell -= 2;
}

void gravity(Sprite *sp) {
  sp->yspeed++;
}
