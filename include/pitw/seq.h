#ifndef PITW_SEQ_H
#define PITW_SEQ_H

#include <pitw/config.h>
#include <stddef.h>

typedef struct pitw_seq pitw_seq;

PITW_API pitw_seq *pitw_seq_up(pitw_seq *seq);
PITW_API void pitw_seq_down(pitw_seq *seq);
PITW_API void *pitw_seq_first(pitw_seq *seq);
PITW_API pitw_seq *pitw_seq_rest(pitw_seq *seq);
PITW_API pitw_seq *pitw_seq_cons(void *val, pitw_seq *seq);

PITW_API pitw_seq *pitw_seq_from_memory(void *ptr, size_t count, size_t size);
#define pitw_seq_from_array(arr) (pitw_seq_from_memory(arr, sizeof(arr) / sizeof(arr[0]), sizeof(arr[0])))

#endif /* PITW_SEQ_H */
