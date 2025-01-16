#include "../include/lotus_core.h"

#include <stdio.h>

void lotus_init_core(void) {
    printf("Sylva Core Initializing...\n");
    
    if (!lotus_init_memory()) {
        printf("Failed to initialize Sylva Memory Layer!\n");
    }
    
    if (!lotus_init_platform()) {
        printf("Failed to initialize Sylva Platform Layer!\n");
    }

    printf("Sylva Core Initialized!\n");
}

void lotus_shutdown_core(void) {
    printf("Sylva Core Shutting Down...\n");
    
    lotus_shutdown_platform();
    lotus_shutdown_memory();

    printf("Sylva Core Shutdown!\n");   
}