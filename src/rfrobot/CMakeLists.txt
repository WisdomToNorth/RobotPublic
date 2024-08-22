

find_package(Eigen3 3.3 REQUIRED CONFIG)
find_package(ompl REQUIRED)
find_package(orocos_kdl REQUIRED COMPONENTS chain)

set(ROBOT_HEADERS
    rfRobotDefine.h
    rfRobotUtils.h
    rfGenerateLs.h
)

set(ROBOT_SOURCES
    rfGenerateLs.cpp
    rfRobotUtils.cpp
)

add_library(RfRobotCore
STATIC
    ${ROBOT_HEADERS}
    ${ROBOT_SOURCES}
    )


target_link_libraries(RfRobotCore
PUBLIC
    Eigen3::Eigen
PRIVATE
    ${OMPL_LIBRARIES}
    ${orocos_kdl_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfDB
    RfGeo3d
    RfBase
    RfLog
    RfIO)

target_include_directories(RfRobotCore
PRIVATE
    ${OMPL_INCLUDE_DIRS}
    ${orocos_kdl_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/build/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    )

set(ROBOT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfrobot)
if (EXISTS ${ROBOT_INCLUDE_DIR})
    file(REMOVE_RECURSE ${ROBOT_INCLUDE_DIR})
endif()

foreach(ROBOT_LIB_HEADES ${ROBOT_HEADERS})
    file(COPY ${ROBOT_LIB_HEADES} DESTINATION ${ROBOT_INCLUDE_DIR})
endforeach()


set(ROBOT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfrobot)
if (EXISTS ${ROBOT_INCLUDE_DIR})
    file(REMOVE_RECURSE ${ROBOT_INCLUDE_DIR})
endif()



foreach(ROBOT_HEADER ${ROBOT_HEADERS})
    file(COPY ${ROBOT_HEADER} DESTINATION ${ROBOT_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
