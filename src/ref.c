#include <stdlib.h>
#include <string.h>

#include "pitw/mem.h"
#include "pitw/ref.h"
#include "atomic.h"

struct pitw_ref {
    pitw_atomic count;
    void (*dealloc)(void *);
    void *ptr;
};

pitw_ref *pitw_ref_create(void *ptr, void (*dealloc)(void *)) {
    pitw_ref *ref = pitw_mem_alloc(sizeof(*ref));
    ref->count = 1;
    /* if no user function is provided, just use free */
    ref->dealloc = dealloc ? dealloc : free;
    ref->ptr = ptr;
    return ref;
}

pitw_ref *pitw_ref_up(pitw_ref *ref) {
    if (ref) pitw_atomic_inc(ref->count);
    return ref;
}

void pitw_ref_down(pitw_ref *ref) {
    if (ref && !pitw_atomic_dec(ref->count)) {
        ref->dealloc(ref->ptr);
        /* help catch use after free */
        ref->ptr = NULL;
        pitw_mem_free(ref);
    }
}

void *pitw_ref_get(pitw_ref *ref) {
    return ref->ptr;
}
