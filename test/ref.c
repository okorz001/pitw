#include "common.h"

/* This value doesn't matter because we won't actually free it */
static char *str = "zzz";
static int deallocated = 0;
static void cb(void *arg) {
    ck_assert_ptr_eq(arg, str);
    deallocated = 1;
}

START_TEST(ref_dealloc) {
    pitw_ref *ref = pitw_ref_create(str, cb);
    ck_assert(!deallocated);

    pitw_ref *ref2 = pitw_ref_up(ref);
    ck_assert(!deallocated);

    pitw_ref_down(ref);
    ck_assert(!deallocated);

    pitw_ref_down(ref2);
    ck_assert(deallocated);
}
END_TEST

TCase *create_ref_tcase(void) {
    TCase *tc = tcase_create("ref");
    tcase_add_test(tc, ref_dealloc);
    return tc;
}
