#include "runtimeScheduler.hpp"
#include "pico/stdlib.h"
#include <iostream>
#include "runnable.hpp"

std::vector<Runnable*> Scheduler::_runnables;

void Scheduler::addRuntime(Runnable* newRunnable)
{
    Scheduler::_runnables.push_back(newRunnable);
}

bool Scheduler::updateConfig(const std::string& runnableID, const nlohmann::json& newConfig)
{
    int runnableIndex = Scheduler::getRunnableIndex(runnableID);
    if ((runnableIndex == -1) || (Scheduler::_runnables.size() <= runnableIndex))
    {
        return false;
    }

    return Scheduler::_runnables[runnableIndex]->setConfig(newConfig);
}

int Scheduler::getRunnableIndex(const std::string& runnableID)
{
    for (int i = 0; i < Scheduler::_runnables.size(); i++)
    {
        if (Scheduler::_runnables[i]->getID() == runnableID)
        {
            return i;
        }
    }
    return -1;
}

void Scheduler::runtimeLoop()
{
    while(true)
    {
        for (Runnable* runnable : Scheduler::_runnables)
        {
            runnable->runtime();
        }
        sleep_ms(50000);
    }
}

