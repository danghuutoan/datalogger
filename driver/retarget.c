#pragma import(__use_no_semihosting_swi)
#include <stdio.h>
#include "usart.h"
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
FILE __stderr;


static int SER_PutChar (int c) {

	usart_send_byte(c);
	return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
static int SER_GetChar (void) {
	return 0;

}



int fputc(int c, FILE *f) {
  if (c == '\n')  {
    SER_PutChar('\r');
  }
  return (SER_PutChar(c));
}


int fgetc(FILE *f) {
  return (SER_PutChar(SER_GetChar()));
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  SER_PutChar(c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}

