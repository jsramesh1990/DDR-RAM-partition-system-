
## 3. include/config.h

```c
#ifndef CONFIG_H
#define CONFIG_H

// Memory configuration
#define TOTAL_DDR_SIZE         (768 * 1024 * 1024)  // 768MB
#define PARTITION_SIZE         (256 * 1024 * 1024)  // 256MB per partition

// Partition addresses
#define GAMING_PARTITION_BASE   0x00000000
#define RW_PARTITION_BASE       0x10000000
#define USERSPACE_PARTITION_BASE 0x20000000

// Memory protection flags
#define MEM_READ_ONLY     0x01
#define MEM_READ_WRITE    0x02
#define MEM_EXECUTE       0x04
#define MEM_NO_ACCESS     0x08

// Error codes
#define MEM_SUCCESS       0
#define MEM_ERROR         -1
#define MEM_FULL          -2
#define MEM_INVALID       -3
#define MEM_PROTECTED     -4

#endif // CONFIG_H
