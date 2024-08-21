foreach(
  key IN
  ITEMS NUMBER_OF_LOGICAL_CORES
        NUMBER_OF_PHYSICAL_CORES
        TOTAL_VIRTUAL_MEMORY
        AVAILABLE_VIRTUAL_MEMORY
        TOTAL_PHYSICAL_MEMORY
        AVAILABLE_PHYSICAL_MEMORY
        IS_64BIT
        HAS_FPU
        HAS_MMX
        HAS_MMX_PLUS
        HAS_SSE
        HAS_SSE2
        HAS_SSE_FP
        HAS_SSE_MMX
        HAS_AMD_3DNOW
        HAS_AMD_3DNOW_PLUS
        HAS_IA64
        OS_NAME
        OS_RELEASE
        OS_VERSION
        OS_PLATFORM)
  cmake_host_system_information(RESULT _${key} QUERY ${key})
endforeach()

string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")

set(APP_PATH ${CMAKE_BINARY_DIR}/executable/)
set(APP_RESOURCE_PATH ${APP_PATH}/Files/)
set(APP_CONTEXT_PATH ${APP_RESOURCE_PATH}/context/)
set(APP_CONFIG_PATH ${APP_RESOURCE_PATH}/config/)
set(APP_LOG_PATH ${APP_RESOURCE_PATH}/log/)
set(APP_THEME_PATH ${APP_CONFIG_PATH}/UITheme/)

# Regressions
set(REG_ROOT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/regressions/)
set(REG_OUTPUT_FOLDER ${REG_ROOT_FOLDER}/output/)
set(REG_FILE_FOLDER ${REG_ROOT_FOLDER}/Files/)
set(REG_RESOURCE_FOLDER ${REG_FILE_FOLDER}/resources/)

configure_file(
  config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/build/include/generated/config.h
  @ONLY)
