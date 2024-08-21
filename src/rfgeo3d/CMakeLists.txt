
find_package(Eigen3 3.3 REQUIRED CONFIG)

set(VIEWDATA_HEADERS
    geodataio.h
    geoutils.h
    shapeutils.h
    stdafx.h
    vmselectmode.h
    vmutils.h
    vsbuilder.h
)

set(VIEWDATA_SOURCES
    geodataio.cpp
    geoutils.cpp
    shapeutils.cpp
    stdafx.cpp
    vmselectmode.cpp
    vmutils.cpp
    vsbuilder.cpp
)

add_library(RfGeo3d STATIC
    ${VIEWDATA_HEADERS}
    ${VIEWDATA_SOURCES}
)

target_link_libraries(RfGeo3d
PRIVATE
    ${OpenCASCADE_LIBRARIES}
    RfBase
    Eigen3::Eigen
    )

target_include_directories(RfGeo3d PRIVATE
    ${PROJECT_SOURCE_DIR}/build/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${OpenCASCADE_INCLUDE_DIR})

set(VIEWDATA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfgeo3d)
if (EXISTS ${VIEWDATA_INCLUDE_DIR})
    file(REMOVE_RECURSE ${VIEWDATA_INCLUDE_DIR})
endif()



foreach(VIEWDATA_HEADER ${VIEWDATA_HEADERS})
    file(COPY ${VIEWDATA_HEADER} DESTINATION ${VIEWDATA_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
