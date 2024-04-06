#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "vbe.h"
static uint16_t hres;
static uint16_t vres;
static void *video_mem;
static uint8_t bits_per_pixel;
uint16_t get_hres(){
  return hres;
}
uint16_t get_vres(){
  return vres;
}
void map_phys_virt(uint16_t mode){
  vbe_mode_info_t vmi_p;
  if(vbe_get_mode_info(mode, &vmi_p) != 0) panic("couldn't get vbe mode info"); 
  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  
  struct minix_mem_range mr;
  unsigned int vram_base = vmi_p.PhysBasePtr; 
  unsigned int vram_size = hres * vres * bits_per_pixel;
  int r;
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size; 
  
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");
}

void vg_draw_pixel(uint32_t *ptr, uint32_t color) {
    int i;
    size_t n = (size_t) (bits_per_pixel/8);
    for(i = 0; i< hres*vres; i++){
        memcpy(ptr, &color, n);
    }
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  size_t n = (size_t) (bits_per_pixel / 8);
  uint32_t *ptr;
  ptr = video_mem;
  ptr += hres*y*n + x*n;
  for (uint16_t i = 0; i < len; i++){
    vg_draw_pixel(ptr, color);
    ptr+=n;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(uint16_t i = 0; i < height; i++)
        if(vg_draw_hline(x, y+i, width, color) != 0) return 1;

    return 0;
}
