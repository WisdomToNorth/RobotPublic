# TEST

set(IOTEST_FILES
    main.cpp
    test_brep_io.cpp
    test_ply_io.cpp
    test_step_io.cpp
    test_robot_io.cpp
    file_converter.cpp
)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_io ${IOTEST_FILES})

target_link_libraries(TEST_io
PRIVATE
    ${GTEST_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfIO
    RfGeo3d
    RfBase
    RfDB
    )

target_include_directories(TEST_io
PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/build/include
    ${OpenCASCADE_INCLUDE_DIR})

target_link_directories(TEST_io
PRIVATE
    ${GTEST_LIBRARY_DIR}
    ${ASSIMP_LIBRARY_DIR}
    ${OpenCASCADE_LIBRARY_DIR}
    )


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    AddCoverage(TEST_io)
endif()

# gtest_discover_tests(TEST_io TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_io
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)

