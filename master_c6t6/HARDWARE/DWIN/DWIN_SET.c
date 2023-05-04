#include "DWIN_SET.H"
#include "delay.h"
#include "sys.h"


u8  DWIN_Analysis_EN = 0;						//屏幕指令分析使能标志位
u8  DWIN_TX_BUF[DWIN_TX_LEN] = {0X5A ,0XA5};	//发送数据，以 5A A5 为报头
u8  DWIN_RX_BUF[DWIN_RX_LEN];     	//接收缓冲,最大 DWIN_REC_LEN 个字节.
u8  DWIN_RX_CNT = 0;										//接收计数，串口中断函数使用



/////////////////////////////////////////////////////////////////
//文本发送函数，用来改变文本显示控件的显示内容
//addr   : 16位地址，如0X6530；
//*TextP : 数组中的元素地址，所指内容为该文本的长度，以便确定发送长度
//用法：Text_upload( 0X6530, &Time_Buf[0]);
/////////////////////////////////////////////////////////////////
void DWINsendtext( u16 addr, u16 *TextP)
{
	u8 Text_Len = *TextP & 0X00FF;

	if(Text_Len != 0)
	{
		DWIN_TX_BUF[4] = addr >> 8;
		DWIN_TX_BUF[5] = addr & 0X00FF;

		DWIN_TX_BUF[2] = Text_Len + 3;
		DWIN_TX_BUF[3] = 0X82;

		for(u8 i = 0; i < Text_Len; i++)
		{
			if( i%2 == 0)
			{
				DWIN_TX_BUF[6 + i] = *(TextP + 1 + i/2) >> 8;
			}

			else
			{
				DWIN_TX_BUF[6 + i] = *(TextP + 1 + i/2) & 0X00FF;
			}
		}

		DWINSendMessage();
	}
}




/////////////////////////////////////////////////////////////////
//数据发送函数，用来改变文本显示控件的显示内容
//addr   : 16位地址，如0X6530；
//*TextP : 数组中的元素地址，所指内容为该文本的长度，以便确定发送长度
//用法：Text_upload( 0X6530, &Time_Buf[0]);
/////////////////////////////////////////////////////////////////
void DWINsenddata( u16 addr, u16 num)
{
	DWIN_TX_BUF[2] = 0X05;
	DWIN_TX_BUF[3] = 0X82;

	DWIN_TX_BUF[4] = addr >> 8;
	DWIN_TX_BUF[5] = addr & 0x00FF;

	DWIN_TX_BUF[6] = num >> 8;
	DWIN_TX_BUF[7] = num & 0x00FF;

	DWINSendMessage();
}





//////////////////////////////////////////////////////////////////////
//数据变量显示/隐藏
//addr : 变量地址
//Cmd  : 是否显示，1显示，0隐藏
//隐藏数据指令：5AA5 05 82 9000 FF00
//显示数据指令：5AA5 05 82 9000 1000
//其中1000为变量地址 9000 为描述指针（为变量指针 + 0X8000），FF00为隐藏，此处写变量地址即为显示
//////////////////////////////////////////////////////////////////////
void DWINdatadisPlay( u16 addr , u8 cmd)
{
	u16 Dp = addr + 0X8000;

	DWIN_TX_BUF[2] = 0X05;									//数据长度
	DWIN_TX_BUF[3] = 0X82;									//82为写

	DWIN_TX_BUF[4] = Dp >> 8;								//描述指针高八位
	DWIN_TX_BUF[5] = Dp & 0X00FF;						//描述指针低八位
	
	if(cmd)
	{
		DWIN_TX_BUF[6] = addr >> 8;						//写入变量地址
		DWIN_TX_BUF[7] = addr & 0x00FF;
	}

	else
	{
		DWIN_TX_BUF[6] = 0XFF;								//写入FF00
		DWIN_TX_BUF[7] = 0X00;
	}

	DWINSendMessage();
}




//////////////////////////////////////////////////////////////////////
//清除文本区
//该函数向目标文本控件写入空格（0X20）实现清除操作
//注：一个文字需写入两个字节
//addr：变量地址
//len ：长度
//指令：5AA5 05 82 1300 2020 （清除两个字符或一个汉字）
//1300：变量地址， 2020：空格
//////////////////////////////////////////////////////////////////////
void DWINtextclear( u16 addr, u8 len)
{
	DWIN_TX_BUF[2] = 0X03 + len;						//数据长度
	DWIN_TX_BUF[3] = 0X82;									//82为写
	
	DWIN_TX_BUF[4] = addr >> 8;								//变量指针高八位
	DWIN_TX_BUF[5] = addr & 0X00FF;						//变量指针低八位
	
	u8 i = 0;
	
	for(; i<len ; i++)
	{
		DWIN_TX_BUF[ 6 + i ] = 0X20;
	}
		
	DWINSendMessage();
}





//////////////////////////////////////////////////////////////////////
//设置文本颜色
//addr : 变量地址
//cmd  : 是否显示，1显示，0隐藏
//设置颜色指令：5AA5 05 82 9503 F800
//9503：描述指针偏移三位
//F800：红色代码
//////////////////////////////////////////////////////////////////////
void DWINsettextcolor( u16 addr, u16 color)
{
	u16 Dp = addr + 0X8003;
	u16 color_set = color;
	
	DWIN_TX_BUF[2] = 0X05;									//数据长度
	DWIN_TX_BUF[3] = 0X82;									//82为写
	
	DWIN_TX_BUF[4] = Dp >> 8;								//描述指针高八位
	DWIN_TX_BUF[5] = Dp & 0X00FF;						//描述指针低八位
	
	DWIN_TX_BUF[6] = color_set >> 8;				//写入颜色代码
	DWIN_TX_BUF[7] = color_set & 0x00FF;
	
	DWINSendMessage();
}







//设置屏幕页面（最小值0）
//PageNum : 页面号
void DWINPagePicSet( u8 PageNum)
{
	DWIN_TX_BUF[2] = 0X07;
	DWIN_TX_BUF[3] = 0X82;

	DWIN_TX_BUF[4] = 0X00;
	DWIN_TX_BUF[5] = 0X84;

	DWIN_TX_BUF[6] = 0X5A;
	DWIN_TX_BUF[7] = 0X01;

	DWIN_TX_BUF[8] = 0X00;
	DWIN_TX_BUF[9] = PageNum;

	DWINSendMessage();
}






//屏幕重新启动
void DWINSysReset()
{
	DWIN_TX_BUF[2] = 0X07;
	DWIN_TX_BUF[3] = 0X82;

	DWIN_TX_BUF[4] = 0X00;
	DWIN_TX_BUF[5] = 0X04;

	DWIN_TX_BUF[6] = 0X55;
	DWIN_TX_BUF[7] = 0XAA;

	DWIN_TX_BUF[8] = 0X5A;
	DWIN_TX_BUF[9] = 0XA5;

	DWINSendMessage();
}





//发送信息，将缓存数组中的单条信息通过串口发送到迪文屏
void DWINSendMessage()
{
	for( u8 i = 0 ; i < DWIN_TX_BUF[2] + 3 ; i++)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, DWIN_TX_BUF[i]);
	}
}

