# app
set(RFAPP_HEADERS
    app_settings.h
    apptheme.h
    delegate.h
    observer.h
    revoke.h
    rfFilepath.h
    rfProperty.h
    rfPropertyGeneric.h
    rfPropertyGroup.h
    rfquantity.h
    rfsignal.h
    rfunit.h
    rfunit_system.h
    task_manager.h
    task_progress.h
    textid.h
)

set(RFAPP_SOURCES
    app_settings.cpp
    apptheme.cpp
    revoke.cpp
    rfProperty.cpp
    rfPropertyGeneric.cpp
    rfPropertyGroup.cpp
    rfquantity.cpp
    rfsignal.cpp
    rfunit.cpp
    rfunit_system.cpp
    task_manager.cpp
    task_progress.cpp
    textid.cpp

)

add_library(RfApp STATIC
    ${RFAPP_HEADERS}
    ${RFAPP_SOURCES}
)

target_link_libraries(RfApp
PRIVATE
    ${OpenCASCADE_LIBRARIES}
    RfBase
    RfLog
    )

target_include_directories(RfApp PRIVATE
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/build/include
    ${OpenCASCADE_INCLUDE_DIR}
 )

set(RFAPP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfapp)
if (EXISTS ${RFAPP_INCLUDE_DIR})
    file(REMOVE_RECURSE ${RFAPP_INCLUDE_DIR})
endif()


foreach(RFAPP_HEADER ${RFAPP_HEADERS})
    file(COPY ${RFAPP_HEADER} DESTINATION ${RFAPP_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)