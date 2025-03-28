#pragma once

#include "evpp/platform_config.h"

#ifdef __cplusplus
#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include <glog/logging.h>

#ifdef GOOGLE_STRIP_LOG

#ifdef GOOGLE_LOG_TO_SINK_ONLY

namespace evpp {
    void set_sink(google::LogSink* s);
    google::LogSink* sink();
}

#define GLOG_INFO LOG_TO_SINK_BUT_NOT_TO_LOGFILE(evpp::sink(), INFO)
#define GLOG_WARNING  LOG_TO_SINK_BUT_NOT_TO_LOGFILE(evpp::sink(), WARNING)
#define GLOG_ERROR LOG_TO_SINK_BUT_NOT_TO_LOGFILE(evpp::sink(), ERROR)
#define GLOG_FATAL LOG_TO_SINK_BUT_NOT_TO_LOGFILE(evpp::sink(), FATAL)

#else

#define GLOG_INFO LOG(INFO)
#define GLOG_WARNING LOG(WARNING)
#define GLOG_ERROR LOG(ERROR)
#define GLOG_FATAL LOG(FATAL)

#endif

#if GOOGLE_STRIP_LOG == 0
#define LOG_TRACE GLOG_INFO
#define LOG_DEBUG GLOG_INFO
#define LOG_INFO  GLOG_INFO
#define DLOG_TRACE GLOG_INFO << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
#define LOG_TRACE if (false) GLOG_INFO
#define LOG_DEBUG if (false) GLOG_INFO
#define LOG_INFO  if (false) GLOG_INFO
#define DLOG_TRACE if (false) GLOG_INFO
#endif

#define DLOG_WARN GLOG_WARNING << __PRETTY_FUNCTION__ << " this=" << this << " "
#define LOG_WARN  GLOG_WARNING
#define LOG_ERROR GLOG_ERROR
#define LOG_FATAL GLOG_FATAL

#else
#define LOG_TRACE std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOG_DEBUG std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO  std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOG_WARN  std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOG_ERROR std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOG_FATAL std::cout << __FILE__ << ":" << __LINE__ << " "
#define DLOG_TRACE std::cout << __PRETTY_FUNCTION__ << " this=" << this << " "
#define DLOG_WARN std::cout << __PRETTY_FUNCTION__ << " this=" << this << " "
#endif
#endif // end of define __cplusplus

//#ifdef _DEBUG
//#ifdef assert
//#undef assert
//#endif
//#define assert(expr)  { if (!(expr)) { LOG_FATAL << #expr ;} }
//#endif