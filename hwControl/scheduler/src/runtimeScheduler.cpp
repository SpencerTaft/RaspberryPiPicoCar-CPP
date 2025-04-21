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
    printf("in Scheduler::updateConfig: %s\n", newConfig); //{"left headlight": {"pin": "22", "isOn": "true"}}

    nlohmann::json parsedConfig = nlohmann::json::parse(newConfig);
    printf("Parsed config: %s\n", parsedConfig.dump().c_str());

    //Read first config object for MVP
    if (parsedConfig.is_object() && parsedConfig.contains("config1"))
    {
        printf("Parsed config contains ID\n");
        nlohmann::json firstConfig = parsedConfig["config1"];//nlohmann::json::parse(newConfig);

            if (firstConfig.is_object() && firstConfig.contains("ID"))
        {
            int runnableIndex = getRunnableIndex((char*)firstConfig["ID"].get<std::string>().c_str()); //todo see if c++strings link here

            if (!Scheduler::_runnables[runnableIndex]->setConfig(firstConfig.dump().c_str()))
            {
                printf("Failed to set config for runnable %s\n", firstConfig["ID"].get<std::string>().c_str());
                return false;
            }
    }
    
    return true; //todo temp solution
}
    





//////////////-------------------------////// todo update this to use "newConfig"
    // int runnableIndex = Scheduler::getRunnableIndex(parsedRunnableID);
    // if ((runnableIndex == -1) || (Scheduler::_runnables.size() <= runnableIndex))
    // {
    //     return false;
    // }

    // return Scheduler::_runnables[runnableIndex]->setConfig(parsedConfig);
    return true;
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

