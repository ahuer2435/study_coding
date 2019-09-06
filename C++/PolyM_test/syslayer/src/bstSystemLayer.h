/***************************************************************
文件名:	bstSystemLayer.h
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/



#ifndef _BSTSYSTEMLAYER_H
#define _BSTSYSTEMLAYER_H

//thead return value definition
typedef enum
{
	PTHD_PENDDING =0x1,
	PTHD_NOPEND,
}PEND_STATE;

typedef enum
{
	PTHD_SYNC_UNLOCK = 0x3,
	PTHD_SYNC_LOCKED,
}SYNC_LOCK_STATE;

typedef enum
{
	CRITICAL_REGION_UNLOCK = 0x5,
	CRITICAL_REGION_LOCKED,
}CRIRESOURCE_LOCK_STATE;

typedef enum
{
	PTHD_NO_ERR =0x8000,
	PTHD_ARG_ERR,
	PTHD_MEMACCESS_ERR,
	PTHD_CREATE_FAIL = -1, 
	PTHD_DELETE_OK = 0x8002, 
	PTHD_NO_EXIST,
	PTHD_NOW_RUNNING,
	PTHD_DELAY_OK,
	PTHD_PEND_OK,
	PTHD_RESUME_OK,
	PTHD_QUIT_OK,
	PTHD_WAIT_OK,
}PTHDRUN_STATE;


typedef enum
{
	PTHD_MGRNO_ERR = 0x90,
	PTHD_MGRINIT_OK,
	PTHD_MGRINIT_ERR = 0x96,
	PTHD_MGR_NODEINSERT_ERR,
	PTHD_MGR_NODEDELETE_ERR,
	PTHD_MGRCREATE_ERR,
	PTHD_MGRDELETE_ERR,
	PTHD_MGR_NODE_NOEXIST, 
}PTHDMGR_STATE;

//timer return value definition
typedef enum
{
	TIMER_NO_ERR = 0x5000,
	TIMER_ARG_ERR,
	TIMER_OPEN_ALREADY,
	TIMER_OPEN_YES,
	TIMER_CLOSED_ALREADY,
	TIMER_INIT_FAIL,
	TIMER_INSERT_FAIL,
	TIMER_DELETE_FAIL,
	TIMER_NO_CREATE,
	TIMER_CREAT_FAIL = -10,
	TIMER_NUM_NOFOUND = 0x5006,
	TIMER_STOP_ERR,
	TIMER_DESTORY_ERR,
}TIMER_ERRSTATE;


//type definition of thread callback funcs.
typedef	 VOID *(*PTHREADFUN)(VOID *);
//type definition of  timer callback funcs.
typedef void (* TIMERATNENTRY)(void *);


extern PPTHREADMGRTABLE pPthdMgrTblGbl;
extern unsigned short bstPthdMgtTableInit(PPTHREADMGRTABLE *pPthreadMgrTable);
extern unsigned short bstMutexMgtTblInit(PSYNCMUTEXMGRTABLE pSyncMutexMgrTable);
extern unsigned short bstTmrMgrInit(void);
extern pthread_t bstThreadCreate(PTHREADFUN pThreadFun, void *vpPthdFunArg, signed char *pchModName, unsigned char byPthreadPri);
extern void *bstTmrHandle(void *pvArg);

#endif //_BSTSYSTEMLAYER_H

