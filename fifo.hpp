#pragma once
#include<queue> //per std::queue dlla standard library

template<typename T>
class fifo {

private:

    std::queue<T> q; //il contenitore STL che conterrà elementi di tipo T

public:

    fifo() = default; //costruttore  di default

    void put(T val) { //inserisce in fondo alla coda
        q.push(val);
    }

    T get() {
        T val = q.front(); //estrae il valore in cima alla coda
        q.pop(); //rimuove il valore in cima alla coda
        return val; //lo restituisce
    }

    bool empty() const { //restituisce true se la coda è vuota
        return q.empty();
    }

};
