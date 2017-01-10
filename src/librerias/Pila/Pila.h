//*****************************************************************
// File:   pila.h
// Date:   18 de diciembre 2016
//*****************************************************************
/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como una pila, desarrollado en la práctica
2 de la asignatura de EDA
******************************************************************/

#ifndef PILA_H
#define PILA_H

/***********************/
/***  PARTE PUBLICA  ***/
/***********************/
using namespace std;

/*
* Los valores del TAD pila representan secuencias de elementos con
* acceso LIFO (last in, first out), esto es, el último elemento añadido
* será el primero en ser borrado.
*/
template <typename Dato> struct Pila;

/*
* Devuelve en pila la pila vacía, sin elementos
*/
template <typename Dato> void crear(Pila<Dato>& pila);

/*
* Devuelve en pila la pila vacía y además libera la memoria utilizada previamente por pila
*/
template <typename Dato> void liberar(Pila<Dato>& pila);

/*
* Devuelve en pila la pila resultante de añadir d a pila
*/
template <typename Dato> void apilar(Pila<Dato>& pila, const Dato& d);

/*
* Si pila es no vacía, devuelve en pila la pila resultante de eliminar de pila el último
* elemento que fue apilado y devuelve en d este elemento
*
* En el caso de que la operacion se realize correctamente, y pila y d correspondan a
* estos valores, la función devuelve true.
*/
template <typename Dato> bool desapilar(Pila<Dato>& pila, Dato& d);

/*
* Si pila es no vacía devuelve en d el último elemento en ser apilado
*
* En el caso de que la operacion se realize correctamente la función devuelve true.
*/
template <typename Dato> bool cima(const Pila<Dato>& pila, Dato& d);

/*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
template <typename Dato> bool esVacia(const Pila<Dato>& pila);

/*
* Devuelve el número de elementos de la pila pila
*/
template <typename Dato> unsigned int tamanyo(const Pila<Dato>& pila);

/***********************/
/***  PARTE PRIVADA  ***/
/***********************/
template <typename Dato>
struct Pila{
    friend void crear<Dato>(Pila<Dato>& pila);
    friend void liberar<Dato>(Pila<Dato>& pila);
    friend void apilar<Dato>(Pila<Dato>& pila, const Dato& d);
    friend bool desapilar<Dato>(Pila<Dato>& pila, Dato& d);
    friend bool cima<Dato>(const Pila<Dato>& pila, Dato& d);
    friend bool esVacia<Dato>(const Pila<Dato>& pila);
    friend unsigned int tamanyo<Dato>(const Pila<Dato>& pila);

private:
    struct Nodo{
        Dato dato;
        Nodo* anterior;
    };
    Nodo* cima;
    unsigned int size;
};

/************************/
/***  IMPLEMENTACIÓN  ***/
/************************/

template <typename Dato> void crear(Pila<Dato>& p) {
    p.size = 0;
    p.cima = nullptr;
}

template <typename Dato> void liberar(Pila<Dato>& p){
  while (p.size>0) {
    typename Pila<Dato>::Nodo* aux = p.cima->anterior;
    delete p.cima;
    p.cima = aux;
    aux = nullptr;
    --p.size;
  }
}

template <typename Dato> void apilar(Pila<Dato>& p, const Dato& d) {
        typename Pila<Dato>::Nodo* aux = new typename Pila<Dato>::Nodo;
        aux->anterior = p.cima;
        aux->dato = d;
        p.cima = aux;
        ++p.size;
}

template <typename Dato> bool desapilar(Pila<Dato>& p, Dato& d) {
    if (!esVacia(p)){
        d = p.cima->dato;
        typename Pila<Dato>::Nodo* aux = p.cima->anterior;
        delete p.cima;
        p.cima = aux;
        aux = nullptr;
        --p.size;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename Dato> bool cima(const Pila<Dato>& p, Dato& d) {
    if (!esVacia(p))
    {
        d = p.cima->dato;
        return true;
    }
    else
    {
        return false;
    }

}

template <typename Dato> bool esVacia(const Pila<Dato>& pila) {
    return tamanyo(pila) == 0;
}

template <typename Dato> unsigned int tamanyo(const Pila<Dato>& pila) {
    return pila.size;
}

#endif //PILA_H
