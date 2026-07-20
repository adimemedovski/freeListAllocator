#include "unity.h"
#include "freeListAllocator.h"
#include <sys/mman.h>
#include <unistd.h>

void setUp(void) {

}

void tearDown(void) {

}

void testInitFreeList(void) {
    FreeList freeList;

    TEST_ASSERT_TRUE(initFreeList(&freeList));
    
    /*
     * Testing whether memoryBasePtr initialised properly.
     */
    TEST_ASSERT_TRUE(freeList.memoryBasePtr != NULL);
    TEST_ASSERT_TRUE(freeList.memoryBasePtr != MAP_FAILED);
   
    /*
     * Testing whether head initialised properly.
     */
    TEST_ASSERT_EQUAL_size_t(MAX_MEMORY_SIZE, freeList.head -> blockSize);
    TEST_ASSERT_TRUE(freeList.head -> next == NULL);
    
    /*
     * Cleanup.
     */
    munmap(freeList.memoryBasePtr, MAX_MEMORY_SIZE);
}

void testFreeListAlloc(void) {
    FreeList freeList;
    initFreeList(&freeList);
    
    /*
     * Testing first allocation functionality.
     */
    size_t *ptrOne = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 3, _Alignof(size_t));        
    

    /*
     * Testing if ptrOne is aligned in case where no padding is needed.
     */
    TEST_ASSERT_TRUE((size_t) ptrOne % _Alignof(size_t) == 0);
  
    /*
     * Testing if metaData is aligned.
     */
    char *retrieveMetaData = (char*) ptrOne;
    retrieveMetaData -= sizeof(MetaData);
    
    TEST_ASSERT_TRUE((size_t) retrieveMetaData % _Alignof(MetaData) == 0);
   
    /*
     * Testing if new block is ahead of ptr by blockSize and 
     * if it is aligned.
     */
    char *retrieveNewBlock = (char*) ptrOne;
    retrieveNewBlock += sizeof(size_t) * 3;
    
    TEST_ASSERT_TRUE(freeList.head == (Block*) retrieveNewBlock);
    TEST_ASSERT_TRUE((size_t) retrieveNewBlock % _Alignof(Block) == 0); 
    

    char* ptrTwo = (char*) freeListAlloc(&freeList, sizeof(char), _Alignof(char));
    TEST_ASSERT_TRUE((size_t) freeList.head % _Alignof(Block) == 0);  

    size_t *ptrThree = (size_t*) freeListAlloc(&freeList, sizeof(size_t), _Alignof(size_t));

    TEST_ASSERT_TRUE((size_t) ptrThree % _Alignof(size_t) == 0);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitFreeList);
    RUN_TEST(testFreeListAlloc);
    return UNITY_END();
}
