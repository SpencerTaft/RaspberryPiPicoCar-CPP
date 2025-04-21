#include "runtimeScheduler.hpp"
#include <pico/stdlib.h>
#include <iostream>
#include <cstring>
#include "runnable.hpp"
#include <nlohmann/json.hpp>

std::vector<Runnable*> Scheduler::_runnables;

void Scheduler::addRuntime(Runnable* newRunnable)
{
    Scheduler::_runnables.push_back(newRunnable);
}

bool Scheduler::updateConfig(char* newConfig)
{
    bool updateConfigSuccess = true;

    nlohmann::json parsedConfig = nlohmann::json::parse(newConfig);

    //MVP only expects one config item named config1, but this can be adjusted to take an array of configs in the future
    if (parsedConfig.is_object() && parsedConfig.contains("config1"))
    {
        nlohmann::json firstConfig = parsedConfig["config1"];

        if (firstConfig.is_object() && firstConfig.contains("ID"))
        {
            int runnableIndex = getRunnableIndex((char*)firstConfig["ID"].get<std::string>().c_str()); //todo see if c++strings link here
            if (runnableIndex == -1)
            {
                printf("Runnable with ID %s not found\n", firstConfig["ID"].get<std::string>().c_str());
                updateConfigSuccess = false;
            }

            if (!Scheduler::_runnables[runnableIndex]->setConfig(firstConfig.dump().c_str()))
            {
                printf("Failed to set config for runnable %s\n", firstConfig["ID"].get<std::string>().c_str());
                updateConfigSuccess = false;
            }
        }
    }
    else
    {
        printf("Invalid configuration received");
        updateConfigSuccess = false;
    }

    return updateConfigSuccess;
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
        sleep_ms(RUNTIME_PERIOD_MS);
    }
}

