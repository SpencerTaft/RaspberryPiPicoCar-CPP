#include "runtimeScheduler.hpp"

bool Scheduler::addRuntime(Runnable* newRunnable)
{
    _runnables.push_back(newRunnable);
}

void Scheduler::startRuntime()
{
    _runtimeEnable = true;
}

void Scheduler::stopRuntime()
{
    _runtimeEnable = false;
}

void Scheduler::runtimeScan()
{
    //todo
}


////////////