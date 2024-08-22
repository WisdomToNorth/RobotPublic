


set(RFDB_HEADERS
    dbAssembly.h
    dbCommon.h
    dbLabelTree.h
    dbMesh.h
    dbPart.h
    dbPointCloud.h
    dbProperty.h
    dbRobot.h
    dbRobotDefine.h
    dbTrace.h
    dbTrajectory.h
    dbVariant.h
)

set(RFDB_SOURCES
    dbAssembly.cpp
    dbLabelTree.cpp
    dbMesh.cpp
    dbPart.cpp
    dbPointCloud.cpp
    dbProperty.cpp
    dbRobot.cpp
    dbTrace.cpp
    dbTrajectory.cpp
    dbVariant.cpp
)

set(RFDB_UTIL_HEADERS
    dbutils/dbpartutils.h
    dbutils/dbcheck.h
    dbutils/dbVariantUtils.h
    dbutils/shapebuilder.h
)

set(RFDB_UTIL_SOURCES
    dbutils/dbcheck.cpp
    dbutils/dbpartutils.cpp
    dbutils/dbVariantUtils.cpp
    dbutils/shapebuilder.cpp
)

add_library(RfDB STATIC
${RFDB_HEADERS}
${RFDB_SOURCES}
${RFDB_UTIL_HEADERS}
${RFDB_UTIL_SOURCES}
)

target_link_libraries(RfDB
PUBLIC
    RfGeo3d # we may later remove this dependency
PRIVATE
    ${OpenCASCADE_LIBRARIES}
    RfBase
    )

target_include_directories(RfDB
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/build/include
    ${OpenCASCADE_INCLUDE_DIR}
    )


set(RFDB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfdb)
set(RFDB_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfdb/dbutils)

if (EXISTS ${RFDB_INCLUDE_DIR})
    file(REMOVE_RECURSE ${RFDB_INCLUDE_DIR})
endif()

foreach(RFDB_HEADER ${RFDB_HEADERS})
    file(COPY ${RFDB_HEADER} DESTINATION ${RFDB_INCLUDE_DIR})
endforeach()

foreach(RFDB_UTILS_HEADER ${RFDB_UTIL_HEADERS})
    file(COPY ${RFDB_UTILS_HEADER} DESTINATION ${RFDB_UTILS_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
