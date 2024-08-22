cmake_minimum_required(VERSION 3.21)

project(
  RobotFramework
  VERSION 1.2.1
  LANGUAGES CXX)

set(CONTEXT_VERSION 1.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(C)

# Print the C++ compiler name and path
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

if(DEFINED ENV{DISPLAY})
  set(BUILD_WITH_GUI ON)
else()
  set(BUILD_WITH_GUI OFF)
endif()

if(BUILD_WITH_GUI)
  add_definitions(-DENABLE_GUI)
endif()

if(WIN32)
    #need by github action windows
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SetUpOCCT.cmake)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/PrintMessage.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CopyRegFile.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/SetFlags.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/TestConfig.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/KConfig.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindPKG.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/AddIconToBinary.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Doxygen.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfbase)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfgraphics)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rflog)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfdb)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfapp)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfgeo3d)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfio)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/rfrobot)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/gui)
