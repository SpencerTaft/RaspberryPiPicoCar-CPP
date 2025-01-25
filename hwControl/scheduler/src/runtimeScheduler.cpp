#include "runtimeScheduler.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include "runnable.hpp"

void Scheduler::addRuntime(Runnable* newRunnable)
{
    _runnables.push_back(newRunnable);
}

void Scheduler::startRuntime()
{
    if (!_runtimeEnable)
    {
        _runtimeEnable = true;
        //_runtimeThread = std::thread(&Scheduler::runtimeLoop,this); //todo fix
        runtimeLoop();
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
    //while (_runtimeEnable) //todo fix
    std::cout << "runtime loop stopped" << std::endl;
}