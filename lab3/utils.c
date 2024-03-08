#include <lcom/lcf.h>
#include <stdint.h>

int sys_inb_counter = 0;

int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }
  uint32_t val = 0;
  int ret = sys_inb(port, &val);
  *value = val & 0xFF;

#ifdef LAB3
  sys_inb_counter++;
#endif

  return ret;
}
