file(GLOB_RECURSE JIMMY_HPPS "Jimmy" "*.hpp")
file(GLOB_RECURSE JIMMY_CPPS "Jimmy" "*.cpp")

add_library(
    Jimmy
    STATIC
    Jimmy.hpp
    Jimmy.cpp
    ${JIMMY_CPPS}
    ${JIMMY_HPPS}
)
target_include_directories(Jimmy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
