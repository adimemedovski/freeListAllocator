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

    /*
     * Cleanup.
     */
    munmap(freeList.ptrToVirtualAddressSpace, MEMORY_REGION_SIZE);
}

void testFreeListAlloc(void) {
    FreeList freeList;
    initFreeList(&freeList);
    
    size_t *ptrOne = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 3, _Alignof(size_t)); 
    TEST_ASSERT_TRUE(ptrOne != NULL);
  
    TEST_ASSERT_TRUE(freeList.head.ptr == freeList.ptrToVirtualAddressSpace);

    size_t *ptrTwo = (size_t*) freeListAlloc(&freeList, sizeof(size_t), _Alignof(size_t));
    //ptrOne -= sizeof(MetaData);

    //TEST_ASSERT_EQUAL_size_t(sizeof(size_t) * 3, ptrOne[0]);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(testInitFreeList);
    RUN_TEST(testFreeListAlloc);

    return UNITY_END();
}
