/***************************************************************
文件名:	common.h
说明: 	
作者:		王磊 jack.wang@bst.ai
日期:		2019/01/04
历史修改: 	无
--------------------------------------------------------

***/
#ifndef _BSTCOMMON_H
#define _BSTCOMMON_H

//module name definition.
#define ADASBOX_SYSMODNAME		"SYSLAYER"
#define ADASBOX_ADASMODNAME 	"ADAS_MOD01"
#define ADASBOX_DMSMODNAME 		"DMS_MOD01"


//module name definition of  system layer sub-system module.
#define TMRMODNAME		"SYSLAYER-TIMER"
#define THDMODNAME		"SYSLAYER-THDMGR"

//main cycle definition.
#define BSTFOREVER for(;;)

//adas, dms main thread callback func description.
extern void *bstAdasMainHandle(void* bstArg);
extern void *bstDmsMainHandle(void* bstArg);
#endif //_BSTCOMMON_H

