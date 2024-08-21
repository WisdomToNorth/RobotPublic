# TEST

set(RFDB_FILES
    main.cpp
    test_dbutil.cpp
    test_dbcheck.cpp
    test_db.cpp
    test_dbpart.cpp
    test_dbproperty.cpp
    test_shapebuilder.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

add_executable(TEST_db ${RFDB_FILES})

target_link_libraries(TEST_db
PRIVATE
    ${GTEST_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfIO
    RfBase
    RfDB)

target_include_directories(TEST_db
PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/build/include
    )

target_link_directories(TEST_db
PRIVATE
    ${GTEST_LIBRARY_DIR}
    ${OpenCASCADE_LIBRARY_DIR}
    )


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(${PROJECT_SOURCE_DIR}/cmake/AddCoverage.cmake)
    AddCoverage(TEST_db)
endif()

# gtest_discover_tests(TEST_db TEST_DIR ${CMAKE_BINARY_DIR}/executable)
gtest_add_tests(TARGET TEST_db
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/executable)
