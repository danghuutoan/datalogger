
#include "usart.h"
#include "sdcard.h"
#include "stm32f10x.h"
#include "diskio.h"		/* Declarations of disk I/O functions */
#include "ff.h"

#define f_size(fp) ((fp)->fsize)
int main(void) {
	
	
	FATFS fs;      /* File system object (volume work area) */
	FIL fil;       /* File object */
	usart_init();
	//disk_initialize(0);
	/* Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and Systick-Interrupt */
	if (SysTick_Config(SystemCoreClock / 100))  {
		/* Capture error */
		while (1);
	}
	printf("start creating file\r\n");    
	
	f_mount(0,&fs);
	f_open(&fil,"message.txt", FA_WRITE | FA_OPEN_ALWAYS); 
	f_lseek(&fil, f_size(&fil));
	f_printf(&fil,"test without disk_initialize ok  \r\n");

  /* Close the file */
  f_close(&fil);
	
  printf("finish\r\n");


	while(1) {
	}
}

RAMFUNC void SysTick_Handler(void)
{
	static uint8_t  cntdiskio=0;
	printf("a");
	cntdiskio++;
	if ( cntdiskio >= 10 ) {
		cntdiskio = 0;
		disk_timerproc(); /* to be called every 10ms */
	}	
}
