cmake_minimum_required(VERSION 3.10.2)
project(JimmyNeuron VERSION 0.10)


if (WIN32)# AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/../src/SFML/lib")


    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )


    file(GLOB_RECURSE RES_FILES "res" "res/*")
    foreach(file ${RES_FILES})

        file(RELATIVE_PATH relfile ${CMAKE_SOURCE_DIR} ${file})

        set(newFileName "${relfile}")

        get_filename_component(newFile ${newFileName} DIRECTORY)

        if(NOT EXISTS bin/${newFileName})
            file(COPY ${file} DESTINATION "bin/${newFile}")
            message("copy ${relfile}")

            message(" ${file} copied.")
        elseif(${file} IS_NEWER_THAN bin/${newFileName})
            file(COPY "${file}" DESTINATION "bin/${newFile}")
            message("merge ${relfile}")
        endif()
        
        if(NOT EXISTS ${newFileName})
            file(COPY ${file} DESTINATION ${newFile})
            message("copy ${relfile}")

            message(" ${file} copied.")
        elseif(${file} IS_NEWER_THAN ${newFileName})
            file(COPY "${file}" DESTINATION "${newFile}")
            message("merge ${relfile}")
        endif()
    endforeach()

else()


    # it suuposed to copy file if there is a new wersion of it
    # probably, i still need to change it cause it's not working correctly 
    file(GLOB_RECURSE RES_FILES "res" "res/*")
    foreach(file ${RES_FILES})

        file(RELATIVE_PATH relfile ${CMAKE_SOURCE_DIR} ${file})

        set(newFileName "${relfile}")

        get_filename_component(newFile ${newFileName} DIRECTORY)

        if(NOT EXISTS ${newFileName})
            file(COPY ${file} DESTINATION ${newFile})
            message("copy ${relfile}")

            message(" ${file} copied.")
        elseif(${file} IS_NEWER_THAN ${newFileName})
            file(COPY "${file}" DESTINATION "${newFile}")
            message("merge ${relfile}")
        endif()
    endforeach()

endif()

add_subdirectory(src) # adding libraries
add_subdirectory(bin) # linking to executable
