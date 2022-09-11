if (NOT TARGET JimmyNeuron)
# Create library
    file(GLOB_RECURSE ERPPM_FILES_CPP "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
    file(GLOB_RECURSE ERPPM_FILES_HPP "${CMAKE_CURRENT_LIST_DIR}/../include/*.hpp" "${CMAKE_CURRENT_LIST_DIR}/../include/*.inl")

    add_library(
        JimmyNeuron
        STATIC
        ${ERPPM_FILES_HPP}
        ${ERPPM_FILES_CPP}
    )
    target_link_libraries       (JimmyNeuron PUBLIC ${JIMMY_NEURON_CORE_LIBS} ${JIMMY_NEURON_OPTIONAL_LIBRIES})
    target_compile_definitions  (JimmyNeuron PUBLIC ${JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS})
    target_include_directories  (JimmyNeuron PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include")
endif()


