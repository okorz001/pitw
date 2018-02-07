#include <stdlib.h>

#include "pitw/mem.h"
#include "seq.h"

pitw_seq *pitw_seq_up(pitw_seq *seq) {
    if (seq) pitw_atomic_inc(seq->count);
    return seq;
}

void pitw_seq_down(pitw_seq *seq) {
    if (seq && !pitw_atomic_dec(seq->count)) {
        seq->dealloc(seq);
    }
}

void *pitw_seq_first(pitw_seq *seq) {
    return seq->first;
}

pitw_seq *pitw_seq_rest(pitw_seq *seq) {
    return seq->rest(seq);
}

typedef struct {
    pitw_seq seq;
    pitw_seq *rest;
} pitw_cons;

static void pitw_cons_dealloc(pitw_seq *seq) {
    /* TODO: container_of */
    pitw_cons *cons = (pitw_cons *)seq;
    pitw_seq_down(cons->rest);
    pitw_mem_free(cons);
}

static pitw_seq *pitw_cons_rest(pitw_seq *seq) {
    /* TODO: container_of */
    pitw_cons *cons = (pitw_cons *)seq;
    return cons->rest;
}

pitw_seq *pitw_seq_cons(void *first, pitw_seq *rest) {
    pitw_cons *cons = pitw_mem_alloc(sizeof(*cons));
    cons->seq.count = 0;
    cons->seq.first = first;
    cons->seq.dealloc = pitw_cons_dealloc;
    cons->seq.rest = pitw_cons_rest;
    cons->rest = pitw_seq_up(rest);
    return &cons->seq;
}

pitw_seq *pitw_seq_from_memory(void *ptr, size_t count, size_t size) {
    /* C standard does not allow pointer arithmetic on void *. */
    char *p = ptr;
    pitw_seq *rest = NULL;
    /* Build in reverse */
    for (int i = count - 1; i >= 0; --i) {
        rest = pitw_seq_cons(p + i * size, rest);
    }
    return rest;
}
