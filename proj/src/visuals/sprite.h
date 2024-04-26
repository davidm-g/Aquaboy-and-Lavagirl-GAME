#include <lcom/lcf.h>
typedef struct {
    uint16_t x,y;
    uint16_t width,height;
    int16_t xpeed, yspeed;
    uint32_t *map;
} Sprite;   

Sprite *create_sprite(uint32_t *map, uint16_t x, uint16_t y,int16_t xpeed, int16_t yspeed);
void destroy_sprite(Sprite *sp);

typedef struct {
    Sprite *sp; // standard sprite
    int aspeed; // no. frames per pixmap
    int cur_aspeed; // no. frames left to next change
    uint8_t num_fig; // number of pixmaps
    int cur_fig; // current pixmap
    uint32_t **map; // array of pointers to pixmaps
} AnimSprite;

create_animSprite(uint8_t no_pic, uint32_t *pic1[], ...);
int animate_animSprite(AnimSprite *sp,uint32_t* base);
void destroy_animSprite(AnimSprite *sp);