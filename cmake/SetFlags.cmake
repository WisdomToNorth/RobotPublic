add_compile_options(-Wall)

message(STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID})

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    message(STATUS "Using Clang")
    add_compile_options(-std=c++20)# -Werror)

    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    message(STATUS "Using GCC")
    add_compile_options(-std=c++20)# -Werror)
    if(APPLE)
        add_compile_options(-g -O0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES IntelLLVM)
    message(STATUS "Using Intel C++ Compiler")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-O1)
    elseif(CMAKE_BUILD_TYPE MATCHES Release)
        add_compile_options(-O3)
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -Rno-debug-disables-optimization") # Use static libstdc++
endif()

if(MSVC)
    add_compile_options(
        /wd4061
        /wd4365
        /wd4464
        /wd4514
        /wd4623
        /wd4625
        /wd4626
        /wd4668
        /wd4710
        /wd4711
        /wd4820
        /wd5026
        /wd5027
        /wd5031
        /wd5045
        /wd5204
        /wd5220
    )
    option(USE_MP "use multiple" ON)
    option(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP "Set The Global Option COMPILE_FLAGS /MP to target." ON)
    if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
    set(VS_STARTUP_PROJECT ${PROJECT_NAME})

endif(MSVC)

# message(STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID})
# if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
#     list(APPEND CXX_FLAGS)# "-fno-rtti" "-fno-exceptions")
#     list(APPEND CXX_FLAGS_DEBUG)# "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
#     list(APPEND CXX_FLAGS_RELEASE)# "-O3" "-Wno-unused")
# endif()
# if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
#     SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c11")
#     SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
#     SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#     SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
#     SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

#     SET (CMAKE_CXX_FLAGS_INIT                "-Wall -std=c++17")
#     SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
#     SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#     SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
#     SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
# endif()


# set(_CXX_FLAGS)
# if(_march_native_works)
#   message(STATUS "Using processor's vector instructions (-march=native compiler flag set)")
#   set(_CXX_FLAGS "-march=native")
# elseif(_xhost_works)
#   message(STATUS "Using processor's vector instructions (-xHost compiler flag set)")
#   set(_CXX_FLAGS "-xHost")
# else()
#   message(STATUS "No suitable compiler flag found for vectorization")
# endif()
