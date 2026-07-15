#include "unity.h"
#include "freeListAllocator.h"
#include <sys/mman.h>
#include <unistd.h>

void setUp(void) {

}

void tearDown(void) {

}

void testInitMemoryManager(void) {
    MemoryManager memoryManager;

    TEST_ASSERT_TRUE(initMemoryManager(&memoryManager));
    TEST_ASSERT_TRUE(memoryManager.ptrToVirtualAddressSpace != NULL); 
    TEST_ASSERT_TRUE(memoryManager.ptrToVirtualAddressSpace != MAP_FAILED); 
    
    /*
     * Clean up.
     */
    munmap(memoryManager.ptrToVirtualAddressSpace, MEMORY_REGION_SIZE);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitMemoryManager);

    return UNITY_END();
}
