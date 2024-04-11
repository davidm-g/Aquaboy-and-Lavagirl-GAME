#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "vbe.h"
static uint16_t hres;
static uint16_t vres;
static void *video_mem;
static uint8_t bits_per_pixel;
vbe_mode_info_t vmi_p;
uint16_t get_hres(){
  return hres;
}
uint16_t get_vres(){
  return vres;
}
void map_phys_virt(uint16_t mode){
  memset(&vmi_p, 0, sizeof(vmi_p));
  if(vbe_get_mode_info(mode, &vmi_p) != 0) panic("couldn't get vbe mode info"); 
  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  
  struct minix_mem_range mr;
  unsigned int vram_base = vmi_p.PhysBasePtr; 
  unsigned int vram_size = hres * vres * (bits_per_pixel/8); //we were putting bits instead of bytes
  int r;
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size; 
  
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    size_t n = (size_t) (bits_per_pixel/8); //number of bytes per pixel
    uint8_t *ptr;
    ptr = video_mem;
    ptr += hres*y*n + x*n;
    if(memcpy(ptr, &color, n)==NULL) return 1;
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for (uint16_t i = 0; i < len; i++){
    if(vg_draw_pixel(x+i,y, color)!=0) return 1;
  }
  return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(uint16_t i = 0; i < height; i++)
        if(vg_draw_hline(x, y+i, width, color) != 0) return 1;
    return 0;
}

uint32_t index_indexed_mode(uint16_t row, uint16_t col,uint8_t no_rectangles, uint32_t first, uint8_t step){
  return (first + (row * no_rectangles + col) * step) % (1 << vmi_p.BitsPerPixel);
}

uint32_t direct_mode(uint32_t red, uint32_t green, uint32_t blue){
  return (red << vmi_p.RedFieldPosition) | (green << vmi_p.GreenFieldPosition) | (blue << vmi_p.BlueFieldPosition);
}
uint32_t first_red(uint32_t color){
  uint32_t redMask = (1 << vmi_p.RedMaskSize) - 1; //100000000-1 = 011111111, if redmasksize = 8
  uint32_t redComponent = color >> vmi_p.RedFieldPosition;
  return redMask & redComponent;
}
uint32_t first_green(uint32_t color){
  uint32_t greenMask = (1 << vmi_p.GreenMaskSize) - 1; 
  uint32_t greenComponent = color >> vmi_p.GreenFieldPosition;
  return greenMask & greenComponent;
}
uint32_t first_blue(uint32_t color){
  uint32_t blueMask = (1 << vmi_p.BlueMaskSize) - 1; 
  uint32_t blueComponent = color >> vmi_p.BlueFieldPosition;
  return blueMask & blueComponent;
}
uint32_t red_value(uint32_t first_red, uint16_t col_num, uint8_t step){
  return (first_red + col_num * step) % (1 << vmi_p.RedMaskSize);
}

uint32_t green_value(uint32_t first_green, uint16_t row_num, uint8_t step){
  return (first_green + row_num * step) % (1 << vmi_p.GreenMaskSize);
}

uint32_t blue_value(uint32_t first_blue, uint16_t row_num, uint16_t col_num, uint8_t step){
  return (first_blue + (row_num+col_num) * step) % (1 << vmi_p.BlueMaskSize);
}
