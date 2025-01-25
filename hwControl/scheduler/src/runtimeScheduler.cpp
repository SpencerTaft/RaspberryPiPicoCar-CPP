#include "runtimeScheduler.hpp"
#include "pico/stdlib.h"
#include <iostream>
#include "runnable.hpp"

// void Scheduler::addRuntime(Runnable* newRunnable)
// {
//     Scheduler::_runnables.push_back(newRunnable);
// }

void Scheduler::runtimeLoop()
{
    std::cout << "runtime loop started" << std::endl;
    while(true)
    {
        std::cout << "core 2?" << std::endl;
        sleep_ms(1000);
    }
    std::cout << "runtime loop stopped" << std::endl;
}