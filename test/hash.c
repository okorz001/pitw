#include "common.h"

START_TEST(hash_get) {
    pitw_hash *hash = pitw_hash_create();
    ck_assert_ptr_nonnull(hash);
    ck_assert(!pitw_hash_contains(hash, "foo"));

    int i = 42;
    pitw_hash_set(hash, "foo", &i);
    ck_assert(pitw_hash_contains(hash, "foo"));
    ck_assert_ptr_eq(pitw_hash_get(hash, "foo"), &i);

    pitw_hash_free(hash);
}
END_TEST

START_TEST(hash_remove) {
    pitw_hash *hash = pitw_hash_create();

    int i = 42;
    pitw_hash_set(hash, "foo", &i);

    pitw_hash_remove(hash, "foo");
    ck_assert_ptr_null(pitw_hash_get(hash, "foo"));
    ck_assert(!pitw_hash_contains(hash, "foo"));

    pitw_hash_free(hash);
}
END_TEST

START_TEST(hash_contains) {
    pitw_hash *hash = pitw_hash_create();
    ck_assert_ptr_nonnull(hash);
    ck_assert(!pitw_hash_contains(hash, "foo"));

    pitw_hash_set(hash, "foo", NULL);
    ck_assert(pitw_hash_contains(hash, "foo"));

    pitw_hash_remove(hash, "foo");
    ck_assert(!pitw_hash_contains(hash, "foo"));

    pitw_hash_free(hash);
}
END_TEST

static size_t fake_hash(char *key) {
    return 0; /* boom */
}

START_TEST(collision) {
    pitw_hash *hash = pitw_hash_create_custom(0, fake_hash);

    int i = 46;
    pitw_hash_set(hash, "foo", &i);
    int j = 2;
    pitw_hash_set(hash, "bar", &j);
    ck_assert_ptr_eq(pitw_hash_get(hash, "foo"), &i);
    ck_assert_ptr_eq(pitw_hash_get(hash, "bar"), &j);

    pitw_hash_remove(hash, "foo");
    ck_assert(pitw_hash_contains(hash, "bar"));
    ck_assert_ptr_eq(pitw_hash_get(hash, "bar"), &j);

    pitw_hash_free(hash);
}
END_TEST

START_TEST(resize) {
    pitw_hash *hash = pitw_hash_create_custom(2, NULL);

    char *keys[] = {"a", "b", "c"};
    int vals[] = {1, 2, 3};

    for (int i = 0; i < 3; ++i) {
        pitw_hash_set(hash, keys[i], &vals[i]);
    }

    for (int i = 0; i < 3; ++i) {
        ck_assert_ptr_eq(pitw_hash_get(hash, keys[i]), &vals[i]);
    }

    pitw_hash_free(hash);
}
END_TEST

TCase *create_hash_tcase(void) {
    TCase *tc = tcase_create("hash");
    tcase_add_test(tc, hash_get);
    tcase_add_test(tc, hash_remove);
    tcase_add_test(tc, hash_contains);
    tcase_add_test(tc, collision);
    tcase_add_test(tc, resize);
    return tc;
}
