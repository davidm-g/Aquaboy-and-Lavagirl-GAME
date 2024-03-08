/**

*/

#include <stdbool.h>
#include <stdint.h>

void(kbc_ih)();

int(keyboard_subscribe_int)(uint8_t *bit_no);

int(keyboard_unsubscribe_int)();

int can_read_outbuf();
