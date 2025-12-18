#include "ddr_memory.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Memory allocation tracking structure
typedef struct mem_block {
    void* address;
    size_t size;
    struct mem_block* next;
    uint32_t magic;  // For corruption detection
} mem_block_t;

#define MAGIC_NUMBER 0xDEADBEEF

ddr_memory_t* ddr_init(size_t total_size) {
    ddr_memory_t* memory = malloc(sizeof(ddr_memory_t));
    if (!memory) return NULL;
    
    memory->base_address = malloc(total_size);
    if (!memory->base_address) {
        free(memory);
        return NULL;
    }
    
    memory->total_size = total_size;
    memory->used_size = 0;
    memory->protection_flags = MEM_READ_WRITE;
    memory->is_initialized = true;
    
    // Initialize memory to zero
    memset(memory->base_address, 0, total_size);
    
    printf("DDR Memory initialized: %zu MB\n", total_size / (1024 * 1024));
    return memory;
}

void ddr_deinit(ddr_memory_t* memory) {
    if (memory) {
        if (memory->base_address) {
            free(memory->base_address);
        }
        free(memory);
    }
}

memory_partition_t* create_partition(ddr_memory_t* memory, 
                                     size_t size, 
                                     uint32_t protection,
                                     const char* name) {
    if (!memory || size > memory->total_size - memory->used_size) {
        return NULL;
    }
    
    memory_partition_t* partition = malloc(sizeof(memory_partition_t));
    if (!partition) return NULL;
    
    partition->base_address = memory->base_address + memory->used_size;
    partition->size = size;
    partition->used = 0;
    partition->protection = protection;
    strncpy(partition->name, name, sizeof(partition->name) - 1);
    
    memory->used_size += size;
    
    printf("Partition '%s' created: %zu MB, Protection: 0x%08X\n",
           name, size / (1024 * 1024), protection);
    
    return partition;
}

// Simple first-fit allocator for demonstration
void* partition_alloc(memory_partition_t* partition, size_t size) {
    if (!partition || size == 0 || size > partition->size - partition->used) {
        return NULL;
    }
    
    // Check protection
    if ((partition->protection & MEM_NO_ACCESS) || 
        (!(partition->protection & MEM_READ_WRITE))) {
        return NULL;
    }
    
    void* ptr = partition->base_address + partition->used;
    partition->used += size;
    
    // Initialize allocated memory to zero
    memset(ptr, 0, size);
    
    return ptr;
}

void partition_free(memory_partition_t* partition, void* ptr) {
    // Simplified implementation - in real system would track allocations
    // For simulation, we just decrement used counter
    if (partition && ptr) {
        // Note: Real implementation would need proper allocation tracking
        printf("Free operation simulated for partition '%s'\n", partition->name);
    }
}

void partition_clear(memory_partition_t* partition) {
    if (partition) {
        memset(partition->base_address, 0, partition->size);
        partition->used = 0;
    }
}

void memory_copy(void* dest, const void* src, size_t n) {
    memcpy(dest, src, n);
}

void memory_set(void* ptr, uint8_t value, size_t n) {
    memset(ptr, value, n);
}

void memory_protect(memory_partition_t* partition, uint32_t flags) {
    if (partition) {
        partition->protection = flags;
    }
}

void print_memory_stats(const ddr_memory_t* memory) {
    if (!memory) return;
    
    printf("\n=== DDR Memory Statistics ===\n");
    printf("Total Size: %zu MB\n", memory->total_size / (1024 * 1024));
    printf("Used Size: %zu MB\n", memory->used_size / (1024 * 1024));
    printf("Available: %zu MB\n", 
           (memory->total_size - memory->used_size) / (1024 * 1024));
    printf("Protection Flags: 0x%08X\n", memory->protection_flags);
    printf("Initialized: %s\n", memory->is_initialized ? "Yes" : "No");
}

void print_partition_stats(const memory_partition_t* partition) {
    if (!partition) return;
    
    printf("\n=== Partition '%s' Statistics ===\n", partition->name);
    printf("Base Address: %p\n", partition->base_address);
    printf("Total Size: %zu MB\n", partition->size / (1024 * 1024));
    printf("Used: %zu MB\n", partition->used / (1024 * 1024));
    printf("Available: %zu MB\n", 
           (partition->size - partition->used) / (1024 * 1024));
    printf("Protection: 0x%08X\n", partition->protection);
    printf("Usage: %.2f%%\n", 
           (float)partition->used / partition->size * 100.0f);
}
