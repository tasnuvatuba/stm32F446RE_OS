#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"
#include <stdint.h>

void checkPriority(enum IRQn_TypeDef IRQn, uint32_t x);
void kmain(void)
{
	__sys_init();
	enum IRQn_TypeDef IRQn = USART2_IRQn ;
	NVIC_EnableIRQn(IRQn);

	//PA5= GPIOA pin 5 output mode
	RCC->AHB1ENR |= (1<<0);
	GPIOA->MODER |= (1<<10);

	kprintf((uint8_t*)"%s",(uint8_t*) "basepri\n"); 
	//NVIC_DisableIRQn(IRQn);
	//disable_irq();
	//enable_irq();
	NVIC_SetPriority(IRQn, 5);
	set_BASEPRI(6);
	unset_BASEPRI();
	while(1){
		GPIOA->BSRR |= (1<<5);
	}
	
	
	
	
}

void checkPriority(enum IRQn_TypeDef IRQn, uint32_t x){
	uint32_t p= NVIC_GetPriority(IRQn);
	kprintf((uint8_t*)"%s",(uint8_t*)"prev: "); 
	kprintf((uint8_t*)"%d",(uint8_t*)&p);
	p= NVIC_GetPriority(IRQn);
	kprintf((uint8_t*)"%s",(uint8_t*)",  after setting: "); 
	kprintf((uint8_t*)"%d",(uint8_t*)&p);
	kprintf((uint8_t*)"%s",(uint8_t*)"\n");
	
}

