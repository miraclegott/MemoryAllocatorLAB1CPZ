#include <stdio.h>
#include <assert.h>
#include "MemoryAllocator.c"  // Підключаємо основний файл для тестування

void test_mem_alloc_free() {
    printf("=== Testing mem_alloc and mem_free ===\n");
    
    void *ptr1 = mem_alloc(128);
    assert(ptr1 != NULL);
    printf("Allocated 128 bytes at %p\n", ptr1);
    mem_show();
    
    void *ptr2 = mem_alloc(256);
    assert(ptr2 != NULL);
    printf("Allocated 256 bytes at %p\n", ptr2);
    mem_show();
    
    mem_free(ptr1);
    printf("Freed 128 bytes\n");
    mem_show();
    
    mem_free(ptr2);
    printf("Freed 256 bytes\n");
    mem_show();
    
    printf("mem_alloc and mem_free passed.\n");
    printf("<------------------------------>\n");
}

void test_mem_realloc() {
    printf("=== Testing mem_realloc ===\n");
    
    void *ptr = mem_alloc(64);
    assert(ptr != NULL);
    printf("Allocated 64 bytes at %p\n", ptr);
    mem_show();
    
    void *new_ptr = mem_realloc(ptr, 128);
    assert(new_ptr != NULL);
    printf("Reallocated to 128 bytes at %p\n", new_ptr);
    mem_show();
    
    mem_free(new_ptr);
    printf("Freed 128 bytes\n");
    mem_show();
    
    printf("mem_realloc passed.\n");
    printf("<------------------------------>\n");
}

void test_large_alloc() {
    printf("=== Testing large allocation ===\n");
    
    void *ptr = mem_alloc(10 * PAGE_SIZE); // Більше ніж дефолтна арена
    assert(ptr != NULL);
    printf("Allocated large block (%d bytes) at %p\n", 10 * PAGE_SIZE, ptr);
    mem_show();
    
    mem_free(ptr);
    printf("Freed large block\n");
    mem_show();
    
    printf("Large allocation test passed.\n");
    printf("<------------------------------>\n");
}

void test_mem_show() {
    printf("=== Testing mem_show ===\n");
    
    void *ptr1 = mem_alloc(128);
    void *ptr2 = mem_alloc(256);
    printf("Allocated 128 and 256 bytes\n");
    mem_show();
    
    mem_free(ptr1);
    printf("Freed 128 bytes\n");
    mem_show();
    
    mem_free(ptr2);
    printf("Freed 256 bytes\n");
    mem_show();
    
    printf("mem_show test passed.\n");
    printf("<------------------------------>\n");
}

int main() {
    test_mem_alloc_free();
    test_mem_realloc();
    test_large_alloc();
    test_mem_show();
    
    printf("All tests passed!\n");
    return 0;
}