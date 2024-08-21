# TEST

set(LOGTEST_FILES
    main.cpp
    test_grphic.cpp
    )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_graphic ${LOGTEST_FILES})

target_link_libraries(TEST_graphic
PRIVATE
    ${GTEST_LIBRARIES}
    ${Qt_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfGraphic
)

target_include_directories(TEST_graphic PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/build/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    )

target_link_directories(TEST_graphic
PRIVATE
    ${Qt_LIBRARY_DIR}
    ${GTEST_LIBRARY_DIR}
    ${OpenCASCADE_LIBRARY_DIR}
    )

# gtest_discover_tests(TEST_graphic TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_graphic
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
