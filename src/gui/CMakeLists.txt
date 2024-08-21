# RobotGui

set(CONTEXT_EDITOR_FILES

    app_flag.h
    app_manager.h
    app_system.h
    halio.h
    mode_var.h
    selectmodehelper.h
    viewer_observer.h
    viewer_utils.h
    viewerwrapper.h
    viewnode.h
    viewnode_utils.h
    viewrobot.h
    app_manager.cpp
    app_system.cpp
    halio.cpp
    main.cpp
    mode_var.cpp
    selectmodehelper.cpp
    viewer_observer.cpp
    viewer_utils.cpp
    viewnode.cpp
    viewnode_utils.cpp
    viewrobot.cpp

    ui/app_mode_dock.h
    ui/cmd_dock.h
    ui/file_explore_dock.h
    ui/file_widget.h
    #ui/gui_controls.h
    ui/gui_positions.h
    ui/mainwindow.h
    ui/select_mode_widget.h
    ui/viewerwidget.h
    ui/app_mode_dock.cpp
    ui/cmd_dock.cpp
    ui/file_explore_dock.cpp
    ui/file_widget.cpp
    #ui/gui_controls.cpp
    ui/gui_positions.cpp
    ui/mainwindow.cpp
    ui/select_mode_widget.cpp
    ui/viewerwidget.cpp

    frameutils/qtutils.h
    frameutils/qtutils.cpp

    command/cmd_base.h
    command/cmd_base.cpp
    command/cmd_changecontext.h
    command/cmd_changecontext.cpp
    command/cmd_drawdemo.h
    command/cmd_drawdemo.cpp
    command/cmd_ruler.h
    command/cmd_ruler.cpp
    command/cmd_importfile.h
    command/cmd_importfie.cpp
    command/cmd_exportfile.h
    command/cmd_exportfile.cpp
    command/cmd_configrobot.h
    command/cmd_configrobot.cpp
    command/cmd_move.h
    command/cmd_move.cpp
    command/cmd_move_ui.h
    command/cmd_move_ui.cpp
    command/cmd_removecontext.cpp
    command/cmd_removecontext.h
    command/cmd_newcontext.cpp
    command/cmd_newcontext.h
    command/cmd_remove.h
    command/cmd_remove.cpp
    command/cmd_viewerpan.cpp
    command/cmd_viewerpan.h
    command/cmd_viewerzoom.cpp
    command/cmd_viewerzoom.h
    command/cmd_viewerrot.h
    command/cmd_viewerrot.cpp
    command/cmd_showinfo.h
    command/cmd_showinfo.cpp
    command/cmd_setting.h
    command/cmd_setting.cpp
    command/cmd_robotfor.h
    command/cmd_robotfor.cpp
    command/cmd_robotmove.h
    command/cmd_robotmove.cpp
    command/cmdmanager.h
    command/cmdmanager.cpp
    command/cmdregister.h
    command/cmdregister.cpp
    command/cmdicon.h
    command/cmdicon.cpp
    command/cmddefine.h
    command/cmddefine.cpp

    ui/form/listwidget.h
    ui/form/listwidget.cpp
    ui/form/rangebar.h
    ui/form/rangebar.cpp
    ui/form/filesystemwidget.h
    ui/form/filesystemwidget.cpp
)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/executable)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#ICON
if(APPLE)
    AddIconToBinary(ROBOT_GUI_FILES ICONS ${CMAKE_CURRENT_SOURCE_DIR}/icons/robot.icns)
    add_executable(RobotGui
    MACOSX_BUNDLE
    ${ROBOT_GUI_FILES}
    ${CONTEXT_EDITOR_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/contexteditor.qrc
)
else() #windows, linux
    if(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_SKIP_BUILD_RPATH FALSE)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
        set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/libs")
    endif()
    add_executable(RobotGui
    ${CONTEXT_EDITOR_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/contexteditor.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/myapp.rc)
endif()

if(MSVC)
    set_target_properties(RobotGui PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
endif()

target_include_directories(RobotGui
PRIVATE
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/build/include
    ${OpenCASCADE_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}/src/gui/Gui_autogen/include)

target_link_directories(RobotGui
PRIVATE
    ${OpenCASCADE_LIBRARY_DIR}
    ${Boost_LIBRARY_DIR}
    ${Qt_LIBRARY_DIR})

target_link_libraries(RobotGui
PRIVATE
    ${Qt_LIBRARIES}
    ${OpenCASCADE_LIBRARIES}
    ${Boost_LIBRARIES}
    RfBase
    RfGraphic
    RfGeo3d
    RfDB
    RfApp
    RfLog
    RfIO)


set(RESOURCE_DIR ${CMAKE_BINARY_DIR}/Files)
if (EXISTS ${RESOURCE_DIR})
    file(REMOVE_RECURSE ${RESOURCE_DIR})
endif()

file(COPY ${PROJECT_SOURCE_DIR}/Files DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
