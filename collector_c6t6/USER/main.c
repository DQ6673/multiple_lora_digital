#include "syscfg.h"


int main(void){

	systemconfig();
	
	Lorapairrequest();
	delay_ms(800);
	
	while(1)
	{
		if((detect_temp_now != detect_temp) || (detect_flag == 1))
		{
			detect_temp_now = detect_temp;
			lorasend();
			detect_flag = 0;
		}
		IWDG_Feed();
	}
}

