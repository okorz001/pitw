#ifndef PITW_VERSION_H
#define PITW_VERSION_H

#include <pitw/config.h>

/**
 * @file
 * Utility functions for checking compatibility between libpitw versions.
 *
 * To ensure binary compatibility with the shared library loaded at runtime,
 * it is recommended that clients add the following code snippet to their
 * initialization routine:
 * @code
 * if (!pitw_version_check()) {
 *   // exit with error
 * }
 * @endcode
 *
 * If libpitw is statically linked, then binary compatibility is guaranteed
 * and this check is unnecessary.
 * @ingroup util
 */

#include <stdint.h>

#include <pitw/config.h>

/**
 * Packs a version number into a 32-bit unsigned integer.
 * @param major The major version number.
 * @param minor The minor version number.
 * @param patch The patch version number.
 */
#define pitw_version_pack(major, minor, patch) ((uint32_t)((major << 24) | (minor << 16) | (patch << 8)))

/**
 * The version number of the libpitw compiled against, suitable for printing.
 */
#define pitw_version_compiled_string() (PITW_VERSION_STRING)

/**
 * The version number of the libpitw compiled against, suitable for checking.
 */
#define pitw_version_compiled() (pitw_version_pack(PITW_VERSION_MAJOR, PITW_VERSION_MINOR, PITW_VERSION_PATCH))

/**
 * The version number of the libpitw runtime, suitable for printing.
 * The returned string must NOT be modified.
 */
PITW_API const char *pitw_version_runtime_string(void);

/**
 * The version number of the libpitw runtime, suitable for checking.
 */
PITW_API uint32_t pitw_version_runtime(void);

/**
 * Checks if a given version is compatible with a target version.
 * @param actual The version to check for compatibility.
 * @param target The target version to compare against.
 * @return 0 if the given version is not compatible with the target version.
 */
PITW_API int pitw_version_compatible(uint32_t actual, uint32_t target);

/**
 * Checks if the libpitw runtime version is compatible with the libpitw that was
 * compiled against.
 * @return 0 if the runtime version is not compatible with the compiled version.
 */
#define pitw_version_check() (pitw_version_compatible(pitw_version_runtime(), pitw_version_compiled()))

#endif /* PITW_VERSION_H */
