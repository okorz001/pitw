#ifndef PITW_UTIL_H
#define PITW_UTIL_H

#include <stddef.h>

/**
 * @file
 * @ingroup util
 * Miscellaneous utilities that don't have a more appropriate header.
 */

/**
 * Returns a pointer to the parent structure enclosing a field.
 * Given some structure instance:
 * @code
 * struct thing { int a; int b; } t;
 * @endcode
 * The following condition will be true:
 * @code
 * pitw_util_parent(&t.b, struct thing, b) == &t;
 * @endcode
 * This macro can be used to simulate inheritance between structures. Subtypes
 * are defined by embedding superclasses as fields anywhere in the structure.
 * @code
 * struct animal { ... };
 * struct dog {
 *     ...
 *     struct animal super;
 *     ...
 * };
 * @endcode
 * To upcast to a supertype, simply pass the address of the field:
 * @code
 * struct dog *d;
 * use_animal(&d->super);
 * @endcode
 * To downcast to the subtype, use this macro:
 * @code
 * struct animal *a;
 * use_dog(pitw_util_parent(a, struct dog, super));
 * @endcode
 * @param ptr A pointer to the enclosed field.
 * @param type The type of the enclosing structure.
 * @param field The field that the pointer is pointing to.
 */
#define pitw_util_parent(ptr, type, field) ((type *)((char *)ptr - offsetof(type, field)))

/**
 * "Downcasts" a pointer to a subtype.
 * This macro requires that the supertype is embedded in a field named "super".
 * This macro provides no runtime type checking.
 * @see pitw_util_parent
 * @param ptr A pointer to a supertype.
 * @param type The subtype to downcast to.
 */
#define pitw_util_downcast(ptr, type) pitw_util_parent(ptr, type, super)

#endif /* PITW_UTIL_H */
