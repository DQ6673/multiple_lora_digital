#ifndef __RELAY_H
#define __RELAY_H

#include "sys.h"



#define RELAY_1 PBout(7)
#define RELAY_8 PBout(3)
#define RELAY_2 PBout(6)
#define RELAY_3 PBout(5)
#define RELAY_4 PBout(4)
#define RELAY_5 PBout(2)
#define RELAY_9 PBout(11)
#define RELAY_6 PBout(10)

#define RELAY_7 PAout(15)



void RELAY_Init(void);
void Relay_Set(u8 num, u8 com);


#endif
