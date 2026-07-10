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
    
    /*
     * Test One: Testing basic functionality.
     */
    TEST_ASSERT_TRUE(initFreeList(&freeList));
    
    /*
     * Cleanup.
     */
    munmap(freeList.basePtr, MEMORY_REGION_SIZE);
}


int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(testInitFreeList);

    return UNITY_END();
}
