#include "common.h"

START_TEST(obj_set_i) {
    pitw_obj *o = pitw_obj_create(NULL);
    pitw_obj_up(o);

    intmax_t i = 42;
    ck_assert(!pitw_obj_get_i(o, "foo", &i));
    ck_assert_int_eq(i, 42);

    pitw_obj_set_i(o, "foo", 1);
    ck_assert(pitw_obj_get_i(o, "foo", &i));
    ck_assert_int_eq(i, 1);

    /* TODO: users probably shouldn't have to do this */
    pitw_obj_remove(o, "foo");

    pitw_obj_down(o);
}
END_TEST

TCase *create_obj_tcase(void) {
    TCase *tc = tcase_create("obj");
    tcase_add_test(tc, obj_set_i);
    return tc;
}
