#include <stdio.h>
#include "pico/stdlib.h"

#include "lightRunnable.hpp"

int main() {
    //setup_default_uart();
    stdio_init_all();

    LightRunnable runnable("left headlight");

    while(true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000); //1s
    }
    
    return 0;
}

