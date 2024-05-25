#include "model.h"
int global_counter = 0;
int changesprite = 0;
int spriteindex = 0;
uint8_t kbd_outbuf;
bool change = false;
Sprite *boy;
BoyState boyState = NORMAL;
Sprite *boys[6];
/*
Sprite *boywalk1;
Sprite *boywalk1reverse;
Sprite *boywalk2;
Sprite *boywalk2reverse;
Sprite *boywin;
*/
Sprite *cursor;
Sprite *walls[2];
Sprite *start;
Sprite *exit_button;
SystemState systemState = RUNNING;
MenuState menuState = START;
Sprite *doorblue;
Sprite *doorred;
Sprite *walls20[1200];
Sprite *greenleverright;
Sprite *redleverleft;
Sprite *rightToxic;
Sprite *leftToxic;
Sprite *leftFire;
Sprite *rightFire;
Sprite *leftWater;
Sprite *rightWater;
Sprite *centerToxic;
Sprite *centerFire;
Sprite *centerWater;
Sprite *opendoor;
int *levelArray;
LevelState levelState = LEVEL_1;
extern struct packet packet;
extern int byte_counter;
void(timer_int_handler)() {
  global_counter++;
}

void load_sprites() {
  start = create_sprite((xpm_map_t) start_xpm, 200, 100, 0, 0);
  boys[0] = create_sprite((xpm_map_t) boy_xpm, 20, 530, 0, 0);
  boys[1] = create_sprite((xpm_map_t) boywalk1_xpm, 20, 530, 0, 0);
  boys[2] = create_sprite((xpm_map_t) boywalk1reverse_xpm, 20, 530, 0, 0);
  boys[3] = create_sprite((xpm_map_t) boywalk2_xpm, 20, 530, 0, 0);
  boys[4] = create_sprite((xpm_map_t) boywalk2reverse_xpm, 20, 530, 0, 0);
  boys[5] = create_sprite((xpm_map_t) boywin_xpm, 20, 530, 0, 0);
  cursor = create_sprite((xpm_map_t) hand_xpm, 0, 0, 0, 0);
  /*
  walls[0] = (create_sprite((xpm_map_t) wall_xpm, 100, 500, 0, 0));
  walls[1] = (create_sprite((xpm_map_t) wall2_xpm, 350, 500, 0, 0));
  */
  exit_button = create_sprite((xpm_map_t) exit_button_xpm, 200, 250, 0, 0);
  opendoor = create_sprite((xpm_map_t) opendoor_xpm, 100, 100, 0, 0);
  doorblue = create_sprite((xpm_map_t) doorblue_xpm, 500, 100, 0, 0);
  doorred = create_sprite((xpm_map_t) doorred_xpm, 600, 100, 0, 0);
  greenleverright = create_sprite((xpm_map_t) greenleverright_xpm, 100, 100, 0, 0);
  redleverleft = create_sprite((xpm_map_t) redleverleft_xpm, 100, 100, 0, 0);
  rightToxic = create_sprite((xpm_map_t) rightToxic_xpm, 100, 100, 0, 0);
  leftToxic = create_sprite((xpm_map_t) leftToxic_xpm, 100, 100, 0, 0);
  leftFire = create_sprite((xpm_map_t) leftFire_xpm, 100, 100, 0, 0);
  rightFire = create_sprite((xpm_map_t) rightFire_xpm, 100, 100, 0, 0);
  leftWater = create_sprite((xpm_map_t) leftWater_xpm, 100, 100, 0, 0);
  rightWater = create_sprite((xpm_map_t) rightWater_xpm, 100, 100, 0, 0);
  centerToxic = create_sprite((xpm_map_t) centerToxic_xpm, 100, 100, 0, 0);
  centerFire = create_sprite((xpm_map_t) centerFire_xpm, 100, 100, 0, 0);
  centerWater = create_sprite((xpm_map_t) centerWater_xpm, 100, 100, 0, 0);
  int i, x, y;
  for (i = 0; i < 1200; i++) {
      x = (i % 40) * 20;
      y = (i / 40) * 20;
      if (levelArray[i] == 1) {
        walls20[i] = (create_sprite((xpm_map_t) wall20_20_xpm, x, y, 0, 0));
      }
      else {
        walls20[i] = NULL;
      }
  }
}

void destroy_sprites() {
  /*
  for (int i = 0; i < 6; i++)
    destroy_sprite(boys[i]);
  */
  destroy_sprite(boys[0]);
  destroy_sprite(cursor);
  for (int i = 0; i < 1200; i++)
    destroy_sprite(walls20[i]);
}

void updateArrayWithLevel(int level) {
    char filename[50];
    levelArray[0] = 5;
    sprintf(filename, "/home/lcom/labs/proj/src/visuals/levels/level%d.txt", level);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    int i = 0;
    while (!feof(fp) && i < (40 * 30)) {
        fscanf(fp, "%d", &levelArray[i]);
        i++;
    }
    fclose(fp);
}

Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  /*
  for (int i = 0; i < 2; i++) {
    if (!(x >= (walls[i]->x + walls[i]->width) || // boneco à direita da parede
          (y + sp->height) <= walls[i]->y ||      // boneco por cima da parede
          (x + sp->width) <= walls[i]->x ||       // boneco à esquerda da parede
          y >= (walls[i]->y + walls[i]->height))) // boneco por baixo da parede
      return walls[i];
  }
  */

  for (int j = 0; j < 1200; j++) {
    if (levelArray[j] == 1)
      if (!(x >= (walls20[j]->x + walls20[j]->width) || // boneco à direita da parede
            (y + sp->height) <= walls20[j]->y ||      // boneco por cima da parede
            (x + sp->width) <= walls20[j]->x ||       // boneco à esquerda da parede
            y >= (walls20[j]->y + walls20[j]->height))) // boneco por baixo da parede
        return walls20[j];
  }
  return NULL;
}
void update_timer() {
  timer_int_handler();
  if(menuState == GAME){
    /*
    for (int i = 0; i < 6; i++)
      if (update(boys[i]) != 0)
        change = true;
    */
    if (update(boys[0]) != 0)
      change = true;
  }
  if (change) {
    draw_frame();
    flip_screen();
    change = false;
  }
  else if(change == false && menuState == GAME){
    boyState = NORMAL;
    draw_frame();
    flip_screen();
  }
}

void update_keyboard() {
  kbc_ih();
  switch (menuState)
  {
  case GAME:
    if (kbd_outbuf == ESC_BREAK){
    menuState = START;
    change = true;
    }
    /*
    for(int i = 0; i < 6; i++)
      action_handler(boys[i]);
    */
    action_handler(boys[0]);
    break;
  case START:
    if(kbd_outbuf == ESC_BREAK){
      systemState = EXIT;
    }
    break;
  default:
    break;
  }
  
}

void update_mouse() {
  mouse_ih();
  bytes_to_packet();
  if (byte_counter == 3) {
    packet_parse();
    byte_counter = 0;
    change = true;
    move_cursor(&packet);
    check_mouse_click(packet);
  }
}
void update_rtc(){
  if(global_counter % FRAME_RATE==0)
  rtc_update_values();
  
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
    boyState = WALKRIGHT;
    x += sp->xspeed;
    if (x > get_hres() - sp->width - 20)
      x = get_hres() - sp->width - 20;
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
    boyState = WALKLEFT;
    x += sp->xspeed;
    if (x < 20)
      x = 20;
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
  else 
    boyState = NORMAL;
  return 0;
}

int move_y(Sprite *sp) {
  int16_t y = sp->y;

  if (sp->yspeed > 0) { // moving down
    y += sp->yspeed;
    if (y > get_vres() - sp->height - 20)
      y = get_vres() - sp->height - 20;
    Sprite *wall = checkCollision(sp, sp->x, y);
    if (wall == NULL) {
      if (sp->y != y) {
        sp->y = y;
        if (y == get_vres() - sp->height - 20) {
          set_ground(sp);
        }
        return 1;
      }
      set_ground(sp);
    }
    else if (wall->x - (sp->x + sp->width) < 0 || wall->x + wall->width - sp->x > 0) { //hits wall from above
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
    if (y < 20)
      y = 20;
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
      sp->yspeed = 0;
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

void check_mouse_click(struct packet pp){
  switch (menuState)
  {
  case START:
    if(pp.lb){
        if(cursor->x>=start->x && cursor->x<=start->x+start->width && cursor->y>=start->y && cursor->y<=start->y+start->height){
          menuState = GAME;
        }
        if(cursor->x>=exit_button->x && cursor->x<=exit_button->x+exit_button->width && cursor->y>=exit_button->y && cursor->y<=exit_button->y+exit_button->height){
          systemState = EXIT;
        }
    }
    break;
  
  default:
    break;
  }
}

