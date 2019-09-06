#include <pthread.h>
#include <queue>
#include <memory>
#include "CommonParam.h"

bool  BE_QUIT       =  false;
bool  bRecordDataTurn = false;
double   TimeStampDiffThreash = 3000.0;
bstMsgQueue<ADAS_PERCEPTION_INFOPack>  MsgQueueAdasPerceptionData("AdasPerceptionQueue");

