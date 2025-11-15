#include <stdio.h>
#include <pico/stdlib.h>

#include "lightRunnable.hpp"
#include "pwmLightRunnable.hpp"
#include "runtimeScheduler.hpp"
#include "httpServer.hpp"

#include "pico/multicore.h"
#include "hardware/gpio.h"

int main() {
    stdio_init_all(); //todo read about

    sleep_ms(1000); //1s

    //LightRunnable leftHeadlightRunnable("left headlight");
    //LightRunnable rightHeadlightRunnable("right headlight");

    PWMLightRunnable pwmLightRunnable("flashlight");

    //Scheduler::addRuntime(&leftHeadlightRunnable);
    //Scheduler::addRuntime(&rightHeadlightRunnable);
    Scheduler::addRuntime(&pwmLightRunnable);
    

    multicore_launch_core1(Scheduler::runtimeLoop);

    sleep_ms(1000); //1s

    HttpServer server;
    server.initAndRunServer();

    while(true)
    {
        printf("CORE 1!\n");
        sleep_ms(1000); //1s
    }
    
    return 0;
}

