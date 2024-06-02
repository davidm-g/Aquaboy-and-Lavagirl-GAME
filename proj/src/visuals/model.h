#ifndef __MODEL_H
#define __MODEL_H
#include "../devices/graphics/vbe.h"
#include "../draws/draw.h"
#include "defines.h"
#include "devices/graphics/video.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/timer/i8254.h"
#include "devices/timer/timer.h"
#include "devices/utils.h"
#include "sprite.h"
#include "visuals/xpms/arrow.xpm"
#include "visuals/xpms/bar.xpm"
#include "visuals/xpms/boy.xpm"
#include "visuals/xpms/boywalk1.xpm"
#include "visuals/xpms/boywalk1reverse.xpm"
#include "visuals/xpms/boywalk2.xpm"
#include "visuals/xpms/boywalk2reverse.xpm"
#include "visuals/xpms/boywin.xpm"
#include "visuals/xpms/buttonleft.xpm"
#include "visuals/xpms/buttonleftpressed.xpm"
#include "visuals/xpms/buttonright.xpm"
#include "visuals/xpms/buttonrightpressed.xpm"
#include "visuals/xpms/centerFire.xpm"
#include "visuals/xpms/centerToxic.xpm"
#include "visuals/xpms/centerWater.xpm"
#include "visuals/xpms/doorblue.xpm"
#include "visuals/xpms/doorred.xpm"
#include "visuals/xpms/dots.xpm"
#include "visuals/xpms/exit_button.xpm"
#include "visuals/xpms/girl.xpm"
#include "visuals/xpms/girlwalk1.xpm"
#include "visuals/xpms/girlwalk1reverse.xpm"
#include "visuals/xpms/girlwalk2.xpm"
#include "visuals/xpms/girlwalk2reverse.xpm"
#include "visuals/xpms/girlwin.xpm"
#include "visuals/xpms/greenleverright.xpm"
#include "visuals/xpms/hand.xpm"
#include "visuals/xpms/leaderboard_button.xpm"
#include "visuals/xpms/leftFire.xpm"
#include "visuals/xpms/leftToxic.xpm"
#include "visuals/xpms/leftWater.xpm"
#include "visuals/xpms/num_0.xpm"
#include "visuals/xpms/num_1.xpm"
#include "visuals/xpms/num_2.xpm"
#include "visuals/xpms/num_3.xpm"
#include "visuals/xpms/num_4.xpm"
#include "visuals/xpms/num_5.xpm"
#include "visuals/xpms/num_6.xpm"
#include "visuals/xpms/num_7.xpm"
#include "visuals/xpms/num_8.xpm"
#include "visuals/xpms/num_9.xpm"
#include "visuals/xpms/opendoor.xpm"
#include "visuals/xpms/redleverleft.xpm"
#include "visuals/xpms/rightFire.xpm"
#include "visuals/xpms/rightToxic.xpm"
#include "visuals/xpms/rightWater.xpm"
#include "visuals/xpms/start.xpm"
#include "visuals/xpms/tryagain.xpm"
#include "visuals/xpms/wall.xpm"
#include "visuals/xpms/wall2.xpm"
#include "visuals/xpms/wall20_20.xpm"
#include <lcom/lcf.h>
typedef enum {
  NORMAL,
  WALKLEFT,
  WALKRIGHT,
  WINNING
} SpriteState;

typedef enum {
  RUNNING,
  EXIT
} SystemState;

typedef enum {
  START,
  GAME,
  LEADERBOARD,
  GAMEOVER,
  WIN,
  END
} MenuState;

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  int score;
} LeaderboardEntry;

void(timer_int_handler)();
void load_sprites();
void updateArrayWithLevel(int level);
Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y);
void update_timer();
void update_keyboard();
void update_mouse();
void destroy_sprites();
void move_cursor(struct packet *pp);
void action_handler(Sprite *sp);
int update(Sprite *sp);
void updateSpeed(Sprite *sp);
int updatePosition(Sprite *sp);
int move_x(Sprite *sp);
int move_y(Sprite *sp);
void set_ground(Sprite *sp);
void jump(Sprite *sp);
void gravity(Sprite *sp);
void left(Sprite *sp);
void right(Sprite *sp);
void check_mouse_click(struct packet pp);
void update_rtc();
void reset_states();
void add_to_leaderboard(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, int score);
void initialize_leaderboard();
void write_leaderboard_data();
void read_leaderboard_data();
#endif
