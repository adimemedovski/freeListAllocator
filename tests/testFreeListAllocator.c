#include "unity.h"
#include "freeListAllocator.h"
#include <sys/mman.h>
#include <unistd.h>

void setUp(void) {

}

void tearDown(void) {

}

void testInitFreeList(void) {

}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitFreeList);
    RUN_TEST(testFreeListAlloc);
    RUN_TEST(testFreeAlloc);

    return UNITY_END();
}
