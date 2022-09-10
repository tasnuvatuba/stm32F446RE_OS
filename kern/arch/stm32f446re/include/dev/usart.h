#ifndef __USART_H
#define __USART_H
#include "../sys/stm32_peps.h"
void DRV_FPU_ACTIVE(void);
void DRV_USART_INIT(USART_TypeDef*);

void UART2_SendChar(uint8_t c);
void UART2_SendString(char *string);
uint8_t UART2_GetChar(void);
char* UART2_GetString();
#endif
