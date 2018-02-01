#include "common.h"

START_TEST(version_check) {
    ck_assert(pitw_version_check());
}
END_TEST

/* mess with the "compiled" versions */
#define TEMP_STRING PITW_VERSION_STRING
#undef PITW_VERSION_STRING
#define PITW_VERSION_STRING "255.254.253"

#define TEMP_MAJOR PITW_VERSION_STRING
#undef PITW_VERSION_MAJOR
#define PITW_VERSION_MAJOR 255

#define TEMP_MINOR PITW_VERSION_STRING
#undef PITW_VERSION_MINOR
#define PITW_VERSION_MINOR 254

#define TEMP_PATCH PITW_VERSION_STRING
#undef PITW_VERSION_PATCH
#define PITW_VERSION_PATCH 253

START_TEST(version_compiled_string) {
    ck_assert_str_eq(pitw_version_compiled_string(), "255.254.253");
}
END_TEST

START_TEST(version_runtime_string) {
    ck_assert_str_eq(pitw_version_runtime_string(), "1.0.0");
}
END_TEST

START_TEST(version_check_fail) {
    ck_assert(!pitw_version_check());
}
END_TEST

/* reset the compiled version */
#undef PITW_VERSION_PATCH
#define PITW_VERSION_PATCH TEMP_PATCH
#undef TEMP_PATCH

#undef PITW_VERSION_MINOR
#define PITW_VERSION_MINOR TEMP_MINOR
#undef TEMP_MINOR

#undef PITW_VERSION_MAJOR
#define PITW_VERSION_MAJOR TEMP_MAJOR
#undef TEMP_MAJOR

#undef PITW_VERSION_STRING
#define PITW_VERSION_STRING TEMP_STRING
#undef TEMP_STRING

START_TEST(version_compatible) {
    uint32_t v111 = pitw_version_pack(1, 1, 1);
    uint32_t v000 = pitw_version_pack(0, 0, 0);
    uint32_t v200 = pitw_version_pack(2, 0, 0);
    uint32_t v100 = pitw_version_pack(1, 0, 0);
    uint32_t v120 = pitw_version_pack(1, 2, 0);
    uint32_t v110 = pitw_version_pack(1, 1, 0);
    uint32_t v112 = pitw_version_pack(1, 1, 2);

    /* match pass */
    ck_assert(pitw_version_compatible(v111, v111));
    /* major - fail */
    ck_assert(!pitw_version_compatible(v000, v111));
    /* major + fail */
    ck_assert(!pitw_version_compatible(v200, v111));
    /* minor - fail */
    ck_assert(!pitw_version_compatible(v100, v111));
    /* minor + pass */
    ck_assert(pitw_version_compatible(v120, v111));
    /* patch - fail */
    ck_assert(!pitw_version_compatible(v110, v111));
    /* patch + pass */
    ck_assert(pitw_version_compatible(v112, v111));
}
END_TEST

TCase *create_version_tcase(void) {
    TCase *tc = tcase_create("version");
    tcase_add_test(tc, version_check);
    tcase_add_test(tc, version_compiled_string);
    tcase_add_test(tc, version_runtime_string);
    tcase_add_test(tc, version_check_fail);
    tcase_add_test(tc, version_compatible);
    return tc;
}
