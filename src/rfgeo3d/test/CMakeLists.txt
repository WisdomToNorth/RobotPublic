# TEST

set(VMTEST_FILES
    main.cpp
    test_viewdataio.cpp
    test_geoutils.cpp
    test_geodataio.cpp
    test_shapeutils.cpp
    )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_geo3d ${VMTEST_FILES})

target_link_libraries(TEST_geo3d
PRIVATE
    ${GTEST_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfGeo3d)

target_include_directories(TEST_geo3d PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/build/include)

target_link_directories(TEST_geo3d
PRIVATE
    ${GTEST_LIBRARY_DIR}
    ${OpenCASCADE_LIBRARY_DIR}
    )

# gtest_discover_tests(TEST_geo3d TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_geo3d
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
