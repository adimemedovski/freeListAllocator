#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "freeListAllocator.h" 

static int getAlignmentPadding(void* ptr, size_t alignment) {
    if (alignment == 0) {
        fprintf(stderr, "Error: Faild to get alignment padding due to division by zero error.\n");
        return -1;
    }

    if (ptr == NULL) {
        fprintf(stderr, "Error: Failed to get alignment padding as ptr is NULL.\n");
        return -1;
    }
    
    size_t address = (size_t) ptr;
    
    if (address % alignment == 0) {
        return 0;
    }

    return alignment - (address % alignment); 
}

bool initFreeList(FreeList *freeList) {
    if (freeList == NULL || freeList == (FreeList*) NULL) {
        fprintf(stderr, "Error: Failed to init freeList as freeList is NULL.\n");
        return false;
    }

    freeList -> memoryBasePtr = (void*) mmap(NULL, MAX_MEMORY_SIZE,
            PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);
    
    if (freeList -> memoryBasePtr == MAP_FAILED || freeList -> memoryBasePtr == NULL) {
        fprintf(stderr, "Error: Failed to init freeList as mmap returned MAP_FAILED.\n");
        return false;
    }
    
    freeList -> head = (Block*) freeList -> memoryBasePtr;
    freeList -> head -> next = (Block*) NULL;
    freeList -> head -> blockSize = MAX_MEMORY_SIZE; 
    
    return true;
}

void *freeListAlloc(FreeList *freeList, size_t blockSize, size_t alignment) {
    char *ptrToReturn = (char*) freeList -> head;
    ptrToReturn += sizeof(Block); // Avoids overwriting data in freeList -> head.
    
    MetaData *metaData = (MetaData*) ptrToReturn;
    metaData -> padding = getAlignmentPadding((void*) metaData, _Alignof(MetaData));
    metaData += metaData -> padding;

    ptrToReturn += metaData -> padding + sizeof(MetaData);
    
    metaData -> allocatedMemory = blockSize;
    
    if (freeList -> head -> next == (Block*) NULL) {
        char* newHead = (char*) freeList -> head;
        newHead += (char) ptrToReturn + blockSize;
        
        Block *head = (Block*) newHead;
        head -> blockSize = ((intptr_t) freeList -> memoryBasePtr) + MAX_MEMORY_SIZE - ((intptr_t) newHead); // producing seg fault error.
        head -> next = (Block*) NULL; 
        
        freeList -> head = head; 
        
        return (void*) ptrToReturn;
    }
   
    freeList -> head = freeList -> head -> next;
   
    return (void*) ptrToReturn;
}

bool freeAlloc(FreeList *freeList, void* ptr) {
    char *ptrToMetaData = (char*) ptr;
    ptrToMetaData -= sizeof(MetaData);
    
    MetaData *metaData = (MetaData*) ptrToMetaData;

    char* ptrToFreeBlock = (char*) metaData;
    ptrToFreeBlock -= metaData -> padding;
    
    Block *freeBlock = (Block*) ptrToFreeBlock;
    freeBlock -> blockSize = metaData -> padding + sizeof(MetaData) + metaData -> allocatedMemory;
    
    freeBlock -> next = freeList -> head;
    freeList -> head = freeBlock;
    
    return true;
}











