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
    
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testInitMemoryManager);

    return UNITY_END();
}
