#include "../include/r3_core.h"

#include <stdio.h>

void r3_init_core(void) {
    printf("R3 Core Initializing...\n");
    
    if (!r3_init_memory()) {
        printf("Failed to initialize R3 Memory Layer!\n");
    }
    
    if (!r3_init_platform()) {
        printf("Failed to initialize R3 Platform Layer!\n");
    }

    printf("R3 Core Initialized!\n");
}

void r3_shutdown_core(void) {
    printf("R3 Core Shutting Down...\n");
    
    r3_shutdown_platform();
    r3_shutdown_memory();

    printf("R3 Core Shutdown!\n");   
}