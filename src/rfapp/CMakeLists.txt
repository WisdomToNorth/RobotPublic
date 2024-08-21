# app

set(RFAPP_FILES
    app_settings.cpp
    app_settings.h
    apptheme.cpp
    apptheme.h
    delegate.h
    observer.h
    revoke.cpp
    revoke.h
    task_manager.cpp
    task_manager.h
    task_progress.cpp
    task_progress.h
    textid.h
    textid.cpp
    rfsignal.h
    rfsignal.cpp
    rfPropertyGroup.h
    rfPropertyGroup.cpp
    rfProperty.h
    rfProperty.cpp
    rfPropertyGeneric.h
    rfPropertyGeneric.cpp
    rfunit.cpp
    rfunit_system.cpp
    rfunit.h
    rfunit_system.h
    rfquantity.h
    rfFilepath.h
    rfquantity.cpp
)

add_library(RfApp STATIC ${RFAPP_FILES})

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

set(RFAPP_HEADERS
    app_settings.h
    apptheme.h
    delegate.h
    observer.h
    revoke.h
    task_manager.h
    task_progress.h
    textid.h
    rfsignal.h
    rfPropertyGroup.h
    rfProperty.h
    rfPropertyGeneric.h
    rfunit.h
    rfunit_system.h
    rfquantity.h
    rfFilepath.h
)

foreach(RFAPP_HEADER ${RFAPP_HEADERS})
    file(COPY ${RFAPP_HEADER} DESTINATION ${RFAPP_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)