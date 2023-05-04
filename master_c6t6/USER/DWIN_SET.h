#ifndef __DWIN_SET_H
#define __DWIN_SET_H

#include "sys.h"

//输入输出缓冲区长度
#define DWIN_TX_LEN  			30
#define DWIN_RX_LEN  			30



//迪文屏字体颜色宏
#define			D_YELLOW			0XFFE0					//黄色
#define 		D_ORANGE			0XFC00					//橘色
#define 		D_GREEN				0X16C0					//绿色
#define			D_RED					0XF800					//红色

//迪文屏字体宏（GBK）
#define 		OPEN							0XB1D5BACF	//闭开
#define 		CLOSE							0XB6CFBFAA	//断开
#define 		FINE							0XC1BC			//良
#define 		ERR								0XB2EE			//差


//变量声明
extern u8  DWIN_TX_BUF[DWIN_TX_LEN]; 
extern u8  DWIN_RX_BUF[DWIN_RX_LEN]; 
extern u8  DWIN_RX_CNT;
extern u8  DWIN_Analysis_EN;



//函数
void DWINSendMessage(void);
void DWINSysReset(void);
void DWINPagePicSet( u8 PageNum);
void DWINdatadisPlay( u16 addr , u8 cmd);
void DWINsenddata( u16 addr, u16 num);
void DWINsendtext( u16 addr, u16 *TextP);
void DWINtextclear( u16 addr, u8 len);
void DWINsettextcolor( u16 addr, u16 color);

#endif
