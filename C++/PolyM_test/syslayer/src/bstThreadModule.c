/***************************************************************
文件名:	bstThreadModule.c
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
#include "bstSystemLayer.h"



typedef struct{
	unsigned short		woMutexState;
	pthread_mutex_t		*pMutexVal;
	unsigned short  	woSyncState;
	pthread_cond_t  	CondFlagCv;
	pthread_mutex_t  	CondFlagMutex;
}SYNCMUTEXMGRTABLE, *PSYNCMUTEXMGRTABLE;

typedef struct pthdmgr{
	pthread_t 		ThreadId;
	unsigned char	byPthreadPri;
	pthread_attr_t	Attr;
	struct sched_param 	pAram;
	PTHREADFUN 		pThreadFun;
	void		*pvPthdFunArg;
	signed char 		chModName[10];
	signed char 		chThreadCtime[10];
	unsigned short		woPthreadDelayTime;
	SYNCMUTEXMGRTABLE	tHreadSyncMutexTbl;
	unsigned short		woRunErrInfoCode;
	struct pthdmgr 	*pMgrPrior;
	struct pthdmgr 	*pMgrNext;
}PTHREADMGRTABLE, *PPTHREADMGRTABLE;

typedef struct
{
	unsigned int 		dwPthdCtNum;
	PPTHREADMGRTABLE	pMgrPriorHead;
	PPTHREADMGRTABLE	pMgrNextHead;
}PTHREADINFOMGR, *PPTHREADINFOMGR;


PPTHREADMGRTABLE pPthdMgrTblGbl = NULL;
//pthread_t pThreadPthdMgrId = 0;

/*Mgr table pend.*/
pthread_mutex_t PthdMutexVal = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t CritcalRegionMutexVal = PTHREAD_MUTEX_INITIALIZER;
static PTHREADINFOMGR pThdInfoMgrTbl = 
{
	0,
	NULL,
	NULL,
}; 


static unsigned short PthdAttrInit(pthread_attr_t *pAttr, struct sched_param *pParam, unsigned char byPri)
{
	signed short swErrAttr;
	signed short swRs;

	swErrAttr = pthread_attr_init(pAttr);
	if(0 != swErrAttr)
	{
		printf("Init pthread attr Failed.\n");
		return FALSE;
	}

	/*Set sched is realtime module.*/
	swRs = pthread_attr_setschedpolicy(pAttr, SCHED_FIFO);	
	/*Set pthreads priority.*/
	pParam->__sched_priority = byPri; 
	pthread_attr_setschedparam(pAttr, pParam);
	swRs = pthread_attr_getschedparam(pAttr, pParam);
	return TRUE;
}


static unsigned short PthdMgrNodeInsert(PPTHREADMGRTABLE *pMoveNode)
{
	PPTHREADMGRTABLE pMove = NULL;/*¹ÜÀí±íÒÆ¶¯ÖžÕë.*/
	PPTHREADMGRTABLE pPthdMgrInsert = NULL;/*¹ÜÀí±í²åÈë±íÏîÖžÕë.*/

	if(NULL == pMoveNode)
		return PTHD_ARG_ERR;
	
	pPthdMgrInsert = (PPTHREADMGRTABLE)malloc(sizeof(PTHREADMGRTABLE));
	if(NULL == pPthdMgrInsert)
		return PTHD_MGR_NODEINSERT_ERR;

	pMove = pPthdMgrTblGbl;
	if(NULL == (pMove->pMgrNext))
		return PTHD_MGR_NODEINSERT_ERR;

	pthread_mutex_lock(&PthdMutexVal);
	pMove = pMove->pMgrNext;
	pPthdMgrInsert->pMgrPrior = pMove->pMgrPrior;
	pMove->pMgrPrior->pMgrNext = pPthdMgrInsert;
	pPthdMgrInsert->pMgrNext = pMove;
	pMove->pMgrPrior = pPthdMgrInsert;
	*pMoveNode = pPthdMgrInsert;
	pThdInfoMgrTbl.dwPthdCtNum += 1;
	pthread_mutex_unlock(&PthdMutexVal);
	return PTHD_MGRINIT_OK;
}

static unsigned short PthdMgrNodeDelete(pthread_t cPid)
{

	PPTHREADMGRTABLE pMovePrior = NULL;
	PPTHREADMGRTABLE pMoveNext = NULL;
	unsigned int dwI = 0;

	pMovePrior = pPthdMgrTblGbl;
	pMoveNext = pPthdMgrTblGbl;
	for(; (pMovePrior != NULL), (pMoveNext != NULL);  (pMovePrior=pMovePrior->pMgrPrior), 
		(pMoveNext=pMoveNext->pMgrNext), dwI++)
	{
		if(cPid == (pMovePrior->ThreadId))
		{
			pthread_mutex_lock(&PthdMutexVal);
			pMovePrior->pMgrPrior->pMgrNext = pMovePrior->pMgrNext;
			pMovePrior->pMgrNext->pMgrPrior = pMovePrior->pMgrPrior;
			free(pMovePrior);
			pThdInfoMgrTbl.dwPthdCtNum -= 1;
			pthread_mutex_unlock(&PthdMutexVal);
			break;
		}
		else if(cPid == (pMoveNext->ThreadId))
		{
			pthread_mutex_lock(&PthdMutexVal);
			pMoveNext->pMgrPrior->pMgrNext = pMoveNext->pMgrNext;
			pMoveNext->pMgrNext->pMgrPrior = pMoveNext->pMgrPrior;
			free(pMoveNext);
			pThdInfoMgrTbl.dwPthdCtNum -= 1;
			pthread_mutex_unlock(&PthdMutexVal);
			break;
		}	

		if(((pThdInfoMgrTbl.dwPthdCtNum)/2 + 1) == dwI)
			return PTHD_MGR_NODEDELETE_ERR;
	}

	return PTHD_MGRNO_ERR;
}

static unsigned short PthdMgrNodeFind(pthread_t cPid, PPTHREADMGRTABLE *pMove)
{
	PPTHREADMGRTABLE pMovePrior = NULL;
	PPTHREADMGRTABLE pMoveNext = NULL;
	unsigned int dwI = 0;
	
	pMovePrior = pPthdMgrTblGbl;
	pMoveNext = pPthdMgrTblGbl;
	for(; (pMovePrior != NULL), (pMoveNext != NULL);  (pMovePrior=pMovePrior->pMgrPrior), 
		(pMoveNext=pMoveNext->pMgrNext), dwI++)
	{
		if(cPid == (pMovePrior->ThreadId))
		{
			pthread_mutex_lock(&PthdMutexVal);
			*pMove = pMovePrior;
			pthread_mutex_unlock(&PthdMutexVal);
			break;
		}
		else if(cPid == (pMoveNext->ThreadId))
		{
			pthread_mutex_lock(&PthdMutexVal);
			*pMove = pMoveNext;
			pthread_mutex_unlock(&PthdMutexVal);
			break;
		}	

		if(((pThdInfoMgrTbl.dwPthdCtNum)/2 + 1) == dwI)
			return PTHD_MGR_NODE_NOEXIST;
	}

	return PTHD_MGRNO_ERR;
}


unsigned short bstPthdMgtTableInit(PPTHREADMGRTABLE *pPthreadMgrTable)
{

	PPTHREADMGRTABLE pMove = NULL;
	PPTHREADMGRTABLE pPthdMgrInHead = NULL;

	if(NULL == pPthreadMgrTable)
		return PTHD_ARG_ERR;

	pPthdMgrInHead = (PPTHREADMGRTABLE)malloc(sizeof(PTHREADMGRTABLE));
	if(NULL == pPthdMgrInHead)
		return PTHD_MGRINIT_ERR;

	pMove = (PPTHREADMGRTABLE)malloc(sizeof(PTHREADMGRTABLE));
	if(NULL == pMove)
		return PTHD_MGRINIT_ERR;

	pthread_mutex_lock(&PthdMutexVal);
	memset((unsigned char *)pPthdMgrInHead, 0x0, sizeof(PTHREADMGRTABLE));
	memset((unsigned char *)pMove, 0x0, sizeof(PTHREADMGRTABLE));
	pThdInfoMgrTbl.dwPthdCtNum = 2;
	pThdInfoMgrTbl.pMgrPriorHead = (PPTHREADMGRTABLE)pPthdMgrInHead;
	pThdInfoMgrTbl.pMgrNextHead = (PPTHREADMGRTABLE)pPthdMgrInHead;
	*pPthreadMgrTable = (PPTHREADMGRTABLE)(pThdInfoMgrTbl.pMgrNextHead);
	pPthdMgrInHead->pMgrNext = pMove;
	pMove->pMgrNext = pPthdMgrInHead;
	pMove->pMgrPrior = pPthdMgrInHead;
	pPthdMgrInHead->pMgrPrior = pMove;
	pthread_mutex_unlock(&PthdMutexVal);
	return PTHD_MGRINIT_OK;

}

unsigned short bstMutexMgtTblInit(PSYNCMUTEXMGRTABLE pSyncMutexMgrTable)
{

	if(NULL == pSyncMutexMgrTable)
		return PTHD_ARG_ERR;

	pSyncMutexMgrTable->pMutexVal = (pthread_mutex_t *)&CritcalRegionMutexVal;
	pSyncMutexMgrTable->woMutexState = CRITICAL_REGION_UNLOCK;
	pthread_mutex_init(&(pSyncMutexMgrTable->CondFlagMutex), NULL);
	pthread_cond_init(&(pSyncMutexMgrTable->CondFlagCv), NULL);
	pSyncMutexMgrTable->woSyncState = PTHD_SYNC_UNLOCK;
	return PTHD_MGRNO_ERR;
}

pthread_t bstThreadCreate(PTHREADFUN pThreadFun, void *pvPthdFunArg, signed char *pchModName, unsigned char byPthreadPri)
{

	signed short swCtState = 0;
	PPTHREADMGRTABLE pMove = NULL;

	if((NULL==pThreadFun) && (NULL==pchModName) && (0 < byPthreadPri))
		return PTHD_ARG_ERR;

	pthread_mutex_lock(&PthdMutexVal);
	pMove = pPthdMgrTblGbl;	
	pthread_mutex_unlock(&PthdMutexVal);
	if((NULL == pMove->pThreadFun) && (NULL == pMove->pvPthdFunArg))
	{
		if(FALSE == PthdAttrInit(&(pMove->Attr), &(pMove->pAram), byPthreadPri))
			return PTHD_CREATE_FAIL;

		swCtState = pthread_create(&(pMove->ThreadId), &(pMove->Attr), (PTHREADFUN)pThreadFun, pvPthdFunArg);
		if(0 != swCtState)
			return PTHD_CREATE_FAIL;

		pthread_mutex_lock(&PthdMutexVal);
		pMove->byPthreadPri = byPthreadPri;
		pMove->pThreadFun = pThreadFun;
		pMove->pvPthdFunArg = pvPthdFunArg;
		strcpy((signed char *)pMove->chModName, (signed char *)pchModName);
		pMove->woPthreadDelayTime = 0;
		SyncMutexMgrTblInit((PSYNCMUTEXMGRTABLE)&(pMove->tHreadSyncMutexTbl));
		pMove->woRunErrInfoCode = PTHD_NO_ERR;
		pthread_mutex_unlock(&PthdMutexVal);
		return (pMove->ThreadId);
	}

	pthread_mutex_lock(&PthdMutexVal);
	pMove = pMove->pMgrNext;
	pthread_mutex_unlock(&PthdMutexVal);
	if((NULL == pMove->pThreadFun) && (NULL == pMove->pvPthdFunArg))
	{
		if(FALSE == PthdAttrInit(&(pMove->Attr), &(pMove->pAram), byPthreadPri))
			return PTHD_CREATE_FAIL;

		swCtState = pthread_create(&(pMove->ThreadId), &(pMove->Attr), (PTHREADFUN)pThreadFun, pvPthdFunArg);
		if(0 != swCtState)
			return PTHD_CREATE_FAIL;

		pthread_mutex_lock(&PthdMutexVal);
		pMove->byPthreadPri = byPthreadPri;
		pMove->pThreadFun = pThreadFun;
		pMove->pvPthdFunArg = pvPthdFunArg;
		strcpy((signed char *)pMove->chModName, (signed char *)pchModName);
		pMove->woPthreadDelayTime = 0;
		SyncMutexMgrTblInit((PSYNCMUTEXMGRTABLE)&(pMove->tHreadSyncMutexTbl));
		pMove->woRunErrInfoCode = PTHD_NO_ERR;
		pthread_mutex_unlock(&PthdMutexVal);	
		return (pMove->ThreadId);
	}

	if(PTHD_MGR_NODEINSERT_ERR == PthdMgrNodeInsert((PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGRCREATE_ERR;

	if(FALSE == PthdAttrInit(&(pMove->Attr), &(pMove->pAram), byPthreadPri))
		return PTHD_CREATE_FAIL;

	swCtState = pthread_create(&(pMove->ThreadId), &(pMove->Attr), (PTHREADFUN)pThreadFun, pvPthdFunArg);
	if(0 != swCtState)
		return PTHD_CREATE_FAIL;

	pthread_mutex_lock(&PthdMutexVal);
	pMove->byPthreadPri = byPthreadPri;
	pMove->pThreadFun = pThreadFun;
	pMove->pvPthdFunArg = pvPthdFunArg;
	strcpy((signed char *)pMove->chModName, (signed char *)pchModName);
	pMove->woPthreadDelayTime = 0;
	SyncMutexMgrTblInit((PSYNCMUTEXMGRTABLE)&(pMove->tHreadSyncMutexTbl));
	pMove->woRunErrInfoCode = PTHD_NO_ERR;
	pthread_mutex_unlock(&PthdMutexVal);	
	return (pMove->ThreadId);
}

pthread_t bstThreadIdGet(void)
{

	pthread_t pTid;
	PPTHREADMGRTABLE pMove = NULL;
	pTid = pthread_self();
	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(pTid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	return pTid;
}

unsigned short bstThreadDelay(pthread_t cPid, unsigned short wTime)
{

	PPTHREADMGRTABLE pMove = NULL;

	if((0 > cPid) && (0 > wTime))
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pMove->woPthreadDelayTime = wTime;
	sleep(wTime);
	pMove->woPthreadDelayTime = 0;
	return PTHD_MGRNO_ERR;
}

unsigned short bstThreadQuit(pthread_t cPid)
{
	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODEDELETE_ERR == PthdMgrNodeDelete(cPid))
		return PTHD_MGR_NODEDELETE_ERR;

	pthread_exit((void *)0);
	return PTHD_MGRNO_ERR;
}

unsigned short bstThreadWait(pthread_t oPid)
{

	void *pvOverstate;
	PPTHREADMGRTABLE pMove = NULL;
	
	if(0 > oPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(oPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	if(0 != pthread_join(oPid, pvOverstate))
		return FALSE;

	return PTHD_MGRNO_ERR;
}

unsigned short bstCriticalRegionUnLock(pthread_t cPid, unsigned short wLockState)
{

	PPTHREADMGRTABLE pMove = NULL;
	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pMove->tHreadSyncMutexTbl.woMutexState = wLockState;
	pthread_mutex_unlock(pMove->tHreadSyncMutexTbl.pMutexVal);
	return PTHD_MGRNO_ERR;

}

unsigned short bstCriticalRegionLock(pthread_t cPid, unsigned short wLockState)
{

	PPTHREADMGRTABLE pMove = NULL;
	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pMove->tHreadSyncMutexTbl.woMutexState = wLockState;
	pthread_mutex_lock(pMove->tHreadSyncMutexTbl.pMutexVal);
	return PTHD_MGRNO_ERR;
}

unsigned short bstCondUnLock(pthread_t cPid, unsigned short wLockState)
{

	PPTHREADMGRTABLE pMove = NULL;

	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pMove->tHreadSyncMutexTbl.woSyncState= wLockState;
	pthread_mutex_unlock(&(pMove->tHreadSyncMutexTbl.CondFlagMutex));
	return PTHD_MGRNO_ERR;
}

unsigned short bstCondLock(pthread_t cPid, unsigned short wLockState)
{

	PPTHREADMGRTABLE pMove = NULL;
	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pMove->tHreadSyncMutexTbl.woSyncState= wLockState;
	pthread_mutex_lock(&(pMove->tHreadSyncMutexTbl.CondFlagMutex));
	return PTHD_MGRNO_ERR;
}

unsigned short bstCondWait(signed short swCondVal, pthread_t cPid)
{

	PPTHREADMGRTABLE pMove = NULL;

	if(0 > cPid) 
		return PTHD_ARG_ERR;

	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pthread_cond_wait(&(pMove->tHreadSyncMutexTbl.CondFlagCv), &(pMove->tHreadSyncMutexTbl.CondFlagMutex));
	return PTHD_MGRNO_ERR;
}

unsigned short bstCondSignal(signed short swCondVal, pthread_t cPid)
{

	PPTHREADMGRTABLE pMove = NULL;

	if(0 > cPid) 
		return PTHD_ARG_ERR;
	
	if(PTHD_MGR_NODE_NOEXIST == PthdMgrNodeFind(cPid, (PPTHREADMGRTABLE *)&pMove))
		return PTHD_MGR_NODE_NOEXIST;

	pthread_cond_signal (&(pMove->tHreadSyncMutexTbl.CondFlagCv));
	return PTHD_MGRNO_ERR;
}
