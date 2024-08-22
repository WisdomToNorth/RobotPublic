# Base

set(RFGRAPHIC_HEADERS
ais_text.h
graphics_create_driver.h
graphics_create_virtual_window.h
graphics_object_ptr.h
graphics_owner_ptr.h
graphics_scene.h
graphics_texture2d.h
graphics_utils.h
graphics_view_ptr.h
occ_handle.h
occt_window.h
system_type.h
tkernel_utils.h
v3d_view_camera_animation.h
v3d_view_controller.h
view3d_navigation_style.h
widget_occ_view.h
widget_occ_view_controller.h
rfviewer.h
rfviewernew.h
rfviewevent.h
gui_vkey_mouse.h
)
set(RFGRAPHIC_SOURCES
ais_text.cpp
graphics_create_driver.cpp
graphics_create_virtual_window.cpp
graphics_scene.cpp
graphics_utils.cpp
occt_window.cpp
tkernel_utils.cpp
v3d_view_camera_animation.cpp
v3d_view_controller.cpp
widget_occ_view.cpp
widget_occ_view_controller.cpp
widget_occ_view_impl.cpp
widget_occ_view_impl.h
rfviewer.cpp
rfviewer.h
rfviewernew.h
rfviewernew.cpp
rfviewevent.h
gui_vkey_mouse.h
)

add_library(RfGraphic STATIC
    ${RFGRAPHIC_HEADERS}
    ${RFGRAPHIC_SOURCES}
)

target_link_libraries(RfGraphic
PRIVATE
    ${Qt_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    RfBase
)

target_link_directories(RfGraphic
PRIVATE
    ${OpenCASCADE_LIBRARY_DIR}
    ${Qt_LIBRARY_DIR})

target_include_directories(RfGraphic
PRIVATE
    ${OpenCASCADE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/build/include
    )

set(RFGRAPHIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rfgraphics)
if (EXISTS ${RFGRAPHIC_INCLUDE_DIR})
    file(REMOVE_RECURSE ${RFGRAPHIC_INCLUDE_DIR})
endif()


foreach(RFGRAPHIC_HEADER ${RFGRAPHIC_HEADERS})
    file(COPY ${RFGRAPHIC_HEADER} DESTINATION ${RFGRAPHIC_INCLUDE_DIR})
endforeach()

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

