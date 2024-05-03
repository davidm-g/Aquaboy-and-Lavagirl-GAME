#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int set_graphic_mode(uint16_t mode);
int set_text_mode();
void change_screen_vertical_retrace(int h);
void change_screen_artifacts(int h);
