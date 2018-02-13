#include <pitw/hash.h>
#include <pitw/mem.h>
#include <pitw/obj.h>
#include <stdlib.h>

#include "atomic.h"

struct pitw_obj {
    pitw_atomic count;
    pitw_obj *proto;
    pitw_hash *attrs;
};

/* attr value with type metadata for safe access */
struct attr {
    enum {
        ATTR_OBJECT,
        ATTR_POINTER,
        ATTR_INT,
        ATTR_FLOAT,
        ATTR_DOUBLE
    } type;
    union {
        pitw_obj *o;
        void *p;
        intmax_t i;
        float f;
        double d;
    } val;
};

pitw_obj *pitw_obj_create(pitw_obj *proto) {
    pitw_obj *obj = pitw_mem_alloc(sizeof *obj);
    obj->count = 0;
    obj->proto = pitw_obj_up(proto);
    obj->attrs = pitw_hash_create();
    return obj;
}

pitw_obj *pitw_obj_up(pitw_obj *obj) {
    if (obj) pitw_atomic_inc(obj->count);
    return obj;
}

void pitw_obj_down(pitw_obj *obj) {
    if (obj && !pitw_atomic_dec(obj->count)) {
        pitw_obj_down(obj->proto);
        pitw_hash_free(obj->attrs);
        pitw_mem_free(obj);
    }
}

void pitw_obj_remove(pitw_obj *obj, char *key) {
    /* TODO: double lookup is nasty */
    pitw_mem_free(pitw_hash_get(obj->attrs, key));
    pitw_hash_remove(obj->attrs, key);
}

int pitw_obj_get_i(pitw_obj *obj, char *key, intmax_t *out) {
    struct attr *attr = pitw_hash_get(obj->attrs, key);
    if (!attr)
        return 0;
    if (attr->type != ATTR_INT)
        return 0;
    *out = attr->val.i;
    return 1;
}

void pitw_obj_set_i(pitw_obj *obj, char *key, intmax_t val) {
    /* TODO: double lookup is nasty */
    struct attr *old = pitw_hash_get(obj->attrs, key);
    if (old)
        pitw_mem_free(old);

    struct attr *attr = pitw_mem_alloc(sizeof *attr);
    attr->type = ATTR_INT;
    attr->val.i = val;
    pitw_hash_set(obj->attrs, key, attr);
}
