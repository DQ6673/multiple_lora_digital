#include "syscfg.h"

u16 arr[3][4] = {{0X4356,0x5268,0x8975,0x5422},{0X4552,0x5268,0x8975,0x5422},{0X5555,0x5268,0x8975,0x5422}};
u16 a;


int main(void){
	
	
	systemconfig();
	//
	while(1)
	{
		lora_receive();
		Lora_callback();
		signalerror_check();
		IWDG_Feed();
	}
}


