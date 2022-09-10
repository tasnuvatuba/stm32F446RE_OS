#include "../include/dev/usart.h"

//turn on the fpu
void DRV_FPU_ACTIVE(void){
    SCB->CPACR |= ((3UL << 10*2) |  (3UL << 11*2)  );
}


void DRV_USART_INIT(USART_TypeDef* usart)
{	
	//1. Enable the Uart clock and GPIO clock
	RCC->APB1ENR |= (1<<17);
	RCC->AHB1ENR |= (1<<0);
	
	//2. Configure the Uart pins for alternating function
	//using pin PA2 and PA3 for communication
	GPIOA->MODER |= (2<<4);
	GPIOA->MODER |= (2<<6);
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);
	
	GPIOA->AFRL |= (7<<8); //AFR[0] represents AFRL register
	GPIOA->AFRL |= (7<<12);
	
	
	
	//3. Enable the USART by writing the UE bit in USART_CR1 register to 1
	USART2->CR1 = 0x00; //reset
	USART2->CR1 |= (1<<13);
	
	//4. program the M bit in USART_CR1 to define the word length
	USART2->CR1 &= ~(1<<12);
	
	//5. Select the desired baud rate using the USART_BRR regsiter
	USART2->BRR = (7<<0) | (24<<4);
	
	//6. Enable the transmitter/receiver by setting the TE and RE bits in USART_CR1 regsister
  	USART2->CR1 |= (1<<2);
	USART2->CR1 |= (1<<3);

	//interrupt
	USART2->CR1 |= (1<<5); //RXNEIE
	//USART2->CR1 |= USART_CR1_TXEIE;
	//USART2->CR1 |= (1<<6); //TCIE- transmission complete
	
	
}




//////send and receive
void UART2_SendChar(uint8_t c){
	
	USART2->DR = c;
	while(!(USART2->SR & (1<<6)));
}

void UART2_SendString(char *string){
	while(*string)
		UART2_SendChar(*string++);
}

uint8_t UART2_GetChar(void){
	uint8_t temp;
	while(!(USART2->SR & (1<<5)));
	temp = USART2->DR;
	return temp;
}

char string[100];
char* UART2_GetString(){
	
	int idx=0;
	while(1){
		char s= UART2_GetChar();
		if(s==';') { //terminating char ;
			string[idx++] = '\0';
			return string;
		}
		else string[idx++] = s;
	}
}