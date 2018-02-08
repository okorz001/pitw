#include "common.h"

struct foo {
    int i;
    char c;
};

START_TEST(parent) {
    struct foo foo;
    struct foo *ptr;

    ptr = pitw_util_parent(&foo.i, struct foo, i);
    ck_assert_ptr_eq(ptr, &foo);

    ptr = pitw_util_parent(&foo.c, struct foo, c);
    ck_assert_ptr_eq(ptr, &foo);
}
END_TEST

TCase *create_util_tcase(void) {
    TCase *tc = tcase_create("util");
    tcase_add_test(tc, parent);
    return tc;
}
