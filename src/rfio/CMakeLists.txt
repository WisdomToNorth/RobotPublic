# IO ASSIMP
find_package(assimp REQUIRED)

set(IO_FILES
    brep_utils.cpp
    writer_brep.cpp
    assimpreader.cpp
    graphics_mesh_data_source.cpp
    reader_step.cpp
    io_format.cpp
    reader_ply.cpp
    mesh_utils.cpp
    reader_factory.cpp
    stdafx.cpp
    occ_progress_indicator.cpp
    reader_brep.cpp
    tkernel_utils.cpp
    occ_progress_indicator.h
    reader_brep.h
    mesh_utils.h
    reader_factory.h
    io_format.h
    reader_ply.h
    graphics_mesh_data_source.h
    reader_step.h
    writer_base.h
    stdafx.h
    brep_utils.h
    assimpreader.h
    reader_base.h
    tkernel_utils.h
    writer_brep.h
    reader_trajectory.h
    reader_trajectory.cpp
    writer_trajectory.h
    writer_trajectory.cpp
    reader_trace.h
    reader_trace.cpp
    writer_trace.h
    writer_trace.cpp
    writer_robot.h
    writer_robot.cpp
    reader_robot.h
    reader_robot.cpp
    thirdparty/miniply.cpp
    thirdparty/miniply.h
    )

# generate an object library from sources
add_library(RfIO STATIC ${IO_FILES})

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

set(IO_HEADERS
    reader_brep.h
    mesh_utils.h
    reader_factory.h
    io_format.h
    reader_ply.h
    reader_step.h
    writer_base.h
    brep_utils.h
    assimpreader.h
    reader_base.h
    tkernel_utils.h
    writer_brep.h
    writer_robot.h
    reader_robot.h
)


foreach(IO_HEADER ${IO_HEADERS})
    file(COPY ${IO_HEADER} DESTINATION ${IO_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
