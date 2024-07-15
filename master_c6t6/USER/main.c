#include "syscfg.h"

u16 arr[3][4] = {{0X4356, 0x5268, 0x8975, 0x5422}, {0X4552, 0x5268, 0x8975, 0x5422}, {0X5555, 0x5268, 0x8975, 0x5422}};
u16 a;

#define PEER_NEED_NON_SCREEN
#define PEER_UID 0X1B5F7A7C
#define PEER_COMNUM 5

sensor sensorInfo;
void LoadNodeInfoNonScreen(void)
{
	sensorInfo.devicenum = 1;
	sensorInfo.UID = PEER_UID;
	sensorInfo.comnum = PEER_COMNUM;

	DeviceInfSaveRom(&sensorInfo);
}

int main(void)
{
	driver_init();

#ifdef PEER_NEED_NON_SCREEN
	LoadNodeInfoNonScreen();
#endif

	software_init();

	//
	while (1)
	{
		lora_receive();
		Lora_callback();
		signalerror_check();
		IWDG_Feed();
	}
}
