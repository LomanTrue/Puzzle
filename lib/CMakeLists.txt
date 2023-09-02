add_library(application application.cpp application.h)
add_library(puzzle puzzle.cpp puzzle.h)
add_library(DSU DSU.cpp DSU.h)

target_link_libraries(application sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(puzzle sfml-system sfml-window sfml-graphics sfml-audio)