#ifndef PITW_OBJ_H
#define PITW_OBJ_H

#include <pitw/config.h>
#include <stdint.h>

/**
 * @file
 * Dynamically typed objects with prototypical inheritance.
 */

/**
 * A dynamic object.
 */
typedef struct pitw_obj pitw_obj;

/**
 * Creates a new object.
 * @param proto Optional prototype for the new object.
 */
PITW_API pitw_obj *pitw_obj_create(pitw_obj *proto);

/**
 * Increases the refernce count of an object.
 * @param obj The object.
 * @return The object.
 */
PITW_API pitw_obj *pitw_obj_up(pitw_obj *obj);

/**
 * Decreases the reference count of an object.
 * The object should not be used after this point as it may have been
 * deallocated.
 * @param obj The object.
 */
PITW_API void pitw_obj_down(pitw_obj *obj);

PITW_API void pitw_obj_remove(pitw_obj *obj, char *key);

PITW_API int pitw_obj_get_o(pitw_obj *obj, char *key, pitw_obj **out);
PITW_API void pitw_obj_set_o(pitw_obj *obj, char *key, pitw_obj *val);

PITW_API int pitw_obj_get_p(pitw_obj *obj, char *key, void **out);
PITW_API void pitw_obj_set_p(pitw_obj *obj, char *key, void *val);

PITW_API int pitw_obj_get_i(pitw_obj *obj, char *key, intmax_t *out);
PITW_API void pitw_obj_set_i(pitw_obj *obj, char *key, intmax_t val);

PITW_API int pitw_obj_get_f(pitw_obj *obj, char *key, float *out);
PITW_API void pitw_obj_set_f(pitw_obj *obj, char *key, float val);

PITW_API int pitw_obj_get_d(pitw_obj *obj, char *key, double *out);
PITW_API void pitw_obj_set_d(pitw_obj *obj, char *key, double val);

#endif /* PITW_OBJ_H */
