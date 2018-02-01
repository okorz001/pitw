#include "pitw/version.h"

#if PITW_VERSION_MAJOR > 0xFF
#error "pitw major version is too large"
#endif

#if PITW_VERSION_MINOR > 0xFF
#error "pitw minor version is too large"
#endif

#if PITW_VERSION_PATCH > 0xFF
#error "pitw patch version is too large"
#endif

const char *pitw_version_runtime_string(void) {
    /* the value gets baked in by the compiler */
    return pitw_version_compiled_string();
}

uint32_t pitw_version_runtime(void) {
    /* the value gets baked in by the compiler */
    return pitw_version_compiled();
}

static uint8_t major(uint32_t version) {
    return (uint8_t)((version & 0xFF000000) >> 24);
}

static uint8_t minor(uint32_t version) {
    return (uint8_t)((version & 0x00FF0000) >> 16);
}

static uint8_t patch(uint32_t version) {
    return (uint8_t)((version & 0x0000FF00) >> 8);
}

int pitw_version_compatible(uint32_t actual, uint32_t target) {
    /* strict major match */
    if (major(actual) != major(target)) {
        return 0;
    }
    /* minor greater or equal */
    if (minor(actual) < minor(target)) {
        return 0;
    }
    if (minor(actual) > minor(target)) {
        /* if minor is greater, ignore patch */
        return 1;
    }
    /* patch greater or equal */
    if (patch(actual) < patch(target)) {
        return 0;
    }
    return 1;
}
