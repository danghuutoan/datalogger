#include "usart.h"

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "rtc.h"
#include "list.h"

#include "sdcard.h"
#include "stm32f10x.h"
#include "diskio.h"		/* Declarations of disk I/O functions */
#include "ff.h"

TimerHandle_t xSDTimer;

#define f_size(fp) ((fp)->fsize)
FATFS fs;      /* File system object (volume work area) */
FIL fil;       /* File object */

static void vConsoleTask( void *pvParameters );
static void vRTCTask( void *pvParameters );

static void vSDTimerCallback( TimerHandle_t pxTimer ) {
	disk_timerproc(); /* to be called every 10ms */
	printf("message from timer \r\n");
}
int main(void) {
	
	usart_init();
	PL_InitRTC();
	f_mount(0,&fs);
	xSDTimer = xTimerCreate(     "Timer",         // Just a text name, not used by the kernel.
                                         ( 10 ),     // The timer period in ticks.
                                         pdTRUE,         // The timers will auto-reload themselves when they expire.
                                         ( void * ) 1,     // Assign each timer a unique id equal to its array index.
                                         vSDTimerCallback     // Each timer calls the same callback when it expires.
                                     );
																				
	if( xSDTimer == NULL ) {
              // The timer was not created.
  }
  else {
		
		// Start the timer.  No block time is specified, and even if one was
		// it would be ignored because the scheduler has not yet been
		// started.
		if( xTimerStart( xSDTimer, 0 ) != pdPASS )
		{
				// The timer could not be set into the Active state.
		}
	}
	xTaskCreate( vConsoleTask, "Console", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( vRTCTask, "RTC", configMINIMAL_STACK_SIZE+ 1000, NULL, tskIDLE_PRIORITY, NULL );
	vTaskStartScheduler();

	/* Will only get here if there was not enough heap space to create the
	idle task. */
	return 0;
}


void vConsoleTask( void *pvParameters ) {
	static uint8_t l_count_u8;
	for( ;; ) {
		/* test message */
		l_count_u8++;
		printf("hello %d\r\n", l_count_u8);
		vTaskDelay(1000);
	}
}


void vRTCTask( void *pvParameters ) {
	uint8_t l_timebuffer [7];
	for( ;; ) {
		/* test message */
		PL_RTC_GetTime(l_timebuffer);
		uint16_t l_year_u16 = (l_timebuffer[3]<<8) +l_timebuffer[4];
		printf("%d:%d:%d %d-%d-%d\r\n",l_timebuffer[0],l_timebuffer[1],l_timebuffer[2],l_year_u16, l_timebuffer[5],l_timebuffer[6]);
		
		//f_open(&fil,"message.txt", FA_WRITE | FA_OPEN_ALWAYS); 
		//f_lseek(&fil, f_size(&fil));
		//f_printf(&fil,"test with sd card ok  \r\n");

		/* Close the file */
		//f_close(&fil);
		vTaskDelay(1000);
	}
}


