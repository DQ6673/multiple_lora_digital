#include "hal/sx127x_hal.h"
#include "stm32f10x_it.h"
/*
 * LoRa sx1278/76����
 *
 * \version    1.0.0 
 * \date       Dec 17 2019
 * \author     Specter
 */

#define RESET_IOPORT                                GPIOA
#define RESET_PIN                                   GPIO_Pin_11

/*!
 * SX1276 SPI definitions
 */
#define NSS_IOPORT                                  GPIOA
#define NSS_PIN                                     GPIO_Pin_4

#define SPI_INTERFACE                               SPI1
#define SPI_CLK                                     RCC_APB2Periph_SPI1

#define SPI_PIN_SCK_PORT                            GPIOA
#define SPI_PIN_SCK_PORT_CLK                        RCC_APB2Periph_GPIOA
#define SPI_PIN_SCK                                 GPIO_Pin_5

#define SPI_PIN_MISO_PORT                           GPIOA
#define SPI_PIN_MISO_PORT_CLK                       RCC_APB2Periph_GPIOA
#define SPI_PIN_MISO                                GPIO_Pin_6

#define SPI_PIN_MOSI_PORT                           GPIOA
#define SPI_PIN_MOSI_PORT_CLK                       RCC_APB2Periph_GPIOA
#define SPI_PIN_MOSI                                GPIO_Pin_7

/*!
 * SX1276 DIO pins  I/O definitions
 */
#define DIO0_IOPORT                                 GPIOA
#define DIO0_PIN                                    GPIO_Pin_10

#define DIO1_IOPORT                                 GPIOA
#define DIO1_PIN                                    GPIO_Pin_9

#define DIO3_IOPORT                                 GPIOB
#define DIO3_PIN                                    GPIO_Pin_15

#define DIO4_IOPORT                                 GPIOB
#define DIO4_PIN                                    GPIO_Pin_14

//�����ʱ������ms����
void Soft_delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=7950;  //�Լ�����  ����ʱ�䣺20141116
      while(i--) ;    
   }
}

//spi��ʼ��
static void SpiInit( void )
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable peripheral clocks --------------------------------------------------*/
    /* Enable SPIy clock and GPIO clock for SPIy */
    RCC_APB2PeriphClockCmd( SPI_PIN_MISO_PORT_CLK | SPI_PIN_MOSI_PORT_CLK |
                            SPI_PIN_SCK_PORT_CLK, ENABLE );
    RCC_APB2PeriphClockCmd( SPI_CLK, ENABLE );

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_SCK;
    GPIO_Init( SPI_PIN_SCK_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MOSI;
    GPIO_Init( SPI_PIN_MOSI_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MISO;
    GPIO_Init( SPI_PIN_MISO_PORT, &GPIO_InitStructure );

    //����JTAG
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );
    GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE );

    /* SPI_INTERFACE Config -------------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 72/8 MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI_INTERFACE, &SPI_InitStructure );
    SPI_Cmd( SPI_INTERFACE, ENABLE );
}


//SX127X��س�ʼ����Ҫ������������
//SPIƬѡ����Ϊ���,����ʼ��SPI��
//��λ�ǳ�ʼ��Ϊ����ߵ�ƽ
//DI00~5Ϊ���루��ʱֻ�õ���DIO0��DIO1���������Ӳ����ʱDIO1Ҳ���Բ��ӣ�
void SX1276HALInit( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                            RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure SPI-->NSS as output
    GPIO_InitStructure.GPIO_Pin = NSS_PIN;
    GPIO_Init( NSS_IOPORT, &GPIO_InitStructure );
		GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
		SpiInit();
	
		//���ø�λ����
		GPIO_InitStructure.GPIO_Pin = RESET_PIN;
    GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
		GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_SET );
	
    // Configure radio DIO as inputs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    // Configure DIO0
    GPIO_InitStructure.GPIO_Pin =  DIO0_PIN;
    GPIO_Init( DIO0_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO1
    GPIO_InitStructure.GPIO_Pin =  DIO1_PIN;
    GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );
		
		// Configure DIO3
    GPIO_InitStructure.GPIO_Pin =  DIO3_PIN;
    GPIO_Init( DIO3_IOPORT, &GPIO_InitStructure );
		
		// Configure DIO4
    GPIO_InitStructure.GPIO_Pin =  DIO4_PIN;
    GPIO_Init( DIO4_IOPORT, &GPIO_InitStructure );
}

//spi���ͺͽ��պ���
uint8_t SpiInOut( uint8_t outData )
{
    /* Send SPIy data */
    SPI_I2S_SendData( SPI_INTERFACE, outData );
    while( SPI_I2S_GetFlagStatus( SPI_INTERFACE, SPI_I2S_FLAG_RXNE ) == RESET );
    return SPI_I2S_ReceiveData( SPI_INTERFACE );
}

//spiƬѡ��status=0ʹ�ܣ�NSS���ͣ�status=1ʧ�ܣ�NSS���ߣ�
void SpiNSSEnable( uint8_t status )
{
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, (BitAction) status );
}

//��λ���ſ���status=0���ͣ���������
void SX127X_ResetPinControl( uint8_t status )
{
	GPIO_WriteBit( RESET_IOPORT, RESET_PIN,(BitAction) status );
}

//��ȡDIO0��ƽ������ֵ0�͵�ƽ��1�ߵ�ƽ
uint8_t SX1276ReadDio0(void){
	return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
}

//��ȡDIO1��ƽ������ֵ0�͵�ƽ��1�ߵ�ƽ
uint8_t SX1276ReadDio1(void){
	return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
}

/*uint8_t SX1276ReadDio2(void){
	return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
}*/

uint8_t SX1276ReadDio3(void){
	return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );
}

uint8_t SX1276ReadDio4(void){
	return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );
}

/*uint8_t SX1276ReadDio5(void){
	return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );
}*/
