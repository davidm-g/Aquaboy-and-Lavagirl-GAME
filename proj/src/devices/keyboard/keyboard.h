#include "../utils.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

void(kbc_ih)();

int(keyboard_subscribe_int)(uint8_t *bit_no);

int(keyboard_unsubscribe_int)();

int(can_read_outbuf)();

int(write_to_kbc)(uint8_t port, uint8_t cmdbyte);

int(read_value_data_from_kbc)(uint8_t port, uint8_t *output);

int(restore_interrupts)();
int wait_esc_key();
