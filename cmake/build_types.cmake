# CMake defines a few others but we don't need them.
set(allowed_types Debug Release)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES ${allowed_types})
    message(STATUS "Multiple configuration build: ${CMAKE_CONFIGURATION_TYPES}")
    foreach(type ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${type} type)
        message(STATUS "Using CMAKE_C_FLAGS_${type}: ${CMAKE_C_FLAGS_${type}}")
    endforeach()
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()

    if(NOT CMAKE_BUILD_TYPE IN_LIST allowed_types)
        message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}\n"
                "Supported build types: ${allowed_types}\n")
    endif()

    message(STATUS "Single configuration build: ${CMAKE_BUILD_TYPE}")
    string(TOUPPER ${CMAKE_BUILD_TYPE} type)
    message(STATUS "Using CMAKE_C_FLAGS_${type}: ${CMAKE_C_FLAGS_${type}}")
endif()

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "")
