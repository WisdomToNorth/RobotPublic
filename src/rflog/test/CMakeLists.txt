# TEST

set(LOGTEST_FILES
    main.cpp
    test_frlogger.cpp
    )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_log ${LOGTEST_FILES})

target_link_libraries(TEST_log
PRIVATE
    ${GTEST_LIBRARIES}
    RfBase
    RfLog)

target_include_directories(TEST_log PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/build/include)

target_link_directories(TEST_log
PRIVATE
    ${GTEST_LIBRARY_DIR}
    )

# gtest_discover_tests(TEST_log TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_log
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
