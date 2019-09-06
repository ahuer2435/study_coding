#ifndef COMMONPARAM_H
#define COMMONPARAM_H
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <climits>
#include <memory>
#include <deque>
#include <string>
#include <iostream>
#include "Msg.hpp"
#include "Queue.hpp"
#include "bstMsgQueue.h"
#include <unistd.h>

extern bool  BE_QUIT;
extern bool  bRecordDataTurn;
extern double   TimeStampDiffThreash;
#define Param(para) #para << ":" << para << ","

struct  ADAS_PERCEPTION_INFOPack
{
    int width;
    int height;
    double fps;
    cv::Mat frame;
};

extern bstMsgQueue<ADAS_PERCEPTION_INFOPack>  MsgQueueAdasPerceptionData;
#endif