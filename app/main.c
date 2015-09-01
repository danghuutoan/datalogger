#include "usart.h"

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "rtc.h"
static void vConsoleTask( void *pvParameters );
static void vRTCTask( void *pvParameters );
int main(void) {
	
	usart_init();
	PL_InitRTC();
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
