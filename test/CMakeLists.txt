find_package(Check REQUIRED)

add_executable(pitw_tests
               main.c
               ref.c
               version.c)
target_link_libraries(pitw_tests pitw Check::check)

# "test" target is reserved for ctest
add_custom_target(check ALL COMMAND pitw_tests DEPENDS pitw_tests)
