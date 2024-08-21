# TEST

set(APP_TEST_FILES
    main.cpp
    test_setting.cpp
    test_property.cpp
    test_textid.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_app ${APP_TEST_FILES})

target_link_libraries(TEST_app PRIVATE
    ${GTEST_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfApp)

target_include_directories(TEST_app PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/build/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${OpenCASCADE_INCLUDE_DIR})

target_link_directories(TEST_app PRIVATE
    ${GTEST_LIBRARY_DIR}
    ${OpenCASCADE_LIBRARY_DIR})


# gtest_discover_tests(TEST_app TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_app
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
