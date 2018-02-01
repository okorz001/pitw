message(STATUS "Using CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# For lack of better name, "install_targets" is used by CMake
function(install_targets2)
    install(TARGETS ${ARGN}
            EXPORT PITW
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endfunction()

function(install_headers)
    foreach(d ${ARGN})
        install(DIRECTORY "${d}"
                DESTINATION include
                FILES_MATCHING PATTERN "*.h")
    endforeach()
endfunction()

configure_file("${CMAKE_CURRENT_LIST_DIR}/PITWConfig.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/PITWConfig.cmake"
               @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/PITWConfig.cmake"
        DESTINATION share/PITW/cmake)

install(EXPORT PITW
        NAMESPACE PITW::
        FILE PITWTargets.cmake
        DESTINATION share/PITW/cmake)
