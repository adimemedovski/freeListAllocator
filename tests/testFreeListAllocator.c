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
    TEST_ASSERT_TRUE(freeList.ptrToVirtualAddressSpace != NULL);
    TEST_ASSERT_TRUE(freeList.ptrToVirtualAddressSpace != MAP_FAILED);
    TEST_ASSERT_EQUAL_size_t(MEMORY_REGION_SIZE, freeList.head.blockSize);
    TEST_ASSERT_TRUE(freeList.head.ptr == freeList.ptrToVirtualAddressSpace);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(testInitFreeList);

    return UNITY_END();
}
