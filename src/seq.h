#ifndef PITW_PRIVATE_SEQ_H
#define PITW_PRIVATE_SEQ_H

#include "pitw/seq.h"
#include "atomic.h"

struct pitw_seq {
    pitw_atomic count;
    void *first;
    void (*dealloc)(pitw_seq *);
    pitw_seq *(*rest)(pitw_seq *);
};

#endif /* PITW_PRIVATE_SEQ_H */
