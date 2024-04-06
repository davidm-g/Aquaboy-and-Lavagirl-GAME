#include <stdint.h>
#include <stdio.h>

void vg_draw_pixel(uint32_t *ptr, uint32_t color);
uint16_t get_hres();
uint16_t get_vres();
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void map_phys_virt(uint16_t mode);
