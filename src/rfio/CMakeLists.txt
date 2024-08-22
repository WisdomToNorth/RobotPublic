# IO ASSIMP
find_package(assimp REQUIRED)

set(IO_HEADERS
    assimpreader.h
    brep_utils.h
    graphics_mesh_data_source.h
    io_format.h
    mesh_utils.h
    occ_progress_indicator.h
    reader_base.h
    reader_brep.h
    reader_factory.h
    reader_ply.h
    reader_robot.h
    reader_step.h
    reader_trace.h
    reader_trajectory.h
    stdafx.h
    tkernel_utils.h
    writer_base.h
    writer_brep.h
    writer_robot.h
    writer_trace.h
    writer_trajectory.h
)

set(IO_SOURCES
    assimpreader.cpp
    brep_utils.cpp
    graphics_mesh_data_source.cpp
    io_format.cpp
    mesh_utils.cpp
    occ_progress_indicator.cpp
    reader_brep.cpp
    reader_factory.cpp
    reader_ply.cpp
    reader_robot.cpp
    reader_step.cpp
    reader_trace.cpp
    reader_trajectory.cpp
    stdafx.cpp
    tkernel_utils.cpp
    writer_brep.cpp
    writer_robot.cpp
    writer_trace.cpp
    writer_trajectory.cpp
)

set(MINIPLY_FILES
    ${CMAKE_SOURCE_DIR}/thirdparty/miniply/miniply.h
    ${CMAKE_SOURCE_DIR}/thirdparty/miniply/miniply.cpp
)

# generate an object library from sources
add_library(RfIO STATIC
    ${IO_HEADERS}
    ${IO_SOURCES}
    ${MINIPLY_FILES}
)

target_link_libraries(RfIO PRIVATE
    ${OpenCASCADE_LIBRARIES}
    assimp::assimp
    RfBase
    RfGeo3d
    RfDB
    RfApp
)

target_include_directories(RfIO
PRIVATE
    ${ASSIMP_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/build/include
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${OpenCASCADE_INCLUDE_DIR})


set(IO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfio)
if (EXISTS ${IO_INCLUDE_DIR})
    file(REMOVE_RECURSE ${IO_INCLUDE_DIR})
endif()



foreach(IO_HEADER ${IO_HEADERS})
    file(COPY ${IO_HEADER} DESTINATION ${IO_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
