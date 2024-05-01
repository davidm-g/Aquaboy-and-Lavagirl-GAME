#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#define DIRECT_COLOR 0x06

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
uint16_t get_hres();
uint16_t get_vres();
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void map_phys_virt(uint16_t mode);
uint32_t first_red(uint32_t first);
uint32_t first_green(uint32_t first);
uint32_t first_blue(uint32_t first);
uint32_t index_indexed_mode(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step);
uint32_t direct_mode(uint32_t red, uint32_t green, uint32_t blue);
uint32_t red_value(uint32_t first_red, uint16_t col_num, uint8_t step);
uint32_t green_value(uint32_t first_green, uint16_t row_num, uint8_t step);
uint32_t blue_value(uint32_t first_blue, uint16_t row_num, uint16_t col_num, uint8_t step);
int print_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);
void buffer_copy();
