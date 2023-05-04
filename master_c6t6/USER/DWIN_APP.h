#ifndef __DWIN_APP_H
#define __DWIN_APP_H

#include "DWIN_SET.H"


void DWIN_Buffer_Analysis(void);
void PageNodeList_Handler(void);
void PageNodelist_Clear(void);
void PageNodeList_Update(void);
void NodeInfHandler(void);
void DWIN_init(void);
void DWINchangepage(void);
void Addnewdevice(void);
void nodeBuf_refresh(void);
void OneNode_Update(void);
void DeleteOneNode(void);
void Addr_change(void);

#endif
