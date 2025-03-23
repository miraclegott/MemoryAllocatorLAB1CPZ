#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

#define PAGE_SIZE 4096
#define DEFAULT_ARENA_SIZE (PAGE_SIZE * 4)

typedef struct BlockHeader {
    size_t size;
    int free;
    struct BlockHeader *next;
} BlockHeader;

typedef struct ArenaHeader {
    size_t size;
    struct ArenaHeader *next;
    BlockHeader *first_block;
} ArenaHeader;

static ArenaHeader *arena_list = NULL;

void *mem_alloc(size_t size);
void mem_free(void *ptr);
void *mem_realloc(void *ptr, size_t size);
void mem_show();

static ArenaHeader *request_new_arena(size_t size);
static BlockHeader *find_free_block(ArenaHeader **arena, size_t size);

void *mem_alloc(size_t size) {
    if (size == 0) return NULL;
    ArenaHeader *arena = arena_list;
    BlockHeader *block = find_free_block(&arena, size);
    if (!block) {
        arena = request_new_arena(size);
        if (!arena) return NULL;
        block = arena->first_block;
    }
    block->free = 0;
    return (void*)(block + 1);
}

void mem_free(void *ptr) {
    if (!ptr) return;
    BlockHeader *block = (BlockHeader*)ptr - 1;
    block->free = 1;
}

void *mem_realloc(void *ptr, size_t size) {
    if (!ptr) return mem_alloc(size);
    BlockHeader *block = (BlockHeader*)ptr - 1;
    if (block->size >= size) return ptr;
    void *new_ptr = mem_alloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        mem_free(ptr);
    }
    return new_ptr;
}

void mem_show() {
    ArenaHeader *arena = arena_list;
    while (arena) {
        printf("Arena %p, size: %zu\n", (void*)arena, arena->size);
        BlockHeader *block = arena->first_block;
        while (block) {
            printf("  Block %p, size: %zu, free: %d\n", (void*)block, block->size, block->free);
            block = block->next;
        }
        arena = arena->next;
    }
}

static ArenaHeader *request_new_arena(size_t size) {
    size_t arena_size = DEFAULT_ARENA_SIZE;
    if (size + sizeof(BlockHeader) > DEFAULT_ARENA_SIZE) {
        arena_size = size + sizeof(BlockHeader);
    }

    ArenaHeader *arena = (ArenaHeader*)VirtualAlloc(NULL, arena_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!arena) return NULL;

    arena->size = arena_size;
    arena->next = arena_list;
    arena_list = arena;

    BlockHeader *block = (BlockHeader*)(arena + 1);
    block->size = arena_size - sizeof(ArenaHeader) - sizeof(BlockHeader);
    block->free = 1;
    block->next = NULL;
    arena->first_block = block;

    return arena;
}

static BlockHeader *find_free_block(ArenaHeader **arena, size_t size) {
    ArenaHeader *cur_arena = arena_list;
    while (cur_arena) {
        BlockHeader *block = cur_arena->first_block;
        while (block) {
            if (block->free && block->size >= size) {
                return block;
            }
            block = block->next;
        }
        cur_arena = cur_arena->next;
    }
    return NULL;
}
