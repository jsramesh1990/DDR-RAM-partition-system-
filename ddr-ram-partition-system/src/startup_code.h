#ifndef STARTUP_CODE_H
#define STARTUP_CODE_H

#include <stdint.h>
#include <stddef.h>

// Startup routines
void system_init(void);
void memory_init(void);
void partition_init(void);
void enable_interrupts(void);
void disable_interrupts(void);

// Bootstrap loader
void bootstrap_loader(void);

// System status
typedef struct {
    uint32_t total_memory;
    uint32_t available_memory;
    uint32_t gaming_memory_used;
    uint32_t rw_memory_used;
    uint32_t userspace_memory_used;
    uint8_t system_ready;
} system_status_t;

system_status_t* get_system_status(void);

#endif // STARTUP_CODE_H
