#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define MEMORY_REGION_SIZE ((size_t) 4096)

typedef struct {
    size_t allocatedAmount; // In bytes.  
    size_t paddingb; // In bytes.
    size_t alignment; // In bytes.
} MetaData;

typedef struct Block {
    MetaData metaData; 
    size_t blockSize; // Block size in bytes.
    void *ptr; // Pointer to start of block in memory.
    struct Block *nextFreeBlock; // Next free block.
} Block;
 
typedef struct {
    void *basePtr;
    Block nextFreeBlock;
} FreeList;

bool initFreeList(FreeList *freeList);





