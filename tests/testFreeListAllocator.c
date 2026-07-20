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
    

}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitFreeList);

    return UNITY_END();
}
