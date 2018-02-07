#ifndef PITW_MEM_H
#define PITW_MEM_H

#include <stddef.h>

#include <pitw/config.h>

/**
 * @file
 * @ingroup util
 * Controls how pitw allocates and deallocates memory. There is a single global
 * memory allocation context and access is not synchronized. Thus, it is
 * generally only safe to set the allocation context before using pitw.
 *
 * This module is intended to better identify memory leaks in tests and debug
 * builds.
 */

/**
 * Memory allocation context.
 */
typedef struct {
    /**
     * Memory allocation function.
     * @param size Number of bytes to allocate.
     * @return Allocated memory.
     */
    void *(*alloc)(size_t size);

    /**
     * Memory deallocation function.
     * @param free Pointer to memory to free. Must be pointer previously
     *             returned by corresponding allocation function.
     */
    void (*free)(void *ptr);
} pitw_mem_ctx;

/**
 * Returns the current memory allocation context.
 */
PITW_API pitw_mem_ctx pitw_mem_get_ctx(void);

/**
 * Sets the memory allocation context.
 */
PITW_API void pitw_mem_set_ctx(pitw_mem_ctx *ctx);

/**
 * Allocates memory, like malloc.
 * @param size Number of bytes to allocate.
 */
PITW_API void *pitw_mem_alloc(size_t size);

/**
 * Allocates and zeroes memory, like malloc and memset.
 * @param size Number of bytes to allocate.
 */
PITW_API void *pitw_mem_alloc_zero(size_t size);

/**
 * Deallocates memory, like free.
 * @param ptr Pointer to memory to free.
 */
PITW_API void pitw_mem_free(void *ptr);

#endif /* PITW_MEM_H */
