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
    
    munmap(freeList.memoryBasePtr, MAX_MEMORY_SIZE);
}

void testFreeListAlloc(void) {
    FreeList freeList;
    initFreeList(&freeList);

    size_t *ptrOne = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 2, _Alignof(size_t));
    
    TEST_ASSERT_TRUE(ptrOne != NULL);
    ptrOne[0] = 3;
    ptrOne[1] = 2;
    ptrOne[2] = 1;

    TEST_ASSERT_EQUAL_size_t(3, ptrOne[0]);
    TEST_ASSERT_EQUAL_size_t(2, ptrOne[1]);
    TEST_ASSERT_EQUAL_size_t(1, ptrOne[2]);
    
    size_t *ptrTwo = (size_t*) freeListAlloc(&freeList, sizeof(size_t) * 2, _Alignof(size_t));
    ptrTwo[0] = 666;

    TEST_ASSERT_EQUAL_size_t(3, ptrOne[0]);
    TEST_ASSERT_EQUAL_size_t(2, ptrOne[1]);
    TEST_ASSERT_EQUAL_size_t(1, ptrOne[2]);
    
    TEST_ASSERT_EQUAL_size_t(1, ptrTwo[0]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitFreeList);
    RUN_TEST(testFreeListAlloc);

    return UNITY_END();
}
