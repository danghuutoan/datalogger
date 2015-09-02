/* Martin Thomas 4/2009 */

#include "integer.h"
#include "fattime.h"
#include "stm32f10x.h"
#include <time.h>

DWORD get_fattime (void)
{
#if 1
	  DWORD date;
	  time_t _sec = RTC_GetCounter();
	  int tmp_year;
    struct tm *t = localtime(&_sec);
	  tmp_year=t->tm_year-2000;
    date = 0;
    date = (tmp_year<< 25) | ((t->tm_mon+1)<<21) | (t->tm_mday<<16)|\
            (t->tm_hour<<11) | (t->tm_min<<5) | (t->tm_sec>>1);
    return date;
#else
	return 0;
#endif  
}

