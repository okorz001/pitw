find_package(Check REQUIRED)

add_executable(pitw_tests
               hash.c
               main.c
               obj.c
               ref.c
               seq.c
               util.c
               version.c)
target_link_libraries(pitw_tests pitw Check::check)

# "test" target is reserved for ctest
add_custom_target(check ALL COMMAND pitw_tests DEPENDS pitw_tests)
