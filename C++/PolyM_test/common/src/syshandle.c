/***************************************************************
文件名:	syshandle.c
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

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

#include "syshandle.h"

void *bstAudioPlay(const char* bstAudioFile)
{
	char command[300];
	
	if(NULL == bstAudioFile)
		return BSTHANDLEERR;

	memset(command, 0, sizeof(command));
	sprintf(command, "aplay %s", bstAudioFile);
	system(command);
	return BSTHANDLEOK;
}

