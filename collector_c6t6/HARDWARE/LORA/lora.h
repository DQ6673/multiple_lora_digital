#ifndef __LORA_H
#define __LORA_H

#include "example/example_CAD.h"
#include "radio/sx127x_driver.h"
#include "hal/sx127x_hal.h"
#include "example/example_TX.h"
#include "example/example_RX.h"
#include "string.h"
#include "led.h"



extern uint8_t LoraRxBuf_Ana_EN;



void lora_init(void);
void lora_receive(void);
void LoraSendMessage(void);
void lorasend(void);
void Lora_callback(void);

#endif
