#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define USARTx						USART1
#define USARTx_PORT				GPIOA
#define USARTx_TX_PIN			GPIO_Pin_9
#define USARTx_RX_PIN			GPIO_Pin_10
#define USARTx_BAUDRATE   115200

void usart_init( void) {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA 	| \
													RCC_APB2Periph_AFIO 	| \
													RCC_APB2Periph_USART1, 	\
													ENABLE);
	
	/* Configure the USART_Tx as Alternate function Push-Pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin =  USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_PORT, &GPIO_InitStructure);
  
  /* Configure the USART_Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_PORT, &GPIO_InitStructure);
	
	/*-------------------------------------------------------
  USART configured as:
        - Word Length = 8 Bits
        - 1 Stop Bit
        - No parity
        - BaudRate = USART_BAUDRATE bps
        - Receive and transmit enabled
  -------------------------------------------------------*/

  USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);
  
  USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;
  USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;  
  USART_ClockStructInit(&USART_ClockInitStruct);
 
  
  
  /* Enable the USART */
  USART_Cmd(USARTx, ENABLE);  
}

void usart_send_byte(uint8_t l_data_u8) {
	
	/* wait until tx buffer is empty*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx,l_data_u8);
}
