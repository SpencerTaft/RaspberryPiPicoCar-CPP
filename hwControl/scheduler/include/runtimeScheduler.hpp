#pragma once
#include <vector>
#include "runnable.hpp"

class Scheduler
{
    public:
        static void addRuntime(Runnable* newRunnable);
        
        static bool updateConfig(char* newConfig);
        static void runtimeLoop();
        
    private:
        static int getRunnableIndex(char* runnableID);    

        static std::vector<Runnable*> _runnables;
};