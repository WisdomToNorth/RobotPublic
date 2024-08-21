# Logger
find_package(spdlog CONFIG REQUIRED)

set(LOGGER_FILES
    rfLog.cpp
    rfLog.h
    rfLogEntries.cpp
    rfLogEntries.h
    rfLogEntryType.h
    rfLogListWidget.cpp
    rfLogListWidget.h
)

add_library(RfLog STATIC ${LOGGER_FILES})

target_link_libraries(RfLog
PUBLIC
    fmt::fmt
    ${Qt_LIBRARIES}
PRIVATE
    spdlog::spdlog
)

target_include_directories(RfLog
PRIVATE
    ${PROJECT_SOURCE_DIR}/thirdparty
    )

set(LOGGER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include/rflog)
if (EXISTS ${LOGGER_INCLUDE_DIR})
    file(REMOVE_RECURSE ${LOGGER_INCLUDE_DIR})
endif()

set(LOGGER_HEADERS
    rfLog.h
    )

foreach(LOGGER_HEADER ${LOGGER_HEADERS})
    file(COPY ${LOGGER_HEADER} DESTINATION ${LOGGER_INCLUDE_DIR})
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

