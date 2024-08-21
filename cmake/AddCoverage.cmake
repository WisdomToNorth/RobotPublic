function(AddCoverage target)
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        return()
    endif()

    if(NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        message(WARNING "Only GCC and Clang are supported for coverage, skipping coverage target")
        return()
    endif()

    find_program(LCOV_PATH lcov QUIET)
    find_program(GENHTML_PATH genhtml QUIET)

    if(LCOV_PATH AND GENHTML_PATH)
        target_compile_options(${target} PRIVATE --coverage)
        target_link_options(${target} PUBLIC --coverage)
        add_custom_command(TARGET ${target} PRE_BUILD COMMAND
                           find ${CMAKE_BINARY_DIR} -type f
                           -name '*.gcda' -exec rm {} +)
        add_custom_target(coverage
            COMMENT "Running coverage for ${target}..."
            COMMAND ${LCOV_PATH} -d . --zerocounters
            COMMAND $<TARGET_FILE:${target}>
            COMMAND ${LCOV_PATH} -d . --capture -o coverage.info
            COMMAND ${LCOV_PATH} -r coverage.info '/usr/include/*'
            COMMAND ${GENHTML_PATH} -o coverage filtered.info --legend
            COMMAND rm -rf coverage.info filtered.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    else()
        message(WARNING "lcov or genhtml not found, skipping coverage target")
    endif()
endfunction()
