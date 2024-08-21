
# QT
find_package(QT NAMES Qt6 Qt5 REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui REQUIRED)

set(Qt_LIBRARIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
    list(APPEND Qt_LIBRARIES Qt6::OpenGLWidgets)
endif()

set(Qt_LIBRARY_DIR ${Qt${QT_VERSION_MAJOR}_LIBRARY_DIR})
set(Qt_INCLUDE_DIR ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(GTest REQUIRED)
find_package(FMT CONFIG REQUIRED)
find_package(OpenCASCADE CONFIG REQUIRED)

set(Boost_NO_WARN_NEW_VERSIONS TRUE)
find_package(Boost REQUIRED COMPONENTS system)

enable_testing()
include(GoogleTest)

