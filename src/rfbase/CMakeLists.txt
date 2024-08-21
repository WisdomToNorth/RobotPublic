# Base

set(BASE_HEADERS
    concurrent_unionset.h
    concurrent_utils.h
    datautils.h
    meta_enum.h
    rfArrayStack.h
    rfBigInt.h
    rfSigUtils.h
    rfTimeLimit.h
    rfTimer.h
    rfUndoableObjects.h
    rfanimecontroler.h
    rfdefine.h
    rfidtree.h
    rfmath.h
    rfsingleton.h
    rfspan.h
    unionset_basic.h
    rfjson.h
    ratiohelper.h
)

set(BASE_SOURCES
    datautils.cpp
    rfBigInt.cpp
    rfSigUtils.cpp
    rfTimeLimit.cpp
    rfTimer.cpp
    rfUndoableObjects.cpp
    rfanimecontroler.cpp
    rfdefine.cpp
    rfmath.cpp
    unionset_basic.cpp
    ratiohelper.h
    ratiohelper.cpp
    rfjson.cpp
)


add_library(RfBase STATIC
${BASE_HEADERS}
${BASE_SOURCES}
)

target_link_libraries(RfBase
PRIVATE
    ${Boost_LIBRARIES}
    $<$<PLATFORM_ID:Windows>:bcrypt>
)

target_include_directories(RfBase
PUBLIC
    ${PROJECT_SOURCE_DIR}/thirdparty
PRIVATE
    ${Boost_INCLUDE_DIR}
    )

set(BASE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfbase)
if (EXISTS ${BASE_INCLUDE_DIR})
    file(REMOVE_RECURSE ${BASE_INCLUDE_DIR})
endif()


foreach(BASE_HEADER ${BASE_HEADERS})
    file(COPY ${BASE_HEADER} DESTINATION ${BASE_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

