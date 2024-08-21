

set(RFDB_LIB_FILES
    dbAssembly.cpp
    dbAssembly.h
    dbCommon.h
    dbLabelTree.cpp
    dbLabelTree.h
    dbMesh.cpp
    dbMesh.h
    dbPart.cpp
    dbPart.h
    dbPointCloud.cpp
    dbPointCloud.h
    dbProperty.cpp
    dbRobot.cpp
    dbRobot.h
    dbRobotDefine.h
    dbTrace.cpp
    dbTrace.h
    dbTrajectory.cpp
    dbTrajectory.h
    dbVariant.cpp
    dbVariant.h
    dbProperty.h

    dbutils/dbcheck.cpp
    dbutils/dbcheck.h
    dbutils/dbpartutils.cpp
    dbutils/dbpartutils.h
    dbutils/dbVariantUtils.cpp
    dbutils/dbVariantUtils.h
    dbutils/shapebuilder.cpp
    dbutils/shapebuilder.h
)

add_library(RfDB STATIC ${RFDB_LIB_FILES})

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

set(RFDB_HEADERS
    dbProperty.h
    dbMesh.h
    dbTrace.h
    dbCommon.h
    dbAssembly.h
    dbPart.h
    dbVariant.h
    dbPointCloud.h
    dbRobotDefine.h
    dbRobot.h
    dbLabelTree.h
    dbTrajectory.h
)

foreach(RFDB_HEADER ${RFDB_HEADERS})
    file(COPY ${RFDB_HEADER} DESTINATION ${RFDB_INCLUDE_DIR})
endforeach()

set(RFDB_UTILS_HEADERS
    dbutils/dbpartutils.h
    dbutils/dbcheck.h
    dbutils/shapebuilder.h
    dbutils/dbVariantUtils.h
)

foreach(RFDB_UTILS_HEADER ${RFDB_UTILS_HEADERS})
    file(COPY ${RFDB_UTILS_HEADER} DESTINATION ${RFDB_UTILS_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
