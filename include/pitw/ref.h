#ifndef PITW_REF_H
#define PITW_REF_H

#include <stddef.h>

#include <pitw/config.h>

/**
 * @file
 * Reference counted pointers.
 * @ingroup util
 */

/**
 * A reference counted pointer.
 */
typedef struct pitw_ref pitw_ref;

/**
 * Creates a new reference counted pointer with a count of one.
 * The count is initialized to one, so callers SHOULD NOT immediately call
 * #pitw_ref_up on the returned pointer.
 * @param ptr The underlying pointer.
 * @param dealloc Optional callback to free underlying pointer after the
 *                reference count is zero.
 */
PITW_API pitw_ref *pitw_ref_create(void *ptr, void (*dealloc)(void *));

/**
 * Returns the underlying pointer.
 * @param ref The reference counted pointer.
 */
PITW_API void *pitw_ref_get(pitw_ref *ref);

/**
 * Increases the reference count by one.
 * @param ref The reference counted pointer.
 * @return The reference counted pointer.
 */
PITW_API pitw_ref *pitw_ref_up(pitw_ref *ref);

/**
 * Decreases the reference count by one, possibly triggering deallocation.
 * @param ref The reference counted pointer.
 */
PITW_API void pitw_ref_down(pitw_ref *ref);

#endif /* PITW_REF_H */
