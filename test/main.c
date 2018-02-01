#include "common.h"

int main(void) {
    Suite *suite = suite_create("pitw");
    /* add test cases below */
    suite_add_tcase(suite, create_ref_tcase());
    suite_add_tcase(suite, create_version_tcase());

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
