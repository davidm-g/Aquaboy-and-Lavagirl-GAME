#include <lcom/lcf.h>
#include "model.h"
int global_counter = 0;

void (timer_int_handler)() {
  buffer_copy();    
  global_counter++;
}
