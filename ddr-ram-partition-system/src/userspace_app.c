#include "userspace_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USER_APPS 20

static memory_partition_t* userspace_partition = NULL;
static user_app_t* apps[MAX_USER_APPS] = {0};
static userspace_stats_t stats = {0};
static uint32_t next_app_id = 1000;

void userspace_init(memory_partition_t* partition) {
    if (!partition) return;
    
    userspace_partition = partition;
    
    // Initialize stats
    memset(&stats, 0, sizeof(userspace_stats_t));
    
    printf("User space partition initialized\n");
    
    // Start some default apps
    userspace_start_app("System Monitor", APP_TYPE_SYSTEM, 2 * 1024 * 1024);
    userspace_start_app("File Manager", APP_TYPE_UTILITY, 5 * 1024 * 1024);
}

void userspace_start_app(const char* name, app_type_t type, size_t memory_req) {
    if (!userspace_partition || !name) return;
    
    // Find free slot
    int free_slot = -1;
    for (int i = 0; i < MAX_USER_APPS; i++) {
        if (apps[i] == NULL) {
            free_slot = i;
            break;
        }
    }
    
    if (free_slot == -1) {
        printf("Cannot start app '%s': Maximum apps reached\n", name);
        return;
    }
    
    // Check available memory
    if (memory_req > userspace_partition->size - userspace_partition->used) {
        printf("Cannot start app '%s': Insufficient memory\n", name);
        return;
    }
    
    // Allocate app structure
    user_app_t* app = (user_app_t*)partition_alloc(userspace_partition, sizeof(user_app_t));
    if (!app) return;
    
    // Allocate app memory
    app->memory_region = partition_alloc(userspace_partition, memory_req);
    if (!app->memory_region) {
        // Handle allocation failure
        return;
    }
    
    // Initialize app
    app->app_id = next_app_id++;
    strncpy(app->name, name, sizeof(app->name) - 1);
    app->type = type;
    app->memory_size = memory_req;
    app->priority = 5;  // Default priority
    app->is_running = true;
    app->start_time = time(NULL);
    
    apps[free_slot] = app;
    
    // Update statistics
    stats.total_apps++;
    stats.running_apps++;
    stats.total_memory_used += memory_req;
    if (stats.total_memory_used > stats.peak_memory_used) {
        stats.peak_memory_used = stats.total_memory_used;
    }
    
    printf("Started app '%s' (ID: %u, Type: %d, Memory: %zu MB)\n",
           name, app->app_id, type, memory_req / (1024 * 1024));
    
    // Initialize app memory with some data
    switch (type) {
        case APP_TYPE_GUI:
            memset(app->memory_region, 0xAA, memory_req);
            break;
        case APP_TYPE_UTILITY:
            memset(app->memory_region, 0xBB, memory_req);
            break;
        case APP_TYPE_SERVICE:
            memset(app->memory_region, 0xCC, memory_req);
            break;
        default:
            memset(app->memory_region, 0x00, memory_req);
    }
}

void userspace_stop_app(uint32_t app_id) {
    for (int i = 0; i < MAX_USER_APPS; i++) {
        if (apps[i] && apps[i]->app_id == app_id) {
            printf("Stopping app '%s' (ID: %u)\n", apps[i]->name, app_id);
            
            // Update statistics
            stats.running_apps--;
            stats.total_memory_used -= apps[i]->memory_size;
            
            // Free app memory (in real system)
            apps[i]->is_running = false;
            
            // Note: In real implementation, we'd free the memory
            // For simulation, we just mark it as stopped
            
            apps[i] = NULL;
            return;
        }
    }
    
    printf("App with ID %u not found\n", app_id);
}

void userspace_list_apps(void) {
    printf("\n=== User Space Applications ===\n");
    
    bool found = false;
    for (int i = 0; i < MAX_USER_APPS; i++) {
        if (apps[i]) {
            found = true;
            printf("ID: %u, Name: %s, Type: %d, Memory: %zu MB, Running: %s\n",
                   apps[i]->app_id,
                   apps[i]->name,
                   apps[i]->type,
                   apps[i]->memory_size / (1024 * 1024),
                   apps[i]->is_running ? "Yes" : "No");
        }
    }
    
    if (!found) {
        printf("No applications running\n");
    }
    
    printf("\nStatistics:\n");
    printf("  Total Apps: %u\n", stats.total_apps);
    printf("  Running Apps: %u\n", stats.running_apps);
    printf("  Memory Used: %.2f MB\n", stats.total_memory_used / (1024.0 * 1024.0));
    printf("  Peak Memory: %.2f MB\n", stats.peak_memory_used / (1024.0 * 1024.0));
}

void* userspace_alloc(size_t size) {
    if (!userspace_partition || size == 0) return NULL;
    
    void* ptr = partition_alloc(userspace_partition, size);
    if (ptr) {
        stats.total_memory_used += size;
        if (stats.total_memory_used > stats.peak_memory_used) {
            stats.peak_memory_used = stats.total_memory_used;
        }
    }
    
    return ptr;
}

void userspace_free(void* ptr) {
    // Note: In real implementation, we'd free the memory
    // For simulation, we just log it
    printf("User space free operation simulated\n");
}

void userspace_garbage_collect(void) {
    printf("Running garbage collection...\n");
    
    // Simulate garbage collection
    static int gc_counter = 0;
    gc_counter++;
    
    if (gc_counter % 10 == 0) {
        printf("Garbage collection completed. Reclaimed simulated memory.\n");
    }
}

void userspace_run_scheduler(void) {
    static int scheduler_ticks = 0;
    scheduler_ticks++;
    
    if (scheduler_ticks % 100 == 0) {
        printf("User space scheduler: Running app context switch\n");
        stats.app_switches++;
    }
}

void userspace_handle_events(void) {
    static int event_counter = 0;
    event_counter++;
    
    if (event_counter % 50 == 0) {
        printf("User space: Processing events...\n");
        
        // Simulate random app starts/stops
        if (rand() % 100 > 70 && stats.running_apps < MAX_USER_APPS) {
            const char* app_names[] = {
                "Web Browser", "Text Editor", "Media Player", 
                "Calculator", "Terminal", "Settings"
            };
            int idx = rand() % 6;
            userspace_start_app(app_names[idx], APP_TYPE_UTILITY, 
                               (rand() % 10 + 1) * 1024 * 1024);
        }
    }
}

void userspace_update_apps(void) {
    static int update_counter = 0;
    update_counter++;
    
    if (update_counter % 200 == 0) {
        printf("Updating user applications...\n");
        
        // Simulate app updates
        for (int i = 0; i < MAX_USER_APPS; i++) {
            if (apps[i] && apps[i]->is_running) {
                // Simulate app activity
                if (rand() % 100 > 90) {
                    printf("  App '%s' is active\n", apps[i]->name);
                }
            }
        }
    }
}

userspace_stats_t* get_userspace_stats(void) {
    return &stats;
}
