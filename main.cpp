#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    //setup_default_uart();
    stdio_init_all();

    while(true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000); //1s
    }
    
    return 0;
}

