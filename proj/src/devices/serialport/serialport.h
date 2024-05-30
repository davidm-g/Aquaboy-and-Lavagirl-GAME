#include "../utils.h"
#include "UART.h"
#include "queue.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int ser_subscribe_int(uint8_t *bit_no);
int ser_unsubscribe_int();
int ser_get_status(uint8_t *st);
int ser_init();
int ser_ih();
int ser_send_byte(uint8_t byte);
int ser_read_byte();
