#include "usart.h"
int main(void) {
	
	usart_init();
	
	while(1) {
		printf("hello\r\n");
	}
}
