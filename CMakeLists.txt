cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(PITW VERSION 1.0.0 LANGUAGES C)

option(PITW_SHARED "Build pitw as a shared library?" ON)
option(PITW_ENABLE_DOCS "Build pitw documentation?" ON)
option(PITW_ENABLE_TESTS "Build pitw tests?" ON)

include(cmake/build_types.cmake)
include(cmake/compiler.cmake)
include(cmake/features.cmake)
include(cmake/install.cmake)
include(cmake/output.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(PITW_SHARED)
  set(PITW_TYPE SHARED)
else()
  set(PITW_TYPE STATIC)
endif()
message(STATUS "Building library type: ${PITW_TYPE}")

configure_file("${CMAKE_SOURCE_DIR}/cmake/config.h.in"
               "${CMAKE_BINARY_DIR}/include/pitw/config.h"
               @ONLY)

add_library(pitw ${PITW_TYPE}
            src/hash.c
            src/mem.c
            src/obj.c
            src/ref.c
            src/seq.c
            src/version.c)
set_property(TARGET pitw PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET pitw PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
target_include_directories(pitw PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_compile_definitions(pitw PRIVATE PITW_EXPORTS)

# Atomic support is an implementation detail.
if(PITW_HAS_C11_ATOMIC)
  message(STATUS "Using atomics: C11 stdatomic.h")
  target_compile_definitions(pitw PRIVATE PITW_HAS_C11_ATOMIC)
elseif(PITW_HAS_GCC_ATOMIC)
  message(STATUS "Using atomics: GCC __atomic builtins")
  target_compile_definitions(pitw PRIVATE PITW_HAS_GCC_ATOMIC)
elseif(PITW_HAS_GCC_SYNC)
  message(STATUS "Using atomics: GCC __sync builtins")
  target_compile_definitions(pitw PRIVATE PITW_HAS_GCC_SYNC)
elseif(PITW_HAS_WIN32_INTERLOCKED)
  message(STATUS "Using atomics: Windows Interlocked API")
  target_compile_definitions(pitw PRIVATE PITW_HAS_WIN32_INTERLOCKED)
else()
  message(SEND_ERROR "No atomic support")
endif()

install_targets2(pitw)
install_headers("${CMAKE_SOURCE_DIR}/include/"
                "${CMAKE_BINARY_DIR}/include/")

if(PITW_ENABLE_DOCS)
  include(cmake/docs.cmake)
endif()
if(PITW_ENABLE_TESTS)
  add_subdirectory(test)
endif()
