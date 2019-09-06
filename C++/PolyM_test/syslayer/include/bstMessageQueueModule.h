/***************************************************************
文件名:	bstMessageQueueModule.h
说明: 	消息队列模块的头文件，为该模块内部使用，不对外开放。
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/
#ifndef _BSTMESSAGEQUEUEMODULE_H
#define _BSTMESSAGEQUEUEMODULE_H

#define MESSAGEQUEUEMGT_MAX 65535
#define MESSAGEQUEUE_START 1
#define MESSAGEQUEUEMGT_START 1
#define MESSAGEQUEUE_OK 1
#define MESSAGEQUEUE_ERR -1
#define MESSAGEQUEUE_MNAME_LEN  128

typedef struct bstMessageQueue
{
	int messageIndex;
	void *pMessage;
	struct bstMessageQueue *pNext;
}BSTMESSAGEQUEUE,
*PBSTMESSAGEQUEUE,
**PPBSTMESSAGEQUEUE;

typedef struct bstMessageQueueModuleMgt
{	
	char mName[MESSAGEQUEUE_MNAME_LEN];//Module name.
	unsigned int bstMessageQueueId; //message queue id number.
	pthread_mutex_t msgSyncLockValue;//message queue operate sync lock.
	//message queue area.
	PBSTMESSAGEQUEUE pMessageQueueHead;
	PBSTMESSAGEQUEUE pMessageQueueRead;
	PBSTMESSAGEQUEUE pMessageQueueWrite;
	struct bstMessageQueueModuleMgt *pNext;//pointer to next node.
}BSTMESSAGEQUEUEMGT,
*PBSTMESSAGEQUEUEMGT,
**PPBSTMESSAGEQUEUEMGT;
#endif //_BSTMESSAGEQUEUEMODULE_H
