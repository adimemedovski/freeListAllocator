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
    
    TEST_ASSERT_TRUE(freeList.memoryBasePtr != NULL);
    TEST_ASSERT_TRUE(freeList.memoryBasePtr != MAP_FAILED);

    /*
     * Clean up.
     */
    munmap(freeList.memoryBasePtr, MAX_MEMORY_SIZE);
}

void testFreeListAlloc(void) {
    FreeList freeList;
    TEST_ASSERT_TRUE(initFreeList(&freeList));
    
    size_t *ptrOne = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 2, _Alignof(size_t));
    ptrOne[0] = 1;
    ptrOne[1] = 2;

    TEST_ASSERT_EQUAL_size_t(1, ptrOne[0]);
    TEST_ASSERT_EQUAL_size_t(2, ptrOne[1]);
    
    size_t *ptrTwo = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 3, _Alignof(size_t));
    ptrTwo[0] = 33;
    ptrTwo[1] = 66;
    ptrTwo[2] = 99;

    TEST_ASSERT_EQUAL_size_t(1, ptrOne[0]);
    TEST_ASSERT_EQUAL_size_t(2, ptrOne[1]);
 
    TEST_ASSERT_EQUAL_size_t(33, ptrTwo[0]);
    TEST_ASSERT_EQUAL_size_t(66, ptrTwo[1]);
    TEST_ASSERT_EQUAL_size_t(99, ptrTwo[2]);

    /*
     * Clean up.
     */
    munmap(freeList.memoryBasePtr, MAX_MEMORY_SIZE);
}

void testFreeAlloc(void) {
    FreeList freeList;
    initFreeList(&freeList);

    size_t *ptrOne = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 2, _Alignof(size_t));
    ptrOne[0] = 1;
    ptrOne[1] = 2;

    size_t *ptrTwo = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 3, _Alignof(size_t));
    ptrTwo[0] = 33;
    ptrTwo[1] = 66;
    ptrTwo[2] = 99;

    freeAlloc(&freeList, ptrOne);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitFreeList);
    RUN_TEST(testFreeListAlloc);

    return UNITY_END();
}
