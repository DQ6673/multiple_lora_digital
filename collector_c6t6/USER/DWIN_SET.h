#ifndef __DWIN_SET_H
#define __DWIN_SET_H

#include "sys.h"

//�����������������
#define DWIN_TX_LEN  			30
#define DWIN_RX_LEN  			30



//������������ɫ��
#define			D_YELLOW			0XFFE0					//��ɫ
#define 		D_ORANGE			0XFC00					//��ɫ
#define 		D_GREEN				0X16C0					//��ɫ
#define			D_RED					0XF800					//��ɫ




//��������
extern u8  DWIN_TX_BUF[DWIN_TX_LEN]; 
extern u8  DWIN_RX_BUF[DWIN_RX_LEN]; 
extern u8  DWIN_RX_CNT;
extern u8  DWIN_Analysis_EN;



//����
void DWINSendMessage(void);
void DWINSysReset(void);
void DWINPagePicSet( u8 PageNum);
void DWINdatadisPlay( u16 addr , u8 cmd);
void DWINsenddata( u16 addr, u16 num);
void DWINsendtext( u16 addr, u16 *TextP);
void DWINtextclear( u16 addr, u8 len);

#endif
