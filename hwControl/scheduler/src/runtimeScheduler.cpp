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

bool Scheduler::updateConfig(char* newConfig)
{
    char* parsedRunnableID;
    char* parsedConfig;

    //Parse the runnableID and newConfig
    printf("in Scheduler::updateConfig: %s\n", newConfig);
    const char* delimiter = "~";
    char* token = strtok(newConfig, delimiter);
    int tokenIndex = 0;

    if (token == nullptr)
    {
        printf("Tokens was null, debug\n");
    }

    
    while(token != nullptr)
    {
        if(tokenIndex == 0)
        {
            parsedRunnableID = token;
            printf("Parsed runnableID: %s\n", parsedRunnableID);
        }
        else if(tokenIndex == 1)
        {
            parsedConfig = token;
            printf("Parsed Config: %s\n", parsedConfig);
            break;
        }
        else{
            printf("Invalid Config: %s\n", token);
            return false;
        }
        token = strtok(nullptr, delimiter);
        tokenIndex++;
    }

    int runnableIndex = Scheduler::getRunnableIndex(parsedRunnableID);
    if ((runnableIndex == -1) || (Scheduler::_runnables.size() <= runnableIndex))
    {
        return false;
    }

    return Scheduler::_runnables[runnableIndex]->setConfig(parsedConfig);
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

