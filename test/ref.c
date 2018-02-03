#include "common.h"

/* This value doesn't matter because we won't actually free it */
static void *p;
static int deallocated;
static void cb(void *arg) {
    ck_assert_ptr_eq(arg, p);
    deallocated = 1;
}

START_TEST(ref_get) {
    p = (void *)0x1;

    pitw_ref *ref = pitw_ref_create(p, cb);
    ck_assert_ptr_eq(pitw_ref_get(ref), p);
}
END_TEST

START_TEST(ref_dealloc) {
    p = (void *)0x2;
    deallocated = 0;

    pitw_ref *ref = pitw_ref_create(p, cb);
    ck_assert(!deallocated);

    pitw_ref *ref2 = pitw_ref_up(ref);
    ck_assert(!deallocated);

    pitw_ref_down(ref);
    ck_assert(!deallocated);

    pitw_ref_down(ref2);
    ck_assert(deallocated);
}
END_TEST

START_TEST(ref_clear) {
    p = (void *)0x3;
    deallocated = 0;

    pitw_ref *ref = pitw_ref_create(p, cb);
    ck_assert(!deallocated);

    pitw_ref_clear(ref);
    ck_assert(deallocated);
    ck_assert_ptr_null(ref);
}
END_TEST

START_TEST(ref_set) {
    p = (void *)0x4;
    deallocated = 0;

    pitw_ref *ref = pitw_ref_create(p, cb);
    ck_assert(!deallocated);

    /* some arbitrary thing */
    pitw_ref *ref2 = pitw_ref_create(malloc(1), NULL);
    pitw_ref_set(ref, ref2);
    ck_assert_ptr_eq(ref, ref2);
    ck_assert(deallocated);

    pitw_ref_clear(ref);
    pitw_ref_clear(ref2);
}
END_TEST

TCase *create_ref_tcase(void) {
    TCase *tc = tcase_create("ref");
    tcase_add_test(tc, ref_get);
    tcase_add_test(tc, ref_dealloc);
    tcase_add_test(tc, ref_clear);
    tcase_add_test(tc, ref_set);
    return tc;
}
