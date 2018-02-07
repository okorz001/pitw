#include <stdlib.h>
#include <string.h>

#include <pitw/mem.h>

static pitw_mem_ctx ctx = {malloc, free};

pitw_mem_ctx pitw_mem_get_ctx(void) {
    return ctx;
}

void pitw_mem_set_ctx(pitw_mem_ctx *new_ctx) {
    ctx = *new_ctx;
}

void *pitw_mem_alloc(size_t size) {
    return ctx.alloc(size);
};

void *pitw_mem_alloc_zero(size_t size) {
    void *ptr = pitw_mem_alloc(size);
    memset(ptr, 0, size);
    return ptr;
};

void pitw_mem_free(void *ptr) {
    ctx.free(ptr);
};
