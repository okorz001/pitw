#ifndef PITW_TEST_COMMON_H
#define PITW_TEST_COMMON_H

#include <check.h>
#include <pitw/pitw.h>
#include <stdlib.h>

TCase *create_ref_tcase(void);
TCase *create_seq_tcase(void);
TCase *create_version_tcase(void);

#endif  /* PITW_TEST_COMMON_H */
