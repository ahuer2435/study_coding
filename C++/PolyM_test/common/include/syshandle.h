/***************************************************************
文件名:	syshandle.h
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/
#ifndef _BSTSYSHANDLE_H
#define _BSTSYSHANDLE_H

#define BSTHANDLEOK 1
#define BSTHANDLEERR -1;

//system layer handle funcs description.
extern void *bstAudioPlay(const char* bstAudioFile);
#endif //_BSTSYSHANDLE_H

