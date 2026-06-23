cmake_minimum_required(VERSION 3.20)

project(test_unitari)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")


## Creo i target per gli eseguibili che effettueranno i test unitari

add_executable(test_cicli_dfs test_cicli_dfs.cpp)
add_test(NAME cicli_dfs COMMAND test_cicli_dfs)

add_executable(test_maps_to_graph test_maps_to_graph.cpp)
add_test(NAME maps_to_graph COMMAND test_maps_to_graph)

enable_testing()
