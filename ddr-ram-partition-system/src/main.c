#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ddr_memory.h"
#include "gaming_partition.h"
#include "rw_partition.h"
#include "userspace_app.h"
#include "config.h"
#include "startup_code.h"

// Global memory pointers
static ddr_memory_t* ddr_memory = NULL;
static memory_partition_t* gaming_partition = NULL;
static memory_partition_t* rw_partition = NULL;
static memory_partition_t* userspace_partition = NULL;

void startup_code(void) {
    printf("\n=== System Startup Sequence ===\n");
    
    // Step 1: Initialize system
    printf("1. Initializing system...\n");
    system_init();
    
    // Step 2: Initialize memory
    printf("2. Initializing DDR memory...\n");
    memory_init();
    
    // Step 3: Load bootstrap
    printf("3. Loading bootstrap...\n");
    bootstrap_loader();
    
    // Step 4: Enable interrupts
    printf("4. Enabling interrupts...\n");
    enable_interrupts();
    
    printf("System startup complete!\n");
}

void system_init(void) {
    printf("  - CPU initialized\n");
    printf("  - Clocks configured\n");
    printf("  - Peripherals reset\n");
}

void memory_init(void) {
    // Create DDR memory
    ddr_memory = ddr_init(TOTAL_DDR_SIZE);
    if (!ddr_memory) {
        fprintf(stderr, "Failed to initialize DDR memory!\n");
        exit(1);
    }
    
    // Create partitions
    gaming_partition = create_partition(ddr_memory, PARTITION_SIZE, 
                                       MEM_READ_WRITE | MEM_EXECUTE, 
                                       "Gaming");
    
    rw_partition = create_partition(ddr_memory, PARTITION_SIZE,
                                   MEM_READ_WRITE,
                                   "Read/Write");
    
    userspace_partition = create_partition(ddr_memory, PARTITION_SIZE,
                                          MEM_READ_WRITE | MEM_EXECUTE,
                                          "User Space");
    
    if (!gaming_partition || !rw_partition || !userspace_partition) {
        fprintf(stderr, "Failed to create partitions!\n");
        exit(1);
    }
}

void bootstrap_loader(void) {
    printf("  - Loading bootloader from ROM\n");
    printf("  - Verifying boot signature\n");
    printf("  - Jumping to main application\n");
}

void enable_interrupts(void) {
    printf("  - Configuring interrupt controller\n");
    printf("  - Setting up interrupt handlers\n");
    printf("  - Enabling global interrupts\n");
}

void disable_interrupts(void) {
    // For simulation
}

void partition_init(void) {
    printf("  - Setting up partition tables\n");
    printf("  - Configuring memory protection\n");
}

system_status_t* get_system_status(void) {
    static system_status_t status;
    
    status.total_memory = TOTAL_DDR_SIZE;
    status.available_memory = TOTAL_DDR_SIZE - ddr_memory->used_size;
    
    if (gaming_partition) status.gaming_memory_used = gaming_partition->used;
    if (rw_partition) status.rw_memory_used = rw_partition->used;
    if (userspace_partition) status.userspace_memory_used = userspace_partition->used;
    
    status.system_ready = 1;
    
    return &status;
}

void demo_gaming_partition(void) {
    printf("\n=== Gaming Partition Demo ===\n");
    
    gaming_init(gaming_partition);
    gaming_load_textures();
    
    // Run a few frames
    for (int i = 0; i < 5; i++) {
        gaming_start_frame();
        gaming_process_input();
        gaming_update_physics();
        gaming_render_frame();
        gaming_calculate_score();
        gaming_end_frame();
        
        // Create some game objects
        if (i % 2 == 0) {
            game_object_t* obj = create_game_object(gaming_partition);
            if (obj) {
                printf("Created game object: ID=%u, Pos=(%.1f, %.1f)\n",
                       obj->id, obj->position_x, obj->position_y);
            }
        }
        
        usleep(100000);  // 100ms delay
    }
    
    game_state_t* state = get_game_state();
    if (state) {
        printf("\nFinal Game State:\n");
        printf("  Frames: %u\n", state->frame_count);
        printf("  Game Time: %.2f\n", state->game_time);
        printf("  Score: %u\n", state->score);
        printf("  Active Objects: %u\n", state->active_objects);
        printf("  FPS: %.2f\n", gaming_get_fps());
    }
}

void demo_rw_partition(void) {
    printf("\n=== Read/Write Partition Demo ===\n");
    
    rw_init(rw_partition);
    rw_perform_operations();
    rw_benchmark();
    
    // Additional tests
    printf("\n--- Additional RW Tests ---\n");
    
    // Create multiple data blocks
    data_block_t* blocks[5];
    for (int i = 0; i < 5; i++) {
        blocks[i] = rw_create_data_block(512);
        if (blocks[i]) {
            char data[100];
            snprintf(data, sizeof(data), "Test data for block %d", i);
            rw_write_data(blocks[i], data, strlen(data) + 1);
        }
    }
    
    // Verify and clean up
    for (int i = 0; i < 5; i++) {
        if (blocks[i]) {
            rw_delete_data_block(blocks[i]);
        }
    }
    
    rw_defragment();
    rw_verify_integrity();
}

void demo_userspace_partition(void) {
    printf("\n=== User Space Partition Demo ===\n");
    
    userspace_init(userspace_partition);
    
    // Start some applications
    userspace_start_app("Web Browser", APP_TYPE_GUI, 50 * 1024 * 1024);
    userspace_start_app("Media Player", APP_TYPE_GUI, 30 * 1024 * 1024);
    userspace_start_app("Background Service", APP_TYPE_SERVICE, 5 * 1024 * 1024);
    
    // Run user space operations
    for (int i = 0; i < 10; i++) {
        userspace_run_scheduler();
        userspace_handle_events();
        userspace_update_apps();
        
        if (i % 3 == 0) {
            userspace_garbage_collect();
        }
        
        if (i == 5) {
            // Stop an app
            userspace_stop_app(1001);  // File Manager
        }
        
        usleep(50000);  // 50ms delay
    }
    
    // List all applications
    userspace_list_apps();
}

void print_system_status(void) {
    system_status_t* status = get_system_status();
    
    printf("\n=== System Status ===\n");
    printf("Total Memory: %u MB\n", status->total_memory / (1024 * 1024));
    printf("Available Memory: %u MB\n", status->available_memory / (1024 * 1024));
    printf("Gaming Memory Used: %u MB\n", status->gaming_memory_used / (1024 * 1024));
    printf("RW Memory Used: %u MB\n", status->rw_memory_used / (1024 * 1024));
    printf("User Space Memory Used: %u MB\n", status->userspace_memory_used / (1024 * 1024));
    printf("System Ready: %s\n", status->system_ready ? "Yes" : "No");
}

int main(void) {
    printf("=== DDR RAM Partition System ===\n");
    printf("Three Partition Memory Management System\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    // Run startup code
    startup_code();
    
    // Print memory layout
    printf("\n=== Memory Layout ===\n");
    printf("Gaming Partition:   0x%08X - 0x%08X (%zu MB)\n",
           GAMING_PARTITION_BASE, 
           GAMING_PARTITION_BASE + PARTITION_SIZE - 1,
           PARTITION_SIZE / (1024 * 1024));
    
    printf("Read/Write Partition: 0x%08X - 0x%08X (%zu MB)\n",
           RW_PARTITION_BASE,
           RW_PARTITION_BASE + PARTITION_SIZE - 1,
           PARTITION_SIZE / (1024 * 1024));
    
    printf("User Space Partition: 0x%08X - 0x%08X (%zu MB)\n",
           USERSPACE_PARTITION_BASE,
           USERSPACE_PARTITION_BASE + PARTITION_SIZE - 1,
           PARTITION_SIZE / (1024 * 1024));
    
    // Demo each partition
    demo_gaming_partition();
    demo_rw_partition();
    demo_userspace_partition();
    
    // Print statistics
    print_memory_stats(ddr_memory);
    print_partition_stats(gaming_partition);
    print_partition_stats(rw_partition);
    print_partition_stats(userspace_partition);
    
    print_system_status();
    
    // Cleanup
    printf("\n=== System Shutdown ===\n");
    disable_interrupts();
    
    if (ddr_memory) {
        ddr_deinit(ddr_memory);
    }
    
    printf("System shutdown complete. Goodbye!\n");
    
    return 0;
}
