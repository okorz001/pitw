set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)

# No abstraction for this yet
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Command line warning D9025 : overriding '/W3' with '/W4'
    string(REGEX REPLACE " ?/W[0-9]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    # warning C4100: '_i': unreferenced formal parameter
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX /wd4100")
endif()
message(STATUS "Using CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
