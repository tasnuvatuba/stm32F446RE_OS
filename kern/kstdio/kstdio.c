#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/dev/usart.h"

/**
* first argument define the type of string to kprintf and kscanf, 
* %c for charater
* %s for string, 
* %d for integer
* %x hexadecimal
* %f for floating point number
*/
void kprintf(uint8_t *format, uint8_t* outvar)
{
//write your code here
uint32_t mul[4] = {1, 256, 65536, 16777216}; //{2^0, 2^8,  2^16, 2^24 }
format++;
	
	/************unsigned int************/
	if(*format == 'd'){//unsigned int
		
		uint32_t a= 0;
		char s[12];
		uint8_t zFlag =1;
		for(int itr=0; itr<4; itr++){ // 4 byte int data
			a += (*outvar) * mul[itr];
			outvar++;
		}
		
		int i=11;
		s[i] = '\0';
		i--;
	  while(a){
			zFlag =0;
			s[i] = a%10 + 48;
			a=a/10;
			i--;
		}
		if(zFlag){ // if a = 0
			s[i] = '0';
			i--;
		}
		i++;
		UART2_SendString(s+i);
 
 }
	
 /******************hex**************/
 else if(*format == 'x'){  //hex
	 char s[10];
	 uint8_t a;
	 uint8_t i=9, temp;
	 s[i--] = '\0';
	 
	 for(int itr=0; itr<4; itr++){ //4 byte
		 a= *outvar;
		 temp = (a%16);
		 
		 if(temp<10){
			 s[i--] = temp + 48;
		 }
		 else{
			 s[i--] = temp + 55;
		 }
		 temp = a/16;
		 if(temp<10){
			 s[i--] = temp + 48;
		 }
		 else{
			 s[i--] = temp + 55;
		 }
		 
		 outvar++;
		 
		 
	 }
	 i++;
	 while(s[i]=='0' && i<8){//removing leading 0
		 i++;
	 }
	 UART2_SendString(s+i);
 }
 
 /************char************/
 else if(*format == 'c'){
	 UART2_SendChar(*outvar);
 
 }
 
 /*********string************/
 else if(*format == 's'){
	 char *s = (char*)outvar;
	 UART2_SendString(s);
 
 }
 
 /*********float************/
  else if(*format == 'f'){
	 uint8_t byte[4];
	 uint8_t signBit =0;
	 uint8_t exp;
	 //uint8_t expSign= 0;
	 uint32_t mantissa;
	 float f = 1.0;
	 char s[15];
	 
	 for(uint8_t i=0; i<4; i++){
		 byte[i] = *outvar;
		 outvar++;
	 }
	 
	 
	 signBit = byte[3] & 128;
	
	 
	 exp = (byte[3] << 1) + ((byte[2] & 128) >> 7);
	 if(exp>127){
		 exp = exp - 127;
		 for(uint8_t i= 0; i<exp; i++){
			 f*=2;
		 }
	 }
	 else{//negative
		 //expSign = 1;
		 exp = 127 - exp;
		 for(uint8_t i= 0; i<exp; i++){
			 f/=2;
		 }
	 }
	 
	 
	 mantissa = ((uint32_t)(byte[2] & 127) << 25);
	 mantissa = mantissa | ((uint32_t) byte[1] << 17);
	 mantissa = mantissa | ((uint32_t) byte[0] << 9);
	 
	 float m = 1.0;
	 float mts = 1.0;
	 for(uint8_t i=0; i<23; i++){
		 m/=2.0;
		 if(mantissa & (1<<(31-i))){
			 mts+=m;
		 }
		 
	 }
	 
	 f = f* mts; 
	 f = f* 1000;//3 places after decimal point
	 uint32_t temp = f;
	 
	 uint8_t zFlag =1;
	 int i= 11;
		s[i] = '\0';
		i--;
	  while(temp){
			zFlag =0;
			if(i == 7){
				s[i--] = '.';
				continue;
			}
			s[i] = temp%10 + 48;
			temp=temp/10;
			i--;
		}
		if(signBit){
			s[i--] = '-';
		}
		if(zFlag){ // if f = 0
			s[i] = '0';
			i--;
		}
		i++;
		UART2_SendString(s+i);
	 
	 
	}
	
}



/**********************scanf****************/
void kscanf(uint8_t *format,uint8_t* invar)
{
//write your code here
	format++;
	char *s= UART2_GetString();
	
	/************unsigned int**************/
	if(*format == 'd'){//int
		uint32_t a =0;
		while(*s){
			a = a*10 + ((*s) - 48);
			s++;
		}
		//done
		uint8_t *temp = (uint8_t*)&a;
		for(int itr=0; itr<4; itr++){// int 4 byte
			*invar = *temp;
			invar++;
			temp++;
		}
		invar-=4;
	}
	
	/*************hex*******************/
	else if(*format == 'x'){
		uint32_t a =0;
		while(*s){
			if(*s >='0' && *s<='9'){
				a = a*16 + ((*s) - 48);
			}
			else if(*s >= 'A' && *s<= 'F'){
				a = a*16 + ((*s) - 55);
			}
			//else wrong hex
			s++;
		}
		//done
		uint8_t *temp = (uint8_t*)&a;
		for(int itr=0; itr<4; itr++){// int 4 byte
			*invar = *temp;
			invar++;
			temp++;
		}
		invar-=4;
		
	}
	
	/*************char**********/
	else if(*format == 'c'){
		*invar = *s;
		
	}
	
	/**********string*********/
	else if(*format == 's'){
		int i=0;
		while(*s){
			*invar = *s;
			invar++;
			s++;
			i++;
		}
		*invar = 0;
		i++;
		invar -= i;
		
	}
	
	/******float**********/
	else if(*format == 'f'){
		float f=1.0;
		int x = 0;
		
		if( *s && *s=='-'){
			f=f*-1;
			s++;
		}
		
		
		int cnt =1;
		uint8_t cntFlag =0;
		while(*s){
			if(*s == '.'){
				s++;
				cntFlag =1;
				continue;
				}
			
			x=x*10 + *s - 48;
			s++;
				
			if(cntFlag) cnt*=10;
		}
		
		f *= x;
		f /= cnt;
		
		uint8_t *temp = (uint8_t*) &f;
		for(uint8_t i=0; i<4; i++){
			*invar = *temp;
			temp++;
			invar++;
		}
		
		
	}



}




