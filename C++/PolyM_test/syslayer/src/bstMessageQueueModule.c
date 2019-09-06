/***************************************************************
文件名:	bstMessageQueueModule.c
说明: 	消息队列模块，为各个模块线程提供通讯机制。
		该模块消息具备阻塞机制，即接收消息遇到空时阻塞接收消息线程，
		非空时唤醒线程。
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
---------------------------------------------------------

***/

//linux head files
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
//ansi c head files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//module head file.
#include "common.h"
#include "bstSystemLayer.h"

#include "bstMessageQueueModule.h"


//bst message queue pointer.
//PBSTMESSAGEQUEUE pMessageQueueHead;
//PBSTMESSAGEQUEUE pMessageQueueRead, pMessageQueueWrite;

//bst message queue management table pointer
PBSTMESSAGEQUEUEMGT pMessageQueueMgtHead;
PBSTMESSAGEQUEUEMGT pMessageQueueMgtInto;


static int bstMessageQueueInit(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode)
{
	//pthread_mutex_lock(&canMutexLockValue);
	(*ppNewMessageQueueMgtNode)->pMessageQueueHead =(PBSTMESSAGEQUEUE)malloc(sizeof(BSTMESSAGEQUEUE));
	if (NULL == (*ppNewMessageQueueMgtNode)->pMessageQueueHead)
		return MESSAGEQUEUE_ERR;

	(*ppNewMessageQueueMgtNode)->pMessageQueueRead = (*ppNewMessageQueueMgtNode)->pMessageQueueHead;
	(*ppNewMessageQueueMgtNode)->pMessageQueueWrite = (*ppNewMessageQueueMgtNode)->pMessageQueueHead;
	(*ppNewMessageQueueMgtNode)->pMessageQueueWrite->pNext = NULL;	
	return MESSAGEQUEUE_ERR;
}

static int bstMessageQueueInsert(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode,PPBSTMESSAGEQUEUE ppMessageQueueNode)
{
	//flag variable.
	static int MessageQueueFlag=MESSAGEQUEUE_START;
	
	if(MESSAGEQUEUE_START == MessageQueueFlag)
	{	 	
		*ppMessageQueueNode = (*ppNewMessageQueueMgtNode)->pMessageQueueHead;
		(*ppMessageQueueNode)->messageIndex = MESSAGEQUEUE_START;
		MessageQueueFlag += MESSAGEQUEUE_START;		
		return MESSAGEQUEUE_OK;
	}

	*ppMessageQueueNode=(PBSTMESSAGEQUEUE)malloc(sizeof(BSTMESSAGEQUEUE));
	if (NULL == (*ppMessageQueueNode))
		return MESSAGEQUEUE_ERR;//allocate failed.
	
	(*ppNewMessageQueueMgtNode)->pMessageQueueWrite->pNext = *ppMessageQueueNode;
	(*ppNewMessageQueueMgtNode)->pMessageQueueWrite = ((*ppNewMessageQueueMgtNode)->pMessageQueueWrite)->pNext;
	((*ppNewMessageQueueMgtNode)->pMessageQueueWrite)->messageIndex = MessageQueueFlag;
	MessageQueueFlag++;	
	return MESSAGEQUEUE_OK;
}



static int bstMessageQueueDelete(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode, PPBSTMESSAGEQUEUE ppMessageQueueNode)
{

	if((*ppNewMessageQueueMgtNode)->pMessageQueueWrite == (*ppNewMessageQueueMgtNode)->pMessageQueueRead)
	{
		*ppMessageQueueNode = NULL;		
		return MESSAGEQUEUE_ERR;//llbuffer was empty.
	}

	*ppMessageQueueNode = (*ppNewMessageQueueMgtNode)->pMessageQueueRead;
	(*ppNewMessageQueueMgtNode)->pMessageQueueRead = ((*ppNewMessageQueueMgtNode)->pMessageQueueRead)->pNext;
	return MESSAGEQUEUE_OK;	
}


static int bstMessageQueueAdd(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode, void *bstMessage)
{
	int ret;
	PBSTMESSAGEQUEUE pMessageQueueNode;
	
	ret = bstMessageQueueInsert(ppNewMessageQueueMgtNode, (PPBSTMESSAGEQUEUE)&pMessageQueueNode);
	if(MESSAGEQUEUE_ERR == ret)
		return MESSAGEQUEUE_ERR;

	//writing the message data.
	pMessageQueueNode->pMessage =  bstMessage;
	return MESSAGEQUEUE_OK;
}


int bstMessageQueueQuit(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode, void **bstMessage)
{
	int ret;
	PBSTMESSAGEQUEUE pMessageQueueNode;

	ret = bstMessageQueueDelete(ppNewMessageQueueMgtNode, (PPBSTMESSAGEQUEUE)&pMessageQueueNode);
	if(MESSAGEQUEUE_ERR == ret)
		return MESSAGEQUEUE_ERR;

	*bstMessage = pMessageQueueNode->pMessage
	free(pMessageQueueNode);
	return MESSAGEQUEUE_OK;
}



/***************************************************************
函数名称:	bstMessageQueueMgtInit
功能说明: 	消息队列模块管理表初始化函数。
形式参数:	无
返回值:	正确返回1，错误返回-1;
历史修改: 	无
----------------------------------------------------------

***/
int bstMessageQueueMgtInit(void)
{	
	pMessageQueueMgtHead=(PBSTMESSAGEQUEUEMGT)malloc(sizeof(BSTMESSAGEQUEUEMGT));
	if (NULL == pMessageQueueMgtHead)
		return MESSAGEQUEUE_ERR;

	pMessageQueueMgtInto = pMessageQueueMgtHead;
	pMessageQueueMgtInto->pNext=NULL;	
	return MESSAGEQUEUE_OK;
}

static int bstMessageQueueMgtInsert(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode, char *pModuleName)
{
	static unsigned int MessageQueueMgtFlag = MESSAGEQUEUEMGT_START;
	if(MESSAGEQUEUEMGT_START == MessageQueueMgtFlag)
	{	 	
		*ppNewMessageQueueMgtNode = pMessageQueueMgtHead;
		(*ppNewMessageQueueMgtNode)->bstMessageQueueId = MESSAGEQUEUEMGT_START;
		memset((*ppNewMessageQueueMgtNode)->mName, 0, MESSAGEQUEUE_MNAME_LEN);
		strcpy((*ppNewMessageQueueMgtNode)->mName, pModuleName);
		(*ppNewMessageQueueMgtNode)->pNext = NULL;
		//message queue part initilize ...
		bstMessageQueueInit(ppNewMessageQueueMgtNode);
		MessageQueueMgtFlag += MESSAGEQUEUEMGT_START;		
		return MESSAGEQUEUE_OK;
	}

	*ppNewMessageQueueMgtNode=(PBSTMESSAGEQUEUEMGT)malloc(sizeof(BSTMESSAGEQUEUEMGT));
	if (NULL == (*ppNewMessageQueueMgtNode))
		return MESSAGEQUEUE_ERR;//allocate failed.
	pMessageQueueMgtInto->pNext = *ppNewMessageQueueMgtNode;
	pMessageQueueMgtInto = pMessageQueueMgtInto->pNext;
	pMessageQueueMgtInto->bstMessageQueueId = MessageQueueMgtFlag;
	memset(pMessageQueueMgtInto->mName, 0, MESSAGEQUEUE_MNAME_LEN);
	strcpy(pMessageQueueMgtInto->mName, pModuleName);
	pMessageQueueMgtInto->pNext = NULL;	
	bstMessageQueueInit(ppNewMessageQueueMgtNode);
	MessageQueueMgtFlag++;	
	if(MESSAGEQUEUEMGT_MAX == MessageQueueMgtFlag)
		return MESSAGEQUEUE_ERR;
	
	return MESSAGEQUEUE_OK;
}


static int bstMessageQueueMgtDelete(unsigned int bstMessageQueueId)
{
	PBSTMESSAGEQUEUEMGT pMessageQueueMgtTemp, pMessageQueueMgtS;
	pMessageQueueMgtTemp = pMessageQueueMgtHead;
		
	if(bstMessageQueueId == pMessageQueueMgtHead->bstMessageQueueId)
		return MESSAGEQUEUE_ERR; //message queue management table empty.

	for( ; NULL != pMessageQueueMgtTemp->pNext ; pMessageQueueMgtTemp = pMessageQueueMgtTemp->pNext)
	{
		if (bstMessageQueueId == (pMessageQueueMgtTemp->pNext)->bstMessageQueueId) 
		{
			pMessageQueueMgtS = pMessageQueueMgtTemp->pNext ;
			pMessageQueueMgtTemp->pNext = pMessageQueueMgtS ->pNext;
			free(pMessageQueueMgtS);
			return MESSAGEQUEUE_OK;
		}
	}

	return MESSAGEQUEUE_ERR;
}

static int bstMessageQueueMgtFind(PPBSTMESSAGEQUEUEMGT ppNewMessageQueueMgtNode, unsigned int bstMessageQueueId)
{
	PBSTMESSAGEQUEUEMGT pMessageQueueMgtTemp;
	pMessageQueueMgtTemp = pMessageQueueMgtHead;
		
	if(bstMessageQueueId == pMessageQueueMgtHead->bstMessageQueueId)
	{
		(*ppNewMessageQueueMgtNode) = pMessageQueueMgtHead;
		return MESSAGEQUEUE_OK; 
	}
	
	for( ; NULL != pMessageQueueMgtTemp->pNext ; pMessageQueueMgtTemp = pMessageQueueMgtTemp->pNext)
	{
		if (bstMessageQueueId == pMessageQueueMgtTemp->bstMessageQueueId) 
		{
			(*ppNewMessageQueueMgtNode) = pMessageQueueMgtTemp;
			return MESSAGEQUEUE_OK;
		}
	}

	return MESSAGEQUEUE_ERR;
}



int bstMessageQueueCreate(char *ModuleName)
{
	int ret;
	PBSTMESSAGEQUEUEMGT pNewMessageQueueMgtNode;
	ret = bstMessageQueueMgtInsert((PPBSTMESSAGEQUEUEMGT)&pNewMessageQueueMgtNode, ModuleName);
	if(MESSAGEQUEUE_ERR == ret)
	{
		//pthread_mutex_unlock(&canMutexLockValue);
		return MESSAGEQUEUE_ERR;
	}

	return pNewMessageQueueMgtNode->bstMessageQueueId;
}


int bstMessageQueueDestory(unsigned int bstMessageQueueId)
{
	int ret;
	PBSTMESSAGEQUEUEMGT pNewMessageQueueMgtNode;
	ret = bstMessageQueueMgtDelete(bstMessageQueueId);
	if(MESSAGEQUEUE_ERR == ret)
	{
		//pthread_mutex_unlock(&canMutexLockValue);
		return MESSAGEQUEUE_ERR;
	}

	return MESSAGEQUEUE_OK;
}

int bstMessageQueueSend(char *bstModuleName, unsigned int  bstMessageQueueId, void *bstMessage)
{
	int ret;
	PBSTMESSAGEQUEUEMGT pMessageQueueMgtNode;
	ret = bstMessageQueueMgtFind((PPBSTMESSAGEQUEUEMGT)&pMessageQueueMgtNode, bstMessageQueueId);
	if(MESSAGEQUEUE_ERR == ret)
	{
		//pthread_mutex_unlock(&canMutexLockValue);
		return MESSAGEQUEUE_ERR;
	}

	ret = strncmp(pMessageQueueMgtNode->mName, bstModuleName, strlen(bstModuleName));
	if (0 != ret)
		return MESSAGEQUEUE_ERR;

	ret = bstMessageQueueAdd((PPBSTMESSAGEQUEUEMGT)&pMessageQueueMgtNode, bstMessage);
	if(MESSAGEQUEUE_ERR == ret)
		return MESSAGEQUEUE_ERR;
	return MESSAGEQUEUE_OK;
}

int bstMessageQueueReceive(char *bstModuleName, unsigned int  bstMessageQueueId, void **bstMessage)
{
	int ret;
	PBSTMESSAGEQUEUEMGT pMessageQueueMgtNode;
	ret = bstMessageQueueMgtFind((PPBSTMESSAGEQUEUEMGT)&pMessageQueueMgtNode, bstMessageQueueId);
	if(MESSAGEQUEUE_ERR == ret)
	{
		//pthread_mutex_unlock(&canMutexLockValue);
		return MESSAGEQUEUE_ERR;
	}

	ret = strncmp(pMessageQueueMgtNode->mName, bstModuleName, strlen(bstModuleName));
	if (0 != ret)
		return MESSAGEQUEUE_ERR;

	ret = bstMessageQueueQuit((PPBSTMESSAGEQUEUEMGT)&pMessageQueueMgtNode, (PPBSTMESSAGEQUEUEMGT)&pMessageQueueMgtNode, bstMessage);
	if(MESSAGEQUEUE_ERR == ret)
		return MESSAGEQUEUE_ERR;
	return MESSAGEQUEUE_OK;
}

