#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE AT24C256

//��һ�������豸�����Ϣ
#define PAIR_STATE_ADDR   	0
#define RUNTIME_ADDR				128
//�ڶ������洫�������������Ϣ
#define SENSOR_INF_ADDR 		255
#define SENSOR_INF_OFFSET		12
//���������洫������ʷ����
#define SENSOR_DATA_ADDR    1024
#define SENSOR_DATA_OFFSET  576
#define Three_Day_DataSize  6								
//���������汨����Ϣ
#define SENSOR_WDATA_ADDR		30000
#define SENSOR_WDATA_OFFSET 10
//�����������ַ�
#define GBK_Code_ADDR       31000
#define GBK_Code_OFFSET     20

#define Day1   0
#define Day2   2
#define Day3   4

#define ROM_OP_NONE		0x00
#define ROM_Read  		0x01
#define ROM_Write 		0x02
						
u8 AT24CXX_ReadOneByte(u16 ReadAddr);														//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);				//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);										//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC

void Sensor_Inf_Load(uint8_t sn_temp);	//������������
void Sensor_Inf_Save(uint8_t sn_temp);
void Sensor_Data_Read(u8 sensor_sn,u8 day);	//��ȡ��ʷ���ݵ�������
#endif
















