#include "runtimeScheduler.hpp"
#include <thread>
#include <chrono>
#include <iostream>

void Scheduler::addRuntime(Runnable* newRunnable)
{
    _runnables.push_back(newRunnable);
}

void Scheduler::startRuntime()
{
    if (!_runtimeEnable)
    {
        _runtimeEnable = true;
        _runtimeThread = std::thread(&Scheduler::runtimeLoop, this);
    }
}

void Scheduler::stopRuntime()
{
    _runtimeEnable = false;
    if (_runtimeThread.joinable())
    {
        _runtimeThread.join();
    }
}

void Scheduler::runtimeLoop()
{
    std::cout << "runtime loop started" << std::endl;
    while (_runtimeEnable)
    {
        std::cout << "runtime scan" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    std::cout << "runtime loop stopped" << std::endl;
}