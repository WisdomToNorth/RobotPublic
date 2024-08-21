# TEST
set(CONFIG_TEST_FILES
    main.cpp
    test_gui.cpp
)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_gui ${CONFIG_TEST_FILES})

target_link_libraries(TEST_gui
PRIVATE
    ${Qt_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfBase
    RfGeo3d
    RfDB
    RfApp
    RfLog
    RfIO
    )

target_include_directories(TEST_gui
PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/build/include
    )

target_link_directories(TEST_gui
PRIVATE
    ${GTEST_LIBRARY_DIR}
    )

# gtest_discover_tests(TEST_gui TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_gui
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)

