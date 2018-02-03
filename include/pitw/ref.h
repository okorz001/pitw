#ifndef PITW_REF_H
#define PITW_REF_H

#include <stddef.h>

#include <pitw/config.h>

/**
 * @file
 * @ingroup util
 * Thread-safe reference counted pointers.
 *
 * A #pitw_ref contains a pointer to arbitrary data and a reference count. When
 * the reference count reaches zero, the pointer is freed. Reference counted
 * pointers make it much easier to share data in asynchronous environments such
 * as event loops or with worker threads. The reference count is updated
 * atomically and without locking to minimize overhead.
 *
 * Basic usage of a #pitw_ref:
 * @code
 * // Some dynamically allocated thing, in this case a string.
 * char *str = strdup("Hello world!");
 *
 * // Create a reference to it. Note the reference count is initialized to 1.
 * // If no deallocation function is provided, the standard free will be used.
 * pitw_ref *ref = pitw_ref_create(str, NULL);
 * // str SHOULD NOT be used directly after this point.
 *
 * // Do something with it...
 * printf("My cool string: %s\n", pitw_ref_get(ref));
 *
 * // Pass the reference to other things...
 * set_thingy(ref);
 *
 * // Decrease reference count when we are done.
 * pitw_ref_down(ref);
 * // ref MUST NOT be used after this point.
 * @endcode
 *
 * Using an existing #pitw_ref:
 * @code
 * // Immediately increment the reference count to prevent deallocation.
 * pitw_ref_up(ref);
 *
 * // Do something with it...
 * do_something(pitw_ref_get(ref));
 *
 * // Again, decrease the reference count when we are done.
 * pitw_ref_down(ref);
 * // ref MUST NOT be used after this point.
 * @endcode
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
 *                reference count is zero. If absent, free from the C
 *                standard library will be used.
 */
PITW_API pitw_ref *pitw_ref_create(void *ptr, void (*dealloc)(void *));

/**
 * Increases the reference count by one.
 * @param ref The reference counted pointer.
 * @return The reference counted pointer.
 */
PITW_API pitw_ref *pitw_ref_up(pitw_ref *ref);

/**
 * Decreases the reference count by one, possibly triggering deallocation.
 * The reference MUST NOT be used after calling this function as it may have
 * been deallocated.
 * @param ref The reference counted pointer.
 */
PITW_API void pitw_ref_down(pitw_ref *ref);

/**
 * A convenience macro for clearing a #pitw_ref pointer.
 * This is functionally equivalent to the following:
 * @code
 * pitw_ref_down(ref);
 * ref = NULL;
 * @endcode
 * @param ref The reference to clear.
 */
#define pitw_ref_clear(ref) \
    do { \
        pitw_ref_down(ref); \
        ref = NULL; \
    } while(0)

/**
 * A convenience macro for setting a #pitw_ref pointer to another #pitw_ref.
 * This is functionally equivalent to the following:
 * @code
 * pitw_ref_down(ref);
 * ref = pitw_ref_up(other);
 * @endcode
 * @param ref The reference to set.
 * @param other The reference pointing to data to share.
 */
#define pitw_ref_set(ref, other) \
    do { \
        pitw_ref *temp = ref; \
        ref = pitw_ref_up(other); \
        pitw_ref_down(temp); \
    } while(0)

/**
 * Returns the underlying pointer.
 * @param ref The reference counted pointer.
 */
PITW_API void *pitw_ref_get(pitw_ref *ref);

#endif /* PITW_REF_H */
