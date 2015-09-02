
#include "usart.h"
#include "sdcard.h"
#include "stm32f10x.h"
#include "diskio.h"		/* Declarations of disk I/O functions */
#include "ff.h"
FATFS FatFs;   /* Work area (file system object) for logical drive */
int main(void) {
	
	
		FIL fil;       /* File object */
    char line[82]; /* Line buffer */
    FRESULT fr;    /* FatFs return code */
    //FatFs.drv = 1;
		usart_init();
		/* Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and Systick-Interrupt */
		if (SysTick_Config(SystemCoreClock / 100))
		{
			/* Capture error */
			while (1);
		}
		 printf("finish\r\n");
		 MMC_disk_initialize();
//    /* Register work area to the default drive */
    f_mount(&FatFs, "", 1);

//    /* Open a text file */
   fr = f_open(&fil, "message.txt", FA_CREATE_NEW | FA_WRITE);
//		f_write(&fil,"helllo");
//    //if (fr) return (int)fr;

//    

//    /* Close the file */
    f_close(&fil);
	
    printf("finish\r\n");

    //return 0;

	while(1) {
	//	printf("hello\r\n");
	}
}

RAMFUNC void SysTick_Handler(void)
{
	static uint8_t  cntdiskio=0;
	cntdiskio++;
	printf("a");
	if ( cntdiskio >= 10 ) {
		cntdiskio = 0;
		disk_timerproc(); /* to be called every 10ms */
	}	
}