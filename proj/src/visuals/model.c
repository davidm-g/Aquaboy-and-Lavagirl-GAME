#include "model.h"
#include <lcom/lcf.h>
int global_counter = 0;

void(timer_int_handler)() {
  global_counter++;
}
