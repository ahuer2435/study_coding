
#ifndef BST_LOG_H
#define BST_LOG_H
#define BOOST_LOG_DYN_LINK 1
#include <iostream>
#include<boost/log/core.hpp>
#include<boost/log/expressions.hpp>
#include<boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/settings.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <cstdio>
#include <iomanip>
#include <string>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
using namespace boost::log::trivial;

extern boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level,std::string> InfoLoger;
extern boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level,std::string> PerfLoger;

static std::string suffix=   "_%Y-%m-%d_%H-%M-%S.%N.log";
//namespace expr = boost::log::Expressions;
#define LOGFILE(name,loglevel)\
  boost::log::add_file_log\
   (\
      boost::log::keywords::file_name = name+suffix,\
      boost::log::keywords::rotation_size = 10*1024*1024,\
      boost::log::keywords::auto_flush = true,\
      boost::log::keywords::format =\
      (boost::log::expressions::stream <<"["\
      << boost::log::trivial::severity\
      <<"  "<<boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S")\
      <<"  "<<boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")<<"]"\
      <<"  "<<boost::log::expressions::smessage\
      )\
    );               \
    boost::log::trivial::severity_level level = boost::log::trivial::severity_level(loglevel);\
    LogInfo <<"level" <<level ;\
    boost::log::core::get()->set_filter(boost::log::trivial::severity>= level);\
    boost::log::add_common_attributes();\
    boost::log::core::get()->add_global_attribute("ThreadID",boost::log::attributes::current_thread_id());\
    boost::log::core::get()->add_thread_attribute("Scope",boost::log::attributes::named_scope())


#define InitLogger() \
  logging::add_common_attributes();\
  logging::register_simple_formatter_factory<severity_level,char>("Severity");\
  logging::register_simple_filter_factory<severity_level,char>("Severity");\
  std::ifstream file("../AppData/Logconfig.ini");\
  logging::init_from_stream(file)

#define LogInfo  BOOST_LOG_SEV(InfoLoger,info) << "["<<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ << "]"<<"     "

#define LogTrace BOOST_LOG_SEV(InfoLoger,trace) <<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"     "

#define LogDebug BOOST_LOG_SEV(InfoLoger,debug)<<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "

#define LogError BOOST_LOG_SEV(InfoLoger,error)<<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "

#define LogWarning BOOST_LOG_SEV(InfoLoger,warning)<<"["<< __FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "


#define PerfLogInfo  BOOST_LOG_SEV(PerfLoger,info) << "["<<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ << "]"<<"     "

#define PerfLogTrace BOOST_LOG_SEV(PerfLoger,trace) <<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"     "

#define PerfLogDebug BOOST_LOG_SEV(PerfLoger,debug)<<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "

#define PerfLogError BOOST_LOG_SEV(PerfLoger,error)<<"[" <<__FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "

#define PerfLogWarning BOOST_LOG_SEV(PerfLoger,warning)<<"["<< __FILE__ <<":" << __FUNCTION__<<"():" << __LINE__ <<"]"<<"      "



#endif