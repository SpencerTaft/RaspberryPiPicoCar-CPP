#include <stdio.h>
#include "pico/stdlib.h"

#include "lightRunnable.hpp"
#include "runtimeScheduler.hpp"

#include "pico/multicore.h"
#include "hardware/gpio.h"

int main() {
    //setup_default_uart();
    stdio_init_all();

    sleep_ms(10000); //10s

    LightRunnable runnable("left headlight");

    Scheduler::addRuntime(&runnable);
    
    multicore_launch_core1(Scheduler::runtimeLoop);


    while(true)
    {
        printf("CORE 1!\n");
        sleep_ms(1000); //1s
    }
    
    return 0;
}

