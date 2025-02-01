#include "../include/lotus_core.h"

#include <stdio.h>

void lotus_init_core(void) {
    printf("Lotus Core Initializing...\n");
    
    if (!lotus_init_memory()) {
        printf("Failed to initialize Lotus Memory Layer!\n");
    }
    
    if (!lotus_init_platform()) {
        printf("Failed to initialize Lotus Platform Layer!\n");
    }

    printf("Lotus Core Initialized!\n");
}

void lotus_shutdown_core(void) {
    printf("Lotus Core Shutting Down...\n");
    
    lotus_shutdown_platform();
    lotus_shutdown_memory();

    printf("Lotus Core Shutdown!\n");   
}