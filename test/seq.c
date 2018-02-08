#include "common.h"

START_TEST(seq_from_array) {
    int arr[3] = {1, 2, 3};
    int *ptr;

    pitw_seq *seq = pitw_seq_from_array(arr);
    ck_assert_ptr_nonnull(seq);

    /* TODO: this sucks */
    pitw_seq *head = seq;

    ptr = pitw_seq_first(seq);
    ck_assert_int_eq(*ptr, 1);

    seq = pitw_seq_rest(seq);
    ck_assert_ptr_nonnull(seq);

    ptr = pitw_seq_first(seq);
    ck_assert_int_eq(*ptr, 2);

    seq = pitw_seq_rest(seq);
    ck_assert_ptr_nonnull(seq);

    ptr = pitw_seq_first(seq);
    ck_assert_int_eq(*ptr, 3);

    seq = pitw_seq_rest(seq);
    ck_assert_ptr_null(seq);

    pitw_seq_down(head);
}
END_TEST

TCase *create_seq_tcase(void) {
    TCase *tc = tcase_create("seq");
    tcase_add_test(tc, seq_from_array);
    return tc;
}
