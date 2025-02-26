//
// Created by escha on 26.02.25.
//

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct Region Region;

struct Region
{
    Region *next;
    size_t count;
    size_t size;
    uintptr_t data[];
};

typedef struct
{
    Region *begin, *end;
} Arena;

void* arena_alloc(Arena *arena, size_t nbytes);
void *arena_calloc(Arena *arena, size_t nbytes);
void* arena_realloc(Arena *arena, size_t nbytes);
void* arena_reallocarray(Arena *arena, size_t nbytes);
char* arena_strdup(Arena *arena, const char *str);
