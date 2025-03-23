#include <stdio.h>
#include <assert.h>
#include "MemoryAllocator.c"  // Підключаємо основний файл для тестування

void test_mem_alloc_free() {
    printf("Testing mem_alloc and mem_free...\n");
    
    void *ptr1 = mem_alloc(128);
    assert(ptr1 != NULL);
    
    void *ptr2 = mem_alloc(256);
    assert(ptr2 != NULL);
    
    mem_free(ptr1);
    mem_free(ptr2);
    
    printf("mem_alloc and mem_free passed.\n");
}

void test_mem_realloc() {
    printf("Testing mem_realloc...\n");
    
    void *ptr = mem_alloc(64);
    assert(ptr != NULL);

    void *new_ptr = mem_realloc(ptr, 128);
    assert(new_ptr != NULL);
    
    mem_free(new_ptr);

    printf("mem_realloc passed.\n");
}

void test_large_alloc() {
    printf("Testing large allocation...\n");

    void *ptr = mem_alloc(10 * PAGE_SIZE); // Більше ніж дефолтна арена
    assert(ptr != NULL);

    mem_free(ptr);

    printf("Large allocation test passed.\n");
}

void test_mem_show() {
    printf("Testing mem_show...\n");
    void *ptr1 = mem_alloc(128);
    void *ptr2 = mem_alloc(256);
    
    mem_show();
    
    mem_free(ptr1);
    mem_free(ptr2);
    
    printf("mem_show test passed.\n");
}

int main() {
    test_mem_alloc_free();
    test_mem_realloc();
    test_large_alloc();
    test_mem_show();

    printf("All tests passed!\n");
    return 0;
}
