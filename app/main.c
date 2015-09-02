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
TimerHandle_t xSDTimer;

static void vConsoleTask( void *pvParameters );
static void vRTCTask( void *pvParameters );

static void vSDTimerCallback( TimerHandle_t pxTimer ) {
	
	printf("message from timer \r\n");
}
int main(void) {
	
	usart_init();
	PL_InitRTC();
	xSDTimer = xTimerCreate(     "Timer",         // Just a text name, not used by the kernel.
                                         ( 1000 ),     // The timer period in ticks.
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
	xTaskCreate( vRTCTask, "RTC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
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
		vTaskDelay(1000);
	}
}


