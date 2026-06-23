#pragma once
#include<stack> //per std::stack dlla standard library

template<typename T>
class lifo {

private:

    std::stack<T> s; //il contenitore STL che conterrà elementi di tipo T

public:

    lifo() = default; //costruttore  di default

    void put(T val) {
       s.push(val);  //inserisce elemento in cima alla pila
    }

    T get() {
        T val = s.top(); //accede all'elemento in cima alla pila
        s.pop(); //rimuove l'elemento in cima alla pila
        return val; //lo restituisce
    }
    
    bool empty() const { //restituisce true se la pila è vuota
        return s.empty();
    }
};
