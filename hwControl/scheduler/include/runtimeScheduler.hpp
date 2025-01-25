#pragma once
#include <vector>
#include "runnable.hpp"

class Scheduler
{
    public:
        static void addRuntime(Runnable* newRunnable);
        static void runtimeLoop();
        
    private:    
        static std::vector<Runnable*> _runnables;
};