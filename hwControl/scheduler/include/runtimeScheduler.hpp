#pragma once
#include <vector>
#include <atomic>
#include <thread>
#include "runnable.hpp"

class Scheduler
{
    public:
        void addRuntime(Runnable* newRunnable);
        void startRuntime();
        void stopRuntime();
    private:    
        void runtimeLoop();

        std::vector<Runnable*> _runnables;
        std::atomic<bool> _runtimeEnable{false};
        std::thread _runtimeThread;
};