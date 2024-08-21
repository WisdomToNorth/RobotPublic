
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/regressions/Files)
if (EXISTS ${RESOURCE_DIR})
    file(REMOVE_RECURSE ${RESOURCE_DIR})
endif()

# Make the copy, to avoid changing the original files by mistake
file(COPY ${PROJECT_SOURCE_DIR}/Files DESTINATION ${PROJECT_SOURCE_DIR}/regressions)

