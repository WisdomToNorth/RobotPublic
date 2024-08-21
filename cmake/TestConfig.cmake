#CTest configuration
# set(MEMORYCHECK_TYPE "AddressSanitizer")
# set(MEMORYCHECK_SANITIZER_OPTIONS "verbosity=1:symbolize=1:abort_on_error=1:detect_leaks=1")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

include(CTest)
if(CTEST_COMMAND)
    set_property(DIRECTORY APPEND PROPERTY CTEST_CUSTOM_ARGUMENTS "--schedule-random")
endif()
enable_testing()
