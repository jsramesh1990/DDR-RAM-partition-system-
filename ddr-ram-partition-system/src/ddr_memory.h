#ifndef DDR_MEMORY_H
#define DDR_MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// DDR Memory structure
typedef struct {
    uint8_t* base_address;
    size_t total_size;
    size_t used_size;
    uint32_t protection_flags;
    bool is_initialized;
} ddr_memory_t;

// Partition structure
typedef struct {
    uint8_t* base_address;
    size_t size;
    size_t used;
    uint32_t protection;
    char name[32];
} memory_partition_t;

// Memory management functions
ddr_memory_t* ddr_init(size_t total_size);
void ddr_deinit(ddr_memory_t* memory);

memory_partition_t* create_partition(ddr_memory_t* memory, 
                                     size_t size, 
                                     uint32_t protection,
                                     const char* name);

void* partition_alloc(memory_partition_t* partition, size_t size);
void partition_free(memory_partition_t* partition, void* ptr);
void partition_clear(memory_partition_t* partition);

// Memory utilities
void memory_copy(void* dest, const void* src, size_t n);
void memory_set(void* ptr, uint8_t value, size_t n);
void memory_protect(memory_partition_t* partition, uint32_t flags);

// Debug functions
void print_memory_stats(const ddr_memory_t* memory);
void print_partition_stats(const memory_partition_t* partition);

#endif // DDR_MEMORY_H
