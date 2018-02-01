# Prefer pkg-config if that's installed.
if(NOT Check_NO_PKGCONFIG)
  message(STATUS "Finding Check with PkgConfig")
  find_package(PkgConfig)
  if(PkgConfig_FOUND)
    pkg_check_modules(PC_CHECK check)
    if(PC_CHECK_FOUND)
      set(Check_DEFINITIONS ${PC_CHECK_CFLAGS_OTHER})
      set(Check_INCLUDE_DIRS ${PC_CHECK_INCLUDE_DIRS})
      set(Check_LIBRARIES ${PC_CHECK_LDFLAGS})
    endif()
  endif()
endif()

# Do it the hard way.
if(Check_NO_PKGCONFIG OR NOT PC_CHECK_FOUND)
  message(STATUS "Finding Check without PkgConfig")

  # TODO: Is there anyway to determine required CFLAGS without pkgconfig?
  set(Check_DEFINITIONS)

  # Find Check headers.
  find_path(Check_INCLUDE_DIRS
            NAMES check.h
            HINTS ${Check_ROOT}
            PATH_SUFFIXES include)
  mark_as_advanced(FORCE Check_INCLUDE_DIRS)

  # Find Check library.
  find_library(Check_CHECK_LIBRARY
               NAMES check
               HINTS ${Check_ROOT}
               PATH_SUFFIXES lib)
  mark_as_advanced(FORCE Check_CHECK_LIBRARY)

  # Find optional POSIX compatibility library.
  find_library(Check_COMPAT_LIBRARY
               NAMES compat
               HINTS ${Check_ROOT}
               PATH_SUFFIXES lib)
  mark_as_advanced(FORCE Check_COMPAT_LIBRARY)

  set(Check_LIBRARIES ${Check_CHECK_LIBRARY})
  if(Check_COMPAT_LIBRARY)
    list(APPEND Check_LIBRARIES ${Check_COMPAT_LIBRARY})
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Check DEFAULT_MSG
                                  Check_LIBRARIES
                                  Check_INCLUDE_DIRS)

if(Check_FOUND)
  # Create imported target.
  add_library(Check::check INTERFACE IMPORTED)
  set_target_properties(Check::check PROPERTIES
                        INTERFACE_COMPILE_OPTIONS "${Check_DEFINITIONS}"
                        INTERFACE_INCLUDE_DIRECTORIES "${Check_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${Check_LIBRARIES}")
endif()
