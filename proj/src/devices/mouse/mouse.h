#include "i8042.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int send_cmd_mouse(uint8_t cmd);
void bytes_to_packet();
void packet_parse();
void(mouse_ih)();
int can_read_outbuf_mouse();
int change_sample_rate(uint8_t rate);
