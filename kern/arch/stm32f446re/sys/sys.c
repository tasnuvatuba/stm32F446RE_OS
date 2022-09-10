#include "../include/sys/sys.h"
#include "../../../include/kstdio.h"

uint32_t mscount = 0;
uint32_t delay = 0;


void sysTick_init(uint32_t t){
	
	delay = t;
	//clock source ahb
	SysTick->CTRL |= (1<<2);
	//enables the interrupt by setting TICKINT to 1
	SysTick->CTRL |= (1<<1);
	//disbales the timer
	SysTick->CTRL |= (0<<0);
	//sets the val value to 0
	SysTick->VAL |= 0x00;
	//loads the reload value 
	SysTick->LOAD =  180000* t - 1;
	//sets mscount to 0
	mscount = 0;
	//enables the timer
	SysTick->CTRL |= (1<<0);

}
void sysTick_enable(void){
	
	if(! (SysTick->CTRL & (1<<0)) )
	{
		mscount = 0;
		SysTick->CTRL |= (1<<0);
	}
		
	
}
void sysTick_disable(void)
{
	
	if( (SysTick->CTRL & (1<<0)) )
	{
		SysTick->CTRL &= ~(1<<0);
	}
	
}


uint32_t getSysTickCount(void)
{
	return SysTick->VAL;
}



void updateSysTick(uint32_t load)
{
	delay = load;
	SysTick->CTRL |= (1<<2);
	SysTick->CTRL |= (1<<1);
	SysTick->CTRL |= (0<<0);
	SysTick->VAL |= 0x00;
	SysTick->LOAD = 180000*load - 1;
	mscount = 0;
	SysTick->CTRL |= (1<<0);

}

uint32_t getTime(void){
	uint32_t cur = SysTick->VAL;
	//number of cycles per x ms
	uint32_t limit = 180000 * delay; 
	//total time from the beginning of time 
	//1cycle executed in 1/ 180000 ms
	float tTime = (limit - cur)/180000 + mscount;
	return (uint32_t) tTime;
}


uint32_t getTimeElapsed(void)
{
	uint32_t cur = SysTick->VAL;
	//number of cycles per x ms
	uint32_t limit = 180000 * delay; 
	//total time from the beginning of time 
	//1cycle executed in 1/ 180000 ms
	float tTime = (limit - cur)/180000;
	return (uint32_t) tTime;
}

void  SysTick_Handler(void)
{
    mscount += delay;

}



////////////////////////////

void NVIC_SetPriority(enum IRQn_TypeDef IRQn, uint32_t priority){
	if(priority<0 || priority>15){
		kprintf((uint8_t*) "%s", (uint8_t*) "Priority should be between 0 and 15\n"); 
		return;
	}
	uint32_t x = IRQn;
	NVIC->IP[x] = priority<<4; //MSB 4 bits are used as priority
}
	
uint32_t NVIC_GetPriority(enum IRQn_TypeDef IRQn){
	uint32_t x = IRQn;
	return NVIC->IP[x]>>4; //MSB 4 bits are used as priority

}


void NVIC_EnableIRQn(enum IRQn_TypeDef IRQn){
	uint32_t x = IRQn/32;
	uint32_t offset = IRQn%32;
	
	NVIC->ISER[x] |= (1<<offset);

}

void NVIC_DisableIRQn(enum IRQn_TypeDef IRQn){
	uint8_t x = IRQn/32;
	uint8_t offset = IRQn%32;
	
	NVIC->ICER[x] |= (1<<offset);
	
}
	
	
	
void disable_irq(){ //Masking interrupts other than HardFault, NMI, and reset.
	__asm volatile("CPSID i"); //set primask
}
	

void set_BASEPRI(uint32_t value){
	if(value<0 || value>15){
		kprintf((uint8_t*) "%s", (uint8_t*) "Priority should be between 0 and 15\n"); 
		return;
	}
	value = value<<4;  //bit 4 to 7 are used as basepri value
	__asm volatile ("MSR basepri, %0" : : "r" (value) :);

}
	
	
void enable_irq(){  //unset primask
	__asm volatile("CPSIE i");
}

void unset_BASEPRI(){
	uint8_t value = 0<<4;
	__asm volatile ("MSR basepri, %0" : : "r" (value) :);
}

