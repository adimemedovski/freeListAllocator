#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define MEMORY_REGION_SIZE ((size_t) 4096)

typedef struct {
    size_t allocatedAmount;  
} MetaData;

typedef struct Block {
    size_t blockSize; // Block size in bytes.
    void *ptr; // Pointer to start of block in memory.
    struct Block *nextBlock; // Next free block.
} Block;
 
typedef struct {
    void *basePtr;
    Block nextFreeBlock;
} FreeList;

bool initFreeList(FreeList *freeList);





