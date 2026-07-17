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

static bool validateFreeList(FreeList *freeList) {
    if (freeList == NULL || freeList == (FreeList*) NULL) {
        fprintf(stderr, "Error: Failed to validate free list as freeList is NULL.\n");
        return false;
    }

    if (freeList -> memoryBasePtr == NULL) {
        fprintf(stderr, "Error: Failed to validate free list as memoryBasePtr is NULL.\n");
        return false;
    }

    if (freeList -> memoryBasePtr == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to validate free list as memoryBasePtr is MAP_FAILED.\n");
        return false;
    }
    
    if (freeList -> head == (Block*) NULL || freeList -> head == NULL) {
        fprintf(stderr, "Error: Failed to validate free list as head is NULL.\n");
        return false;
    }

    return true; 
}

static bool validateParamsOfFreeListAlloc(FreeList *freeList, size_t blockSize, size_t alignment) {
    if (!validateFreeList(freeList)) {
        fprintf(stderr, "Error: Failed to call freeListAlloc as validation of params failed.\n");
        return false;
    } 
    
    if (blockSize == 0) {
        fprintf(stderr, "Error: Failed to call freeListAlloc as block size cannot be zero.\n");
        return false;
    }

    if (alignment == 0) {
        fprintf(stderr, "Error: Failed to call freeListAlloc as alignment cannot be zero.\n");
        return false;
    }

    return true;
}

        
static void handleFreeListNextIsNULL(FreeList *freeList, void *ptrToReturn, size_t blockSize) {
    char* newHead = (char*) freeList -> head;
    newHead += (char) ptrToReturn + blockSize; // Seg fault when I used intptr_t instead of char?

    size_t headPadding = getAlignmentPadding((void*) newHead, _Alignof(Block));

    newHead += headPadding;

    Block *head = (Block*) newHead;
    head -> blockSize = ((intptr_t) freeList -> memoryBasePtr) + MAX_MEMORY_SIZE - ((intptr_t) newHead); // need to validate this functionality.
    head -> next = (Block*) NULL; 

    freeList -> head = head;
} 

static MetaData *getAlignedMetaData(FreeList *freeList, char* ptrToHead) {
    char *metaDataPtr = ptrToHead + sizeof(Block); // prevents overwriting data in freeList -> head.  
    size_t metaDataAlignmentPadding = getAlignmentPadding((void*) metaDataPtr, _Alignof(MetaData));    
    
    metaDataPtr += metaDataAlignmentPadding;
    
    MetaData *metaData = (MetaData*) metaDataPtr;
    metaData -> padding = metaDataAlignmentPadding;
    
    return metaData;
}

static void *getAlignedPointer(MetaData *metaData, size_t alignment) {
    char *ptrToReturn = (char*) metaData; 
    ptrToReturn += sizeof(MetaData);
    
    size_t ptrToReturnAlignmentPadding = getAlignmentPadding((void*) ptrToReturn, alignment);
    
    ptrToReturn += ptrToReturnAlignmentPadding;
    char *newMetaDataPtr = (char*) metaData; 
    newMetaDataPtr += ptrToReturnAlignmentPadding;
    metaData = (MetaData*) newMetaDataPtr;
    metaData -> padding += ptrToReturnAlignmentPadding;
    
    return (void*) ptrToReturn;
}

/*
 * Need to add overflow checks.
 */
void *freeListAlloc(FreeList *freeList, size_t blockSize, size_t alignment) {
    if (!validateParamsOfFreeListAlloc(freeList, blockSize, alignment)) {
        fprintf(stderr, "Error: Failed to call freeListAlloc as validation of its params failed.\n");
        return NULL;
    } 
    
    MetaData *metaData = getAlignedMetaData(freeList, (char*) freeList -> head);
    void *ptrToReturn = getAlignedPointer(metaData, alignment);

    if (freeList -> head -> next == (Block*) NULL) {
        handleFreeListNextIsNULL(freeList, (char*) ptrToReturn, blockSize); 
        return (void*) ptrToReturn;
    }
   
    freeList -> head = freeList -> head -> next;
     
    return (void*) ptrToReturn;
}

/*
 * We should add a case where we determine whether there is any pointer to be freed at all.
 */
static bool validateParamsOfFreeAlloc(FreeList *freeList, void *ptr) {
    if (!validateFreeList(freeList)) {
        fprintf(stderr, "Error: Failed to call freeAlloc as validation of freeList failed.\n");
        return false;
    }

    if (ptr == NULL) {
        fprintf(stderr, "Error: Failed to call freeAlloc as ptr is NULL.\n");
        return false;
    }

    return true;
}

bool freeAlloc(FreeList *freeList, void* ptr) {
    if (!validateParamsOfFreeAlloc(freeList, ptr)) {
        fprintf(stderr, "Error: Failed to call freeAlloc as validation of its params faileds.\n");
        return false;
    } 

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












