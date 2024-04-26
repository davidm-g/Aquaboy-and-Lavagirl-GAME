#include <lcom/lcf.h>
#include <stdarg.h> // va_* macros are defined here
#include "sprite.h"
Sprite (*create_sprite)(uint32_t *map, uint16_t x, uint16_t y,int16_t xpeed, int16_t yspeed){
    xpm_image_t img;
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
    if(sp == NULL)
        return NULL;
    sp->map = map;
    sp->x = x;
    sp->y = y;
    sp->xpeed = xpeed;
    sp->yspeed = yspeed;
    sp->map= (uint32_t *)(map, XPM_8_8_8_8, &img);
    if(sp->map ==NULL){
        free(sp);
        return NULL;
    }
    sp->width=img.width;
    sp->heigth=img.height;
    return sp;
}
void destroy_sprite(Sprite *sp){
    if(sp==NULL){
        return;
    }
    if(sp->map){
        free(sp->map);
    }
    free(sp);
    sp=NULL;
    return;
}

AnimSprite (*create_animSprite)(uint8_t no_pic,uint32_t *pic1[], ...) {
    AnimSprite *asp = malloc(sizeof(AnimSprite));
    // create a standard sprite with first pixmap
    asp->sp = create_sprite(pic1,0,0,0,0);
    // allocate array of pointers to pixmaps
    asp->map = malloc((no_pic) * sizeof(char *));
    // initialize the first pixmap
    asp->map[0] = asp->sp->map;
    // continues in next transparency

    // initialize the remainder with the variable arguments
    // iterate over the list of arguments
    va_list ap; 
    va_start(ap, pic1);
    for( int i = 1; i < no_pic; i++ ) {
        uint32_t **tmp = va_arg(ap, uint32_t **);
        xpm_image_t img;
        asp->map[i] =(uint32_t*) xpm_load(tmp, XPM_8_8_8_8, &img);
        if( asp->map[i] == NULL
        || img.width != asp->sp->width || img.height != asp->sp->heigth){// failure: release allocated memory
            for(j = 1; j<i;j ++)
                free(asp->map[i]);
            free(asp->map);
            destroy_sprite(asp->sp);
            free(asp);
            va_end(ap);
            return NULL;
        }
    }
    va_end(ap);
    asp->num_fig = no_pic;

}

void destroy_animSprite(AnimSprite *asp){
    for(int i=0;i<asp->num_fig;i++){
        free(asp->map[i]);
    }
    free(asp->map);
    destroy_sprite(asp->sp);
    free(asp);
    asp=NULL;
    return;
}