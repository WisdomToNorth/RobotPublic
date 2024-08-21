# TEST


set(TEST_FILES
    main.cpp
    test_robottools.cpp
    test_paramloader.cpp
)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/executable")

add_executable(TEST_robot ${TEST_FILES})

target_include_directories(TEST_robot
PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src/RobotTools/arm_math
    ${PROJECT_SOURCE_DIR}/build/include)

target_link_libraries(TEST_robot
PRIVATE
    ${GTEST_LIBRARIES}
    ${OMPL_LIBRARIES}
    ${orocos_kdl_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfLog
    RfIO
    RfGeo3d
    RfRobotCore
    RfBase)

target_link_directories(TEST_robot
PRIVATE
    ${OpenCASCADE_LIBRARY_DIR}
    ${OMPL_LIBRARY_DIR}
    ${GTEST_LIBRARY_DIR}
    ${orocos_kdl_LIBRARY_DIR}
    ${ASSIMP_LIBRARY_DIR}
)

# gtest_discover_tests(TEST_robot TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_robot
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)