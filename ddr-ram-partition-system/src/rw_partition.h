#ifndef RW_PARTITION_H
#define RW_PARTITION_H

#include "ddr_memory.h"

// Data structure for read/write operations
typedef struct {
    uint32_t id;
    uint8_t* data;
    size_t size;
    uint32_t checksum;
    time_t timestamp;
} data_block_t;

// Read/Write operations
void rw_init(memory_partition_t* partition);
void rw_perform_operations(void);
void rw_benchmark(void);

// Data management
data_block_t* rw_create_data_block(size_t size);
void rw_write_data(data_block_t* block, const void* data, size_t size);
void rw_read_data(const data_block_t* block, void* buffer, size_t size);
void rw_delete_data_block(data_block_t* block);

// Utility functions
uint32_t calculate_checksum(const void* data, size_t size);
void rw_defragment(void);
void rw_verify_integrity(void);

// Performance metrics
typedef struct {
    size_t total_reads;
    size_t total_writes;
    size_t bytes_read;
    size_t bytes_written;
    double read_latency;
    double write_latency;
} rw_metrics_t;

rw_metrics_t* get_rw_metrics(void);

#endif // RW_PARTITION_H
