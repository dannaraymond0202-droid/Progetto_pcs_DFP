#pragma once //serve per non avere errori di ridefinizione della classe quando la chiamiamo più volte
#include<iostream> //serve per ostream (che serve per stampare tutti i tipi di output)

template <typename T>
class unidirected_edge {
private:
    T u;
    T v;

public:
    unidirected_edge(T a, T b) { //costruttore che garantisce che il nodo u sia minore del nodo v
        if(a==b){
            std::cout<<"Errore non può esserci un arco con due nodi uguali";
            return;
        }
        if(a<b){
            u = a;
            v = b;
        }
        else {
           u = b;
           v = a; 
        }
    }

    T from() const { //getter per leggere u
        return u;
    }
    T to() const { //getter per leggere v
        return v;
    }

bool operator<(const unidirected_edge& other) const { //operatore per definire quando un arco è minore di un altro
    if(u != other.u) {
        return u < other.u;
    }
    else {
        return v < other.v;
    }
}

bool operator==(const unidirected_edge& other) const { //operatore per definire quando un arco è uguale a un altro
    if(((u == other.u)&&(v == other.v)) || ((u == other.v)&&(v == other.u))) { // in un arco non orientato (a,b) = (b,a)
        return true;
    }
    else {
        return false;
    }
}

friend std::ostream& operator<<(std::ostream& os, const unidirected_edge& e) { //friend per accedere a membri privati //operatore per stampare un arco
    os << "(" << e.u << "," << e.v << ")"; 
    return os;
}

};
