//
// Created by escha on 26.02.25.
//

#include <string.h>
#include <sys/mman.h>
#include <malloc.h>

#include "arena.h"

enum {
    REGION_DEFAULT_CAPACITY = 8192
};

#define ARENA_MMAP

static void *alloc_region(const size_t size) {
    const size_t nbytes = sizeof(Region) + sizeof(uintptr_t) * size;
#ifdef ARENA_MMAP
    Region *r = mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
#elif defined(ARENA_MALLOC)
    Region *r = malloc(nbytes);
#else
#error "Invalid implementation"
#endif
    r->next = NULL;
    r->count = 0;
    r->size = size;

    return r;
}

void *arena_alloc(Arena *arena, const size_t nbytes) {
    const size_t size = (nbytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    if (arena->end == NULL) {
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if (capacity < size) capacity = size;
        arena->end = alloc_region(capacity);
        arena->begin = arena->end;
    }

    while (arena->end->count + size > arena->end->size && arena->end->next != NULL) {
        arena->end = arena->end->next;
    }

    if (arena->end->count + size > arena->end->size) {
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if (capacity < size) capacity = size;
        arena->end->next = alloc_region(capacity);
        arena->end = arena->end->next;
    }

    void *result = arena->end->data + arena->end->count;
    arena->end->count += size;

    return result;
}

void *arena_calloc(Arena *arena, const size_t nbytes) {
    void *data = arena_alloc(arena, nbytes);
    memset(data, 0, nbytes);

    return data;
}

void *arena_realloc(Arena *arena, const size_t nbytes) {
    (void) arena;
    (void) nbytes;
    return NULL;
}

void *arena_reallocarray(Arena *arena, const size_t nbytes) {
    (void) arena;
    (void) nbytes;

    return NULL;
}

char *arena_strdup(Arena *arena, const char *str) {
    const size_t len = strlen(str);
    char *dup = arena_alloc(arena, len + 1);
    memmove(dup, str, len + 1); // memcpy potentially unsafe in arenas

    return dup;
}


