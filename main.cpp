#include <stdio.h>
#include "pico/stdlib.h"

#include "lightRunnable.hpp"
#include "runtimeScheduler.hpp"

int main() {
    //setup_default_uart();
    stdio_init_all();

    sleep_ms(10000); //10s

    LightRunnable runnable("left headlight");

    Scheduler scheduler;

    scheduler.addRuntime(&runnable);
    scheduler.startRuntime();

    while(true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000); //1s
    }
    
    return 0;
}

