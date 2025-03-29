#include "runtimeScheduler.hpp"
#include <pico/stdlib.h>
#include <iostream>
#include <cstring>
#include "runnable.hpp"

std::vector<Runnable*> Scheduler::_runnables;

void Scheduler::addRuntime(Runnable* newRunnable)
{
    Scheduler::_runnables.push_back(newRunnable);
}

bool Scheduler::updateConfig(char* runnableID, char* newConfig)
{
    int runnableIndex = Scheduler::getRunnableIndex(runnableID);
    if ((runnableIndex == -1) || (Scheduler::_runnables.size() <= runnableIndex))
    {
        return false;
    }

    return Scheduler::_runnables[runnableIndex]->setConfig(newConfig);
}

int Scheduler::getRunnableIndex(char* runnableID)
{
    for (int i = 0; i < Scheduler::_runnables.size(); i++)
    {
        char* foundID = Scheduler::_runnables[i]->getID();
        if (strcmp(foundID, runnableID) == 0)
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

