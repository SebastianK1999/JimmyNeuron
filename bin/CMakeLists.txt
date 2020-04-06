
add_executable(JimmyNeuronDemo demo.cpp)

target_link_libraries(JimmyNeuronDemo PUBLIC
    # Our
    Jimmy
    
    # External
)


if (WIN32)

    install(TARGETS JimmyNeuronDemo
        EXPORT "MyProjectTargets"
        RUNTIME DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        LIBRARY DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        ARCHIVE DESTINATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    )

    #target_link_options(JimmyNeuronDemo PRIVATE -mwindows)

endif()

