#pragma once
#include <vector>
#include "runnable.hpp"

class Scheduler
{
    public:
        static void addRuntime(Runnable* newRunnable);
        
        static bool updateConfig(const std::string& runnableID, const nlohmann::json& newConfig);
        static void runtimeLoop();
        
    private:
        static int getRunnableIndex(const std::string& runnableID);    

        static std::vector<Runnable*> _runnables;
};