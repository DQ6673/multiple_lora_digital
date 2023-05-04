#ifndef __DETECTION_H
#define __DETECTION_H 			

#include "sys.h"
#include "stm32f10x_exti.h"


#define 		Din_1   	PBin(6)				//一号输入脚
#define 		Din_2   	PBin(5)				//一号输入脚
#define 		Din_3   	PBin(4)				//一号输入脚
#define 		Din_4   	PBin(2)				//一号输入脚
#define 		Din_5   	PBin(10)				//一号输入脚
#define 		Din_6   	PAin(15)				//一号输入脚
#define 		Din_7   	PBin(3)				//一号输入脚
#define 		Din_8   	PBin(11)				//一号输入脚



extern u8 detect_flag;
extern u8 detect_temp;
extern u8 detect_temp_now;


void detection_init(void);
void detect_scan(void);
void detectIOinit(void);
void detectNVICinit(void);
void detectEXTIinit(void);

#endif
