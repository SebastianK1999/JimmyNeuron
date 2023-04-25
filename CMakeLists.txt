cmake_minimum_required(VERSION 3.10.2)
project(EvolutionaryRobotics_PredatorPreyModel VERSION 0.10)

if (WIN32) # Include win64 platforms


    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS 
        "/wd4018 /wd4244 /wd4305 
        /D_CRT_SECURE_NO_WARNINGS 
        /D_CRT_NONSTDC_NO_DEPRECATE 
        /D NOMINMAX /DGLEW_STATIC
        /EHsc"
     )

    set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT\" /NODEFAULTLIB:\"LIBCMT\"")
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
    set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

elseif (APPLE)

    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wno-deprecated-declarations -Wno-reorder-ctor -Wno-unused-function -Wno-unused-variable -g -stdlib=libc++ -std=c++20")
    add_definitions(-DAPPLE)

elseif (UNIX)

    set(CMAKE_CXX_FLAGS "-Wall -pedantic -march=native -O3 -g -std=c++20 -Wno-comment -Wno-sign-compare -Wno-reorder -Wno-unused-function")
    add_definitions(-DUNIX)

endif()
SET(JIMMY_NEURON_CORE_LIBS "${JIMMY_NEURON_CORE_LIBS}" CACHE INTERNAL "JIMMY_NEURON_CORE_LIBS")

add_custom_target(JIMMY_NEURON_RESOURCES_DIR SOURCES "${CMAKE_CURRENT_LIST_DIR}/res/")
add_custom_command(TARGET JIMMY_NEURON_RESOURCES_DIR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_LIST_DIR}/res/"
    "${CMAKE_CURRENT_LIST_DIR}/build/res/"
)   

add_subdirectory("external/") # linking to submodules
add_subdirectory("submodules/") # linking to submodules
add_subdirectory("src/") # adding sources
if  (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("bin/") # adding libraries
endif()
