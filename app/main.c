#include "usart.h"

int main(void) {
	usart_init();
	
	while(1) {
		usart_send_byte('A');
	}
}
