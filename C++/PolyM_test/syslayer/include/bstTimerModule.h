/***************************************************************
文件名:	bstTimerModule.h
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/

#ifndef _BSTTIMERMGR_H
#define _BSTTIMERMGR_H

//an hour.
#define ANHOUR (TMRMINBASENUM*60)
//half an hour.
#define HANHOUR (TMRMINBASENUM*30)
//10 senc for debug and test.
#define TENSENCS (TMRSBASENUM*10)

typedef enum
{
	TMRMOD_ONCE = 0x1,
	TMRMOD_CYCLE,
}TIMER_MODULE;

extern signed long bstTmrCreate(void);
extern unsigned short bstTmrStart(unsigned long dwTime, TIMERATNENTRY pTmrAtnEntry, void *pvArg, unsigned char byMod, signed long sdTmrNum);
extern unsigned short bstTmrDestory(signed long sdTmrNum);
#endif	/*_BSTTIMERMGR_H*/
