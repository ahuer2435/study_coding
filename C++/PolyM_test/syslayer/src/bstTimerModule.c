/***************************************************************
文件名:	bstTimerModule.c
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/

//linux head file
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

//c head file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//self head file
#include "common.h"
#include "bstTimerModule.h"

//Timer managment table size.
#define TMRMODMGRTBLSIZE	65536

//Count base number of timer. 
#define TMRMSBASENUM	100
#define TMRSBASENUM	1000
#define TMRMINBASENUM	60000

typedef struct tmrmgr
{
	signed long		sdTmrNum;
	unsigned char 	byTmrMod;
	unsigned short 	wTmrState;
	unsigned long 	dwTime;
	unsigned long 	dwTick;
	unsigned long 	dwTmDiv;
	TIMERATNENTRY 		pTimerAtnEntry;
	void* 	pvAtnEntryArg;
	unsigned short 	wTmrErr;
	struct tmrmgr *  	pTmrMgrPrior;
	struct tmrmgr* 		pTmrMgrNext;
}TIMERMGRTABLE, *PTIMERMGRTABLE;

typedef struct tmrinfomgr
{
	unsigned long  	dwTmrCutNum;
	PTIMERMGRTABLE  pTmrMgrHead;
	PTIMERMGRTABLE  pTmrMgrTail;
}TIMERINFOMGRTABLE, *PTIMERINFOMGRTABLE;

volatile PTIMERMGRTABLE pTmrMgrMoveHead = NULL;
volatile PTIMERMGRTABLE pTmrMgrMoveTial = NULL;

pthread_t pThreadTmrMgrId = 0;
struct itimerval TimerVal;

const unsigned long	dwDiv = 100000;
static unsigned long g_dwTimeTickCout; 
#define ONE_DAY_TIME_TICK  4320000  
static signed long dwTmrMgrIdAllBaseNum = 0x1;
static pthread_mutex_t TmrMutexVal = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t Ticklock = PTHREAD_MUTEX_INITIALIZER;
static TIMERMGRTABLE TmrMgrTbl[TMRMODMGRTBLSIZE];


static TIMERINFOMGRTABLE TmrInfoMgrTbl = 
{

	0,
	NULL,

	NULL,

}; 

static TIMERINFOMGRTABLE TmrInfoMgrTblIdle = 
{
	0,
	NULL,

	NULL,
}; 

static TIMERINFOMGRTABLE TmrInfoMgrTblUse = 
{

	0,

	NULL,

	NULL,

}; 


static void SigHandler(signed short swSigNo)
{	
    g_dwTimeTickCout++;
    g_dwTimeTickCout = g_dwTimeTickCout%ONE_DAY_TIME_TICK;
    pthread_mutex_unlock(&Ticklock);
}

unsigned short bstTmrMgrInit(void)
{

    unsigned long  dwI;

    pthread_mutex_lock(&TmrMutexVal);	
    memset((void *)TmrMgrTbl, 0x0, sizeof(TmrMgrTbl));
    g_dwTimeTickCout = 0;
    TmrInfoMgrTblIdle.dwTmrCutNum = TMRMODMGRTBLSIZE-1;    
    for(dwI = 0;dwI< TMRMODMGRTBLSIZE-1;dwI++)
    {
       TmrMgrTbl[dwI].sdTmrNum = dwI;
       TmrMgrTbl[dwI].pTmrMgrNext = &TmrMgrTbl[dwI+1];
    }

    TmrInfoMgrTblIdle.pTmrMgrHead = TmrMgrTbl;
    TmrInfoMgrTblIdle.pTmrMgrTail = &TmrMgrTbl[dwI];
    pthread_mutex_unlock(&TmrMutexVal);
    return TIMER_NO_ERR;
}

signed long bstTmrCreate(void)
{	

    signed long  sdTmNum;

    if((NULL == TmrInfoMgrTblIdle.pTmrMgrHead )||TmrInfoMgrTblIdle.dwTmrCutNum <=0)
        return TIMER_CREAT_FAIL;

    pthread_mutex_lock(&TmrMutexVal);	
    if(NULL == TmrInfoMgrTblUse.pTmrMgrHead)
    {
        TmrInfoMgrTblUse.pTmrMgrHead = TmrInfoMgrTblIdle.pTmrMgrHead;
        TmrInfoMgrTblUse.pTmrMgrTail = TmrInfoMgrTblIdle.pTmrMgrHead;
    }
    else
    {
        TmrInfoMgrTblUse.pTmrMgrTail->pTmrMgrNext = TmrInfoMgrTblIdle.pTmrMgrHead;
        TmrInfoMgrTblIdle.pTmrMgrHead->pTmrMgrPrior = TmrInfoMgrTblUse.pTmrMgrTail;
        TmrInfoMgrTblUse.pTmrMgrTail = TmrInfoMgrTblUse.pTmrMgrTail->pTmrMgrNext;
    }

    sdTmNum =  TmrInfoMgrTblUse.pTmrMgrTail->sdTmrNum;
    TmrInfoMgrTblIdle.pTmrMgrHead = TmrInfoMgrTblIdle.pTmrMgrHead->pTmrMgrNext;
    TmrInfoMgrTblIdle.dwTmrCutNum --;
    TmrInfoMgrTblUse.dwTmrCutNum ++;
    TmrInfoMgrTblUse.pTmrMgrTail->wTmrState = TIMER_OPEN_YES;
    TmrInfoMgrTblUse.pTmrMgrTail->pTmrMgrNext = NULL;
    pthread_mutex_unlock(&TmrMutexVal);
    return sdTmNum;
}


unsigned short bstTmrStart(unsigned long dwTime, TIMERATNENTRY pTmrAtnEntry, void *pvArg, unsigned char byMod, signed long sdTmrNum)
{

    if(((0 >= dwTime) && (0 != (dwTime%100))) ||(NULL == pTmrAtnEntry)
		||((TMRMOD_ONCE != byMod) &&(TMRMOD_CYCLE != byMod)) || ((0 >sdTmrNum)&& (sdTmrNum < TMRMODMGRTBLSIZE)))
        return TIMER_ARG_ERR;

    if(TIMER_OPEN_ALREADY == TmrMgrTbl[sdTmrNum].wTmrState)
        return TIMER_OPEN_ALREADY; 

    if(TIMER_OPEN_YES != TmrMgrTbl[sdTmrNum].wTmrState)
        return TIMER_NO_CREATE;

    TmrMgrTbl[sdTmrNum].byTmrMod = byMod;
    TmrMgrTbl[sdTmrNum].pTimerAtnEntry = pTmrAtnEntry;
    TmrMgrTbl[sdTmrNum].pvAtnEntryArg = pvArg;
    g_dwTimeTickCout;
    //(dwTick + wTime/100) % ONE_DAY_TIME_TICK
	TmrMgrTbl[sdTmrNum].dwTime = dwTime/100;
	TmrMgrTbl[sdTmrNum].dwTick =  (g_dwTimeTickCout + dwTime/100)%ONE_DAY_TIME_TICK;
	TmrMgrTbl[sdTmrNum].dwTmDiv = TmrMgrTbl[sdTmrNum].dwTick+5;    /*¶šÊ±Æ÷Ÿ«¶ÈÏÖÔÚ¹æ¶šŽóÓÚ200ms*/ 
	TmrMgrTbl[sdTmrNum].wTmrState = TIMER_OPEN_ALREADY;
	return TIMER_NO_ERR;
}


unsigned short TmrStop(signed long sdTmrNum)

{

	if((0 > sdTmrNum) && (sdTmrNum > TMRMODMGRTBLSIZE))
		return TIMER_ARG_ERR;

	if(0 == TmrMgrTbl[sdTmrNum].wTmrState)
		return TIMER_NO_CREATE;

	if(TIMER_OPEN_YES == TmrMgrTbl[sdTmrNum].wTmrState)
		return TIMER_CLOSED_ALREADY;

	if(TIMER_OPEN_ALREADY == TmrMgrTbl[sdTmrNum].wTmrState)
	{
		TmrMgrTbl[sdTmrNum].wTmrState = TIMER_OPEN_YES;
		return TIMER_NO_ERR;
	}

	return TIMER_ARG_ERR;
}



unsigned short bstTmrDestory(signed long sdTmrNum)

{
	unsigned long dwI = 0;

	if((0 > sdTmrNum) && (sdTmrNum > TMRMODMGRTBLSIZE))
		return TIMER_ARG_ERR;
	if(NULL ==TmrInfoMgrTblUse.pTmrMgrHead)
		return TIMER_ARG_ERR;

    pthread_mutex_lock(&TmrMutexVal);
    if(&TmrMgrTbl[sdTmrNum] == TmrInfoMgrTblUse.pTmrMgrHead)
        TmrInfoMgrTblUse.pTmrMgrHead = TmrMgrTbl[sdTmrNum].pTmrMgrNext;
    else
        TmrMgrTbl[sdTmrNum].pTmrMgrPrior->pTmrMgrNext  =  TmrMgrTbl[sdTmrNum].pTmrMgrNext;
    if(&TmrMgrTbl[sdTmrNum] ==TmrInfoMgrTblUse.pTmrMgrTail)
    {
        TmrInfoMgrTblUse.pTmrMgrTail = TmrMgrTbl[sdTmrNum].pTmrMgrPrior;
        TmrInfoMgrTblUse.pTmrMgrTail->pTmrMgrNext = NULL;
    }
    else
        TmrMgrTbl[sdTmrNum].pTmrMgrNext->pTmrMgrPrior = TmrMgrTbl[sdTmrNum].pTmrMgrPrior;

	TmrInfoMgrTblIdle.pTmrMgrTail->pTmrMgrNext = &TmrMgrTbl[sdTmrNum];
	TmrInfoMgrTblIdle.pTmrMgrTail = TmrInfoMgrTblIdle.pTmrMgrTail->pTmrMgrNext;
	pthread_mutex_unlock(&TmrMutexVal);
	TmrInfoMgrTblIdle.dwTmrCutNum++;
	TmrInfoMgrTblUse.dwTmrCutNum--;
	memset(&TmrMgrTbl[sdTmrNum],0,sizeof(TIMERMGRTABLE));
	return TIMER_NO_ERR;
}


void *bstTmrHandle(void *pvArg)
{

	unsigned short wDsTmrDivBaseNum = 100;/*20ms.*/
	PTIMERMGRTABLE pTimerLoop;

	signal(SIGALRM, (void *)SigHandler);
	TimerVal.it_interval.tv_sec = 0;/**/
	TimerVal.it_interval.tv_usec = dwDiv;/**/
	TimerVal.it_value.tv_sec = 0;
	TimerVal.it_value.tv_usec = dwDiv;/*ÉèÖÃÏµÍ³¶šÊ±»ù×ŒÊ±ŒäÎª20ms.*/
	setitimer(ITIMER_REAL, &TimerVal, NULL);/*¿ªÆôÏµÍ³»ù×Œ¶šÊ±Æ÷.*/


	FOREVER
	{
		pthread_mutex_lock(&Ticklock);
		pTimerLoop = TmrInfoMgrTblUse.pTmrMgrHead;
		pthread_mutex_lock(&TmrMutexVal);
		for(;NULL != pTimerLoop;pTimerLoop = pTimerLoop->pTmrMgrNext)
		{
			if(TIMER_OPEN_ALREADY == pTimerLoop->wTmrState)
			{
				if((g_dwTimeTickCout > pTimerLoop->dwTick)&&(g_dwTimeTickCout < pTimerLoop->dwTmDiv))   
				{
					printf("into timer up check ... run handle.\n");
					if(NULL == (pTimerLoop->pTimerAtnEntry)) 
						continue;
					pTimerLoop->pTimerAtnEntry(pTimerLoop->pvAtnEntryArg);
					printf("\ntimerout handle func stopped.\n");
					if(TMRMOD_ONCE == pTimerLoop->byTmrMod)
	    				pTimerLoop->wTmrState = TIMER_OPEN_YES;
					else
					{
	    				pTimerLoop->dwTick =  (g_dwTimeTickCout + pTimerLoop->dwTime)%ONE_DAY_TIME_TICK;
	    				pTimerLoop->dwTmDiv = pTimerLoop->dwTick+5;
					}
				}
			}
		}

		pthread_mutex_unlock(&TmrMutexVal);
	}
}



