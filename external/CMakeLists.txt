set(JIMMY_NEURON_OPTIONAL_LIBRIES "")
set(JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS "")
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/SFML/")

    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/SFML/")
    set(JIMMY_NEURON_OPTIONAL_LIBRIES "sfml-graphics;sfml-window;sfml-system;")
    set(JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS "${JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS} OPTIONAL_LIBRARY_SFML")

endif()

SET(JIMMY_NEURON_OPTIONAL_LIBRIES "${JIMMY_NEURON_OPTIONAL_LIBRIES}" CACHE INTERNAL "JIMMY_NEURON_OPTIONAL_LIBRIES")
SET(JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS "${JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS}" CACHE INTERNAL "JIMMY_NEURON_OPTIONAL_LIBRIES_DEFINITIONS")
