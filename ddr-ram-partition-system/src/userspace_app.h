#ifndef USERSPACE_APP_H
#define USERSPACE_APP_H

#include "ddr_memory.h"

// User application types
typedef enum {
    APP_TYPE_GUI,
    APP_TYPE_UTILITY,
    APP_TYPE_SERVICE,
    APP_TYPE_BACKGROUND,
    APP_TYPE_SYSTEM
} app_type_t;

// User application structure
typedef struct {
    uint32_t app_id;
    char name[64];
    app_type_t type;
    void* memory_region;
    size_t memory_size;
    uint32_t priority;
    bool is_running;
    time_t start_time;
} user_app_t;

// User space management
void userspace_init(memory_partition_t* partition);
void userspace_start_app(const char* name, app_type_t type, size_t memory_req);
void userspace_stop_app(uint32_t app_id);
void userspace_list_apps(void);

// Memory management for user apps
void* userspace_alloc(size_t size);
void userspace_free(void* ptr);
void userspace_garbage_collect(void);

// Application services
void userspace_run_scheduler(void);
void userspace_handle_events(void);
void userspace_update_apps(void);

// Statistics
typedef struct {
    uint32_t total_apps;
    uint32_t running_apps;
    size_t total_memory_used;
    size_t peak_memory_used;
    uint32_t app_switches;
} userspace_stats_t;

userspace_stats_t* get_userspace_stats(void);

#endif // USERSPACE_APP_H
