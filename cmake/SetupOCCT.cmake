#for github action
set(Old_occ_cmake_folder ${CMAKE_CURRENT_SOURCE_DIR}/occt-vc143-64/cmake)
set(new_occ_cmake_folder ${CMAKE_CURRENT_SOURCE_DIR}/.github/win_occ)

set(TO_REPLACE_FILES
    OpenCASCADEModelingDataTargets.cmake
    OpenCASCADEDataExchangeTargets.cmake
    OpenCASCADEDrawTargets.cmake
    OpenCASCADEFoundationClassesTargets.cmake
    OpenCASCADEModelingAlgorithmsTargets.cmake
    OpenCASCADEVisualizationTargets.cmake
)

set(Old_occ_cmake_file ${Old_occ_cmake_folder}/OpenCASCADEConfig.cmake)
if(EXISTS ${Old_occ_cmake_file})
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/.github/win_occ/lib)
    message(STATUS "link_directories(${CMAKE_CURRENT_SOURCE_DIR}/.github/win_occ/lib)")

    #remove old file
    foreach(to_replace_file ${TO_REPLACE_FILES})
        set(Old_occ_cmake_file ${Old_occ_cmake_folder}/${to_replace_file})
        if(EXISTS ${Old_occ_cmake_file})
            file(REMOVE ${Old_occ_cmake_file})
        endif()
    endforeach()

    #copy new file
    foreach(to_replace_file ${TO_REPLACE_FILES})
        set(new_occ_cmake_file ${new_occ_cmake_folder}/${to_replace_file})
        if(EXISTS ${new_occ_cmake_file})
            file(COPY ${new_occ_cmake_file} DESTINATION ${Old_occ_cmake_folder})
        endif()
    endforeach()

endif()
