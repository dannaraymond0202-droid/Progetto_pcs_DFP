#include <cstdlib>
#include "../de_pina.hpp"

bool cycle_contains(const std::vector<unidirected_edge<int>>& ciclo,
                    const std::vector<std::pair<int,int>>& expected) {
    if (ciclo.size() != expected.size()) return false;
    for (auto& [a, b] : expected) {
        unidirected_edge<int> e(a, b);
        bool found = false;
        for (auto& ce : ciclo) {
            if (ce == e) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

int main() {

    // Circuito: 4 nodi, 5 rami → ν = 2 maglie fondamentali
    unidirected_graph<int> G;
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(0, 2);
    G.add_edge(1, 3);

    auto cicli = De_Pina(G);

    // TEST 1: numero maglie fondamentali
    if (cicli.size() != 2) {
        return EXIT_FAILURE;
    }

    // TEST 2: ciclo 0 — S₀ porta al ciclo minimo su {(0,1),(0,2),(1,2)}
    if (cicli.size() >= 1 && !cycle_contains(cicli[0], {{0,1},{0,2},{1,2}})) {
        return EXIT_FAILURE;
    }

    // TEST 3: ciclo 1 — S₁ porta al ciclo minimo su {(1,2),(2,3),(1,3)}
    if (cicli.size() >= 2 && !cycle_contains(cicli[1], {{1,2},{2,3},{1,3}})) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
