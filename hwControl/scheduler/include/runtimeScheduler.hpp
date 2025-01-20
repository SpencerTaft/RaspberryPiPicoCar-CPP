#pragma once
#include <vector>
#include "runnable.hpp"

class Scheduler
{
    public:
        bool addRuntime(Runnable* newRunnable);
        void startRuntime();
        void stopRuntime();

    private:
        void runtimeScan();
    
        std::vector<Runnable*> _runnables;
        bool _runtimeEnable;

};
