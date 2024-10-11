#pragma once

#include <memory>
#include "evpp/functional.h"

struct event_base;

namespace recipes {

class TimerEventWatcher;
class InvokeTimer;

class InvokeTimer {
public:
    typedef evpp::function<void()> Functor;

    InvokeTimer(struct event_base* evloop, double timeout_ms, const Functor& f);
    ~InvokeTimer();

    void Start();

private:
    void OnTimerTriggered();

private:
    struct event_base* loop_;
    double timeout_ms_;
    Functor functor_;
    std::shared_ptr<TimerEventWatcher> timer_;
};

}
