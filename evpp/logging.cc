#include <evpp/logging.h>

namespace evpp {

    namespace {
        google::LogSink* sink_ = nullptr;
    }

    void set_sink(google::LogSink* s)
    {
        sink_ = s;
    }

    google::LogSink* sink()
    {
        return sink_;
    }

}