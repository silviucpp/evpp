#pragma once

#include "evpp/inner_pre.h"

namespace evpp {
class Buffer;
class TCPConn;

typedef std::shared_ptr<TCPConn> TCPConnPtr;
typedef evpp::function<void()> TimerCallback;

// When a connection established, broken down, connecting failed, this callback will be called
// This is called from a work-thread this is not the listening thread probably
typedef evpp::function<void(const TCPConnPtr&)> ConnectionCallback;


typedef evpp::function<void(const TCPConnPtr&)> CloseCallback;
typedef evpp::function<void(const TCPConnPtr&)> WriteCompleteCallback;
typedef evpp::function<void(const TCPConnPtr&, size_t)> HighWaterMarkCallback;

typedef evpp::function<void(const TCPConnPtr&, Buffer*)> MessageCallback;

namespace internal {
inline void DefaultConnectionCallback(const TCPConnPtr&) {}
inline void DefaultMessageCallback(const TCPConnPtr&, Buffer*) {}
}

}
