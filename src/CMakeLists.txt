file(GLOB_RECURSE JIMMY_CPPS "Jimmy" "*JimmyNeuron/*.hpp")
add_library(
    Jimmy
    STATIC
    JimmyNeuron/Jimmy.hpp
    JimmyNeuron/Jimmy.cpp
)
#target_link_directories(Ui PUBLIC sfml-widgets/lib/libsfml-widgets)
target_include_directories(Jimmy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
