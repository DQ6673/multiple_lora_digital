#include "syscfg.h"

u8 pair_flag = 0;
u32 stm32mcuUID;

void systemconfig(void)
{
	GetUID();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_init();
	
	detection_init();
	
	LED_Init();							//LED ��ʼ����һ�����ڵ���
	lora_init();						//Lora ��ʼ��
	
	TIM3_Init();						//�� TIM4 ��ʱ�������ڶ�ʱ���˿�����˿�
	IWDG_Config(IWDG_Prescaler_64, 625);		//���Ź�
}


//Lora�������������Ϣ����
void Lorapairrequest(void)
{
	pair_flag = 1;
	lorasend();
	pair_flag = 0;
}



//��ȡstm32f1xxxxоƬ��ΨһID����
void GetUID(void)
{    
  stm32mcuUID += *(__IO u32*)(0X1FFFF7F2);//ID��ַ�ĸ�32λ�Ĵ�����ַ
  stm32mcuUID += *(__IO u32*)(0X1FFFF7EE);//ID��ַ����32λ�Ĵ�����ַ
  stm32mcuUID += *(__IO u32*)(0X1FFFF7E8);//ID��ַ�ĵ�32λ�Ĵ�����ַ
	//stm32mcuUID += 2;
}



//�������Ź���ʱ�����ã�
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{    
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();
}


//ι��
void IWDG_Feed(void)
{
   IWDG_ReloadCounter();
}



