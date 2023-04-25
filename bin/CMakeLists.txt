
file (GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS ./*.cpp)
foreach (example ${EXAMPLES})
    get_filename_component      (  example_name  ${example} NAME_WLE)
    add_executable              (${example_name} ${example})
    target_link_libraries       (${example_name} JimmyNeuron ${JIMMY_NEURON_CORE_LIBS} ${JIMMY_NEURON_OPTIONAL_LIBRIES})
    target_compile_features     (${example_name} PRIVATE cxx_std_20)
    target_compile_definitions  (${example_name} PUBLIC ${JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS})
endforeach()