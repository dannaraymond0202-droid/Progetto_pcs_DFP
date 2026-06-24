#include<iostream>
#include<fstream>
#include<map>
#include<list>
#include"../graph_visit.hpp"

//scrive un file .dot per GraphViz
void genera_dot(const unidirected_graph<int>& G, const std::string& filename){
    std::ofstream ofs(filename);   //output file stream, scrive su un file di testo
    ofs << "graph G{\n";
    for(auto e: G.all_edges()){
        ofs<< "   " << e.from() << " -- " << e.to() << "  \n";
    }
    ofs<<"}\n";
    ofs.close();
}

int main() {
    //costruisco il mio grafo
    unidirected_graph<int> G;
    G.add_edge(1,2);
    G.add_edge(1,3);
    G.add_edge(2,3);
    G.add_edge(3,4);
    G.add_edge(3,5);
    G.add_edge(4,5);
    G.add_edge(4,6);
    G.add_edge(5,6);

    //stampo il grafo originale
    std::cout<<"Grafo originale"<<"\n";
    for(auto e : G.all_edges()){
        std::cout<< e << " ";
    }
    std::cout<<"\n\n";
    genera_dot(G, "grafo.dot");

    //BFS
    fifo<int> f;
    unidirected_graph<int> bfsG= graph_visit(G, 1, f);
    std::cout<<"Albero BFS"<<"\n";
    for(auto e : bfsG.all_edges()){
        std::cout<< e << " ";
    }
    std::cout<<"\n\n";
    genera_dot(bfsG, "bfs.dot");

    //DFS
    lifo<int> l;
    unidirected_graph<int> dfsG= graph_visit(G, 1, l);
    std::cout<<"Albero DFS"<<"\n";
    for(auto e : dfsG.all_edges()){
        std::cout<< e << " ";
    }
    std::cout<<"\n\n";
    genera_dot(dfsG, "dfs.dot");

    //DFS ricorsiva
    unidirected_graph<int> rdfsG= recursive_dfs(G, 1);
    std::cout<<"Albero DFS ricorsiva"<<"\n";
    for(auto e : rdfsG.all_edges()){
        std::cout<< e << " ";
    }
    std::cout<<"\n\n";
    genera_dot(rdfsG, "rdfs.dot");

    //Dijkstra
    std::map<unidirected_edge<int>, double> pesi;
    for(auto& e: G.all_edges()){
        pesi[e] = 1;
    }
   
    std::map<int, int> pred = Dijkstra(G, 1, pesi);

    std::cout<<"Dijkstra nodo sorgente 1"<<"\n";
    std::cout<<"Predecessori"<<"\n";
    for(auto it=pred.begin(); it!= pred.end(); ++it){
        std::cout<<"pred["<< (*it).first <<"]" <<"=" << (*it).second << "\n";
    }
    std::cout<<"\n";

    //percorso da 1 a 6
    std::list<int> cammino;
    int nodo = 6;
    while(nodo != 1){
        cammino.push_front(nodo); //push front per mettere all'inizio della lista
        nodo = pred[nodo];
    }
    cammino.push_front(1);

    std::cout<<"Cammino minimo da 1 a 6: ";
    for(auto it=cammino.begin(); it != cammino.end(); ++it){
        if(it != cammino.begin()){
            std::cout<< " -> ";
        }
        std::cout<< *it;
    }
    std::cout<< "\n";

    return 0;

}
