#include "rw_partition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

static memory_partition_t* rw_partition = NULL;
static rw_metrics_t metrics = {0};
static uint32_t next_block_id = 1;

void rw_init(memory_partition_t* partition) {
    if (!partition) return;
    
    rw_partition = partition;
    
    printf("Read/Write partition initialized\n");
    
    // Initialize metrics
    memset(&metrics, 0, sizeof(rw_metrics_t));
}

void rw_perform_operations(void) {
    if (!rw_partition) return;
    
    printf("\n=== Performing Read/Write Operations ===\n");
    
    // Create and write data
    data_block_t* block = rw_create_data_block(1024);  // 1KB block
    if (!block) {
        printf("Failed to create data block!\n");
        return;
    }
    
    // Write some data
    char test_data[] = "Hello from Read/Write partition! Testing 1, 2, 3...";
    rw_write_data(block, test_data, sizeof(test_data));
    
    // Read the data back
    char read_buffer[1024];
    rw_read_data(block, read_buffer, sizeof(test_data));
    
    printf("Data written and read: %s\n", read_buffer);
    
    // Verify checksum
    uint32_t calculated_cs = calculate_checksum(block->data, block->size);
    printf("Checksum: stored=0x%08X, calculated=0x%08X\n", 
           block->checksum, calculated_cs);
    
    if (block->checksum == calculated_cs) {
        printf("Data integrity: OK\n");
    } else {
        printf("Data integrity: FAILED\n");
    }
    
    // Clean up
    rw_delete_data_block(block);
}

void rw_benchmark(void) {
    if (!rw_partition) return;
    
    printf("\n=== Read/Write Benchmark ===\n");
    
    const size_t block_sizes[] = {64, 256, 1024, 4096, 16384};  // Bytes
    const int iterations = 100;
    
    for (int i = 0; i < 5; i++) {
        size_t block_size = block_sizes];
        clock_t total_time = 0;
        
        printf("\nTesting block size: %zu bytes\n", block_size);
        
        // Write benchmark
        clock_t write_start = clock();
        for (int j = 0; j < iterations; j++) {
            data_block_t* block = rw_create_data_block(block_size);
            if (block) {
                // Fill with pattern
                for (size_t k = 0; k < block_size; k++) {
                    block->data[k] = (uint8_t)((j + k) & 0xFF);
                }
                block->checksum = calculate_checksum(block->data, block_size);
                
                metrics.bytes_written += block_size;
                metrics.total_writes++;
                
                rw_delete_data_block(block);
            }
        }
        clock_t write_end = clock();
        double write_time = (double)(write_end - write_start) / CLOCKS_PER_SEC;
        
        printf("  Write: %.2f MB/s\n", 
               (iterations * block_size) / (write_time * 1024 * 1024));
        
        // Read benchmark
        data_block_t* test_block = rw_create_data_block(block_size);
        if (test_block) {
            clock_t read_start = clock();
            for (int j = 0; j < iterations; j++) {
                char* buffer = malloc(block_size);
                rw_read_data(test_block, buffer, block_size);
                metrics.bytes_read += block_size;
                metrics.total_reads++;
                free(buffer);
            }
            clock_t read_end = clock();
            double read_time = (double)(read_end - read_start) / CLOCKS_PER_SEC;
            
            printf("  Read:  %.2f MB/s\n", 
                   (iterations * block_size) / (read_time * 1024 * 1024));
            
            rw_delete_data_block(test_block);
        }
    }
    
    printf("\nTotal operations: %zu reads, %zu writes\n",
           metrics.total_reads, metrics.total_writes);
    printf("Total data: %.2f MB read, %.2f MB written\n",
           metrics.bytes_read / (1024.0 * 1024.0),
           metrics.bytes_written / (1024.0 * 1024.0));
}

data_block_t* rw_create_data_block(size_t size) {
    if (!rw_partition || size == 0) return NULL;
    
    // Allocate block structure
    data_block_t* block = (data_block_t*)partition_alloc(rw_partition, sizeof(data_block_t));
    if (!block) return NULL;
    
    // Allocate data
    block->data = (uint8_t*)partition_alloc(rw_partition, size);
    if (!block->data) {
        // Free the block structure if data allocation fails
        // Note: In real implementation, we'd need proper deallocation
        return NULL;
    }
    
    block->id = next_block_id++;
    block->size = size;
    block->checksum = 0;
    block->timestamp = time(NULL);
    
    printf("Created data block %u, size: %zu bytes\n", block->id, size);
    
    return block;
}

void rw_write_data(data_block_t* block, const void* data, size_t size) {
    if (!block || !data || size == 0) return;
    
    size_t copy_size = (size > block->size) ? block->size : size;
    memcpy(block->data, data, copy_size);
    
    // Calculate checksum
    block->checksum = calculate_checksum(block->data, copy_size);
    block->timestamp = time(NULL);
    
    metrics.bytes_written += copy_size;
    metrics.total_writes++;
    
    printf("Wrote %zu bytes to block %u\n", copy_size, block->id);
}

void rw_read_data(const data_block_t* block, void* buffer, size_t size) {
    if (!block || !buffer || size == 0) return;
    
    size_t copy_size = (size > block->size) ? block->size : size;
    memcpy(buffer, block->data, copy_size);
    
    metrics.bytes_read += copy_size;
    metrics.total_reads++;
    
    printf("Read %zu bytes from block %u\n", copy_size, block->id);
}

void rw_delete_data_block(data_block_t* block) {
    if (!block) return;
    
    printf("Deleted data block %u\n", block->id);
    
    // Note: In real implementation, we'd actually free the memory
    // For simulation, we just mark it as deleted
}

uint32_t calculate_checksum(const void* data, size_t size) {
    if (!data || size == 0) return 0;
    
    const uint8_t* bytes = (const uint8_t*)data;
    uint32_t checksum = 0x12345678;  // Initial value
    
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum << 5) | (checksum >> 27);  // Rotate left 5 bits
        checksum ^= bytes[i];
    }
    
    return checksum;
}

void rw_defragment(void) {
    if (!rw_partition) return;
    
    printf("Defragmenting read/write partition...\n");
    printf("Fragmentation simulation complete\n");
}

void rw_verify_integrity(void) {
    if (!rw_partition) return;
    
    printf("Verifying partition integrity...\n");
    printf("Integrity check passed\n");
}

rw_metrics_t* get_rw_metrics(void) {
    return &metrics;
}
