#file(GLOB_RECURSE JIMMY_HPPS "Jimmy" "*.hpp")
#file(GLOB_RECURSE JIMMY_CPPS "Jimmy" "*.cpp")

add_library(
    Games
    STATIC
    DinoGame.hpp
    DinoGame.cpp
    TicTacToe.hpp
    TicTacToe.cpp
)
target_include_directories(Games PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(Games PUBLIC Jimmy sfml-graphics sfml-window sfml-system)
