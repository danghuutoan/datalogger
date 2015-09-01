#include "usart.h"

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static void vConsoleTask( void *pvParameters );

int main(void) {
	
	usart_init();
	xTaskCreate( vConsoleTask, "Console", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	
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
