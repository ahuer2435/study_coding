#include<bstLog.h>

src::severity_channel_logger<severity_level,std::string> InfoLoger(keywords::channel = "LogSink");
src::severity_channel_logger<severity_level,std::string> PerfLoger(keywords::channel = "PerfLogSink");
