#include <iostream>
#include <cstdlib>
#include "../cicli_dfs.hpp"

bool cycle_contains(const std::vector<unidirected_edge<int>>& ciclo, const std::vector<std::pair<int,int>>& expected) {
    if (ciclo.size() != expected.size()) return false;
    for (auto& [a, b] : expected) {
        unidirected_edge<int> e(a, b);
        bool found = false;
        for (auto& ce : ciclo) {
            if (ce == e) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

int main() {

    // Grafo planare: 4 nodi, 5 rami → ν = 2 maglie fondamentali
    unidirected_graph<int> G;
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(0, 2);
    G.add_edge(1, 3);

    auto cicli = fundamental_cycles(G);

    // TEST 1: numero cicli fondamentali
    if (cicli.size() != 2) {
        return EXIT_FAILURE;
    }

    // TEST 2: primo ciclo (back edge 1-2) → {(0,1),(0,2),(1,2)}
    if (!cycle_contains(cicli[0], {{0,1},{0,2},{1,2}})) {
        return EXIT_FAILURE;
    }

    // TEST 3: secondo ciclo (back edge 1-3) → {(0,1),(0,2),(2,3),(1,3)}
    if (!cycle_contains(cicli[1], {{0,1},{0,2},{2,3},{1,3}})) {
        return EXIT_FAILURE;
    }

    // TEST 4: find_path sul solo albero DFS (0→3 passando per 0-2-3)
    unidirected_graph<int> albero;
    albero.add_edge(0, 1);
    albero.add_edge(0, 2);
    albero.add_edge(2, 3);
    auto path = find_path(albero, 0, 3);
    if (path.size() != 2 || !(path[0] == unidirected_edge<int>(0,2)) || !(path[1] == unidirected_edge<int>(2,3))) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
