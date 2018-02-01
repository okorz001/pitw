#include <stdlib.h>
#include <string.h>

#include "pitw/ref.h"

#define PITW_WINDOWS
#ifdef PITW_WINDOWS
#include <windows.h>
#define ATOMIC_INT volatile LONG
#define ATOMIC_INC(x) (InterlockedIncrement(&x))
#define ATOMIC_DEC(x) (InterlockedDecrement(&x))
#else
#define ATOMIC_INT unsigned int
#define ATOMIC_INC(x) (__atomic_add_fetch(&(x), 1, __ATOMIC_ACQ_REL))
#define ATOMIC_DEC(x) (__atomic_sub_fetch(&(x), 1, __ATOMIC_ACQ_REL))
#endif

struct pitw_ref {
    ATOMIC_INT count;
    void (*dealloc)(void *);
    void *ptr;
};

pitw_ref *pitw_ref_create(void *ptr, void (*dealloc)(void *)) {
    pitw_ref *ref = malloc(sizeof(*ref));
    ref->count = 1;
    /* if no user function is provided, just use free */
    ref->dealloc = dealloc ? dealloc : free;
    ref->ptr = ptr;
    return ref;
}

void *pitw_ref_get(pitw_ref *ref) {
    return ref->ptr;
}

pitw_ref *pitw_ref_up(pitw_ref *ref) {
    ATOMIC_INC(ref->count);
    return ref;
}

void pitw_ref_down(pitw_ref *ref) {
    if (!ATOMIC_DEC(ref->count)) {
        ref->dealloc(ref->ptr);
        /* help catch use after free */
        ref->ptr = NULL;
        free(ref);
    }
}
