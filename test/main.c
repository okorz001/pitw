#include "common.h"

/* check for memory leaks with simple alloc count */
static int allocs;

static void *alloc_count(size_t size) {
    ++allocs;
    return malloc(size);
}

static void free_count(void *ptr) {
    --allocs;
    free(ptr);
}

static void setup(void) {
    allocs = 0;
}

static void teardown(void) {
    ck_assert_int_eq(allocs, 0);
}

static void add_tcase(Suite *suite, TCase *tcase) {
    tcase_add_checked_fixture(tcase, setup, teardown);
    suite_add_tcase(suite, tcase);
}

int main(void) {
    /* install allocator */
    pitw_mem_ctx ctx = {alloc_count, free_count};
    pitw_mem_set_ctx(&ctx);

    Suite *suite = suite_create("pitw");
    /* add test cases below */
    add_tcase(suite, create_ref_tcase());
    add_tcase(suite, create_seq_tcase());
    add_tcase(suite, create_util_tcase());
    add_tcase(suite, create_version_tcase());

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
