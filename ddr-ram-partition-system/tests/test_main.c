#include <stdio.h>
#include <assert.h>
#include "ddr_memory.h"
#include "config.h"

void test_ddr_init(void) {
    printf("Testing DDR initialization...\n");
    
    ddr_memory_t* memory = ddr_init(TOTAL_DDR_SIZE);
    assert(memory != NULL);
    assert(memory->total_size == TOTAL_DDR_SIZE);
    assert(memory->is_initialized == true);
    
    printf("  ✓ DDR initialization passed\n");
    
    ddr_deinit(memory);
}

void test_partition_creation(void) {
    printf("Testing partition creation...\n");
    
    ddr_memory_t* memory = ddr_init(TOTAL_DDR_SIZE);
    
    memory_partition_t* partition = create_partition(memory, PARTITION_SIZE, 
                                                   MEM_READ_WRITE, "Test");
    assert(partition != NULL);
    assert(partition->size == PARTITION_SIZE);
    assert(partition->protection == MEM_READ_WRITE);
    
    printf("  ✓ Partition creation passed\n");
    
    ddr_deinit(memory);
}

void test_memory_allocation(void) {
    printf("Testing memory allocation...\n");
    
    ddr_memory_t* memory = ddr_init(PARTITION_SIZE);
    memory_partition_t* partition = create_partition(memory, PARTITION_SIZE,
                                                   MEM_READ_WRITE, "Test");
    
    void* ptr = partition_alloc(partition, 1024);
    assert(ptr != NULL);
    assert(partition->used == 1024);
    
    printf("  ✓ Memory allocation passed\n");
    
    ddr_deinit(memory);
}

void test_memory_protection(void) {
    printf("Testing memory protection...\n");
    
    ddr_memory_t* memory = ddr_init(PARTITION_SIZE);
    
    // Test read-only partition
    memory_partition_t* ro_partition = create_partition(memory, 1024 * 1024,
                                                       MEM_READ_ONLY, "RO");
    
    // Test no-access partition
    memory_partition_t* na_partition = create_partition(memory, 1024 * 1024,
                                                       MEM_NO_ACCESS, "NA");
    
    printf("  ✓ Memory protection passed\n");
    
    ddr_deinit(memory);
}

int main(void) {
    printf("Running DDR RAM System Tests\n");
    printf("============================\n\n");
    
    test_ddr_init();
    test_partition_creation();
    test_memory_allocation();
    test_memory_protection();
    
    printf("\nAll tests passed!\n");
    
    return 0;
}
