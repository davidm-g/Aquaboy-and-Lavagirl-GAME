#ifndef __RTC_H
#define __RTC_H
#include "../utils.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#define RTC_CTL_REGISTER 0x70
#define RTC_OUT_REGISTER 0x71
#define RTC_IRQ 8
#define RTC_UPDATING BIT(7)
#define RTC_BINARY BIT(2)
#define RTC_UPDATE_REG 10
#define RTC_COUNTING_REG 11
#define READ_SECONDS 0
#define READ_MINUTES 2
#define READ_HOURS 4
#define READ_DAYS 7
#define READ_MONTHS 8
#define READ_YEARS 9

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} rtc_values;
int read_rtc_output(uint8_t cmdbyte, uint8_t *result);
int(rtc_subscribe_int)(uint8_t *bit_no);
int(rtc_unsubscribe_int)();
int is_updating_rtc();
bool is_binary_rtc();
uint8_t to_binary(uint8_t bcd_number);
int rtc_update_values();
void start_rtc();
#endif
