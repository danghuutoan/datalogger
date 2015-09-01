#ifndef __RTC_H__
#define __RTC_H__
#include <stdbool.h>
#include "stm32f10x.h"
#define RTC_PREEMPTY_PRIORITY               4
#define RTC_SUB_PRIORITY                    0
#define TIME_BUFFER_LEN      								10
bool PL_InitRTC(void);
void PL_RTC_GetTime(u8 *timebuffer);
bool PL_RTC_SetTime(u8 *timebuffer);
void GetSysTime(uint8_t *timebuffer);
bool SetSysTime(uint8_t *timebuffer);
typedef struct
{
  uint8_t   hh;
  uint8_t   mm;
  uint8_t   ss;
  uint16_t  year;
  uint8_t   month;
  uint8_t   day;
  uint8_t   weekday;
  uint8_t   yearday;
  uint8_t   isdst;
} sytemdate_t;

extern  sytemdate_t sys_date;
extern  bool        sys_nextday;

#endif /*__RTC_H__*/

