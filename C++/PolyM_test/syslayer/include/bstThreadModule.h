/***************************************************************
文件名:	bstThreadModule.h
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/

#ifndef _BSTPTHREADMGR_H
#define _BSTPTHREADMGR_H
extern pthread_t bstThreadCreate(PTHREADFUN pThreadFun, void *vpPthdFunArg, signed char *pchModName, unsigned char byPthreadPri);
extern pthread_t bstThreadIdGet(void);
extern unsigned short bstThreadDelay(pthread_t cPid, unsigned short woTime);
extern unsigned short bstThreadQuit(pthread_t cPid);
extern unsigned short bstThreadWait(pthread_t oPid);
extern unsigned short bstCriticalRegionUnLock(pthread_t cPid, unsigned short lockState);
extern unsigned short bstCriticalRegionLock(pthread_t cPid, unsigned short lockState);
extern unsigned short bstCondUnLock(pthread_t cPid, unsigned short lockState);
extern unsigned short bstCondLock(pthread_t cPid, unsigned short lockState);
extern unsigned short bstCondWait(signed short swCondVal, pthread_t cPid);
extern unsigned short bstCondSignal(signed short swCondVal, pthread_t cPid);
#endif	/*_BSTPTHREADMGR_H*/











 
