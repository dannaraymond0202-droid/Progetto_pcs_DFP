#pragma once //serve per non avere errori di ridefinizione della classe quando la chiamiamo più volte
#include<iostream> //serve per ostream (che serve per stampare tutti i tipi di output)
using namespace std; //per evitare di dover ripetere std::

template <typename T>
class unidirected_edge {
private:
    T u;
    T v;
    bool inverted;    //necessario per segnalare quando un generatore di tensione ha polarità opposta al verso canonico nodo minore --> nodo maggiore 
                      //o per il verso di percorrenza delle maglie (immagazzina l'informazione del verso originale di percorrenza)

public:
    unidirected_edge(T a, T b) { //costruttore che garantisce che il nodo u sia minore del nodo v
        if(a==b){
            return;
        }
        if(a<b){
            u = a;
            v = b;
            inverted = 0; //0 = falso
        }
        else {
            u = b;
            v = a;
            inverted = 1; //1 = vero
        }
    }

    bool is_inv() const { return inverted; } //getter per sapere se durante la costruzione l'arco è stato invertito 

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
};

template <typename I>
ostream& operator<<(ostream& os, const unidirected_edge<I>& e) //operatore per stampare un arco, fuori dalla classe perché deve essere un operatore di flusso che lavora con std::ostream& come primo operando
{
    os << "(" << e.from() << ")---(" << e.to() << ")";
    return os;
}
