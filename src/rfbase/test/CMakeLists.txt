# TEST_base
message(STATUS "CMake c++ version: ${CMAKE_CXX_STANDARD}")

set(CMAKE_CXX_STANDARD 20)
set(TEST_BASE_FILES
    main.cpp
    test_bigint.cpp
    test_conutils.cpp
    test_datatools.cpp
    test_mathutils.cpp
    test_multiunionset.cpp
    test_timer.cpp
    test_tree.cpp
    test_undoableobj.cpp
    test_span.cpp
    test_json.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_base ${TEST_BASE_FILES})

target_link_libraries(TEST_base
PRIVATE
    ${GTEST_LIBRARIES}
    RfBase)

target_include_directories(TEST_base
PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/build/include
)

target_link_directories(TEST_base
PRIVATE
    ${GTEST_LIBRARY_DIR}
    ${GTEST_BOTH_LIBRARIES}
)


# gtest_discover_tests(TEST_base TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_base
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
