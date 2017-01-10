/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
******************************************************************/

#ifndef LISTAORDENADA_H
#define LISTAORDENADA_H

/*
* Los valores del TAD ListaOrdenada representan una secuencias de elementos ordenados
*/

template<typename Dato>
class ListaOrdenada {
public:
/*
* Pre: ---
* Post: Crea una lista vacía, sin elementos
*/
    ListaOrdenada();

/*
* Pre: ---
* Post: Libera la memoria usada por la lista
*/
    ~ListaOrdenada();

/*
* Pre: ---
* Post: Devuelve una lista vacia y libera la memoria usada por la lista anterior
*/
    void clear();

/*
* Pre: ---
* Post: Devuelve  la lista resultante de añadir d a lista
*/
    void add(const Dato &d);

/*
* Pre: ---
* Post: Devuelve cierto si y solo si d pertenece a la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    bool belongs(const Dato &d);

/*
* Pre: ---
* Post: Devuelve ciero en el caso de que no exista ningún elemento en la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    bool empty();

/*
* Pre: ---
* Post: Devuelve el numero de elementos de la lista
*/
    int size();

/*
* Pre: ---
* Post: Pone el iterador de la lista al principio
*/
    void begin();

/*
* Pre: Se ha ejecutado previamente begin
* Post: Asigna el elemento apuntado por el iterador a d y avanza el iterador
*       Devuelve ciero solo si existen más elementos por visitar en la lista
*/
    bool next(Dato &d);

/*
* Pre: ---
* Post: Copia la lista en copied
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    void copy(ListaOrdenada<Dato> &copied);

private:
    struct Nodo {
        Dato dato;
        Nodo *siguiente;
    };
    Nodo *primero;
    Nodo *ultimo;
    Nodo *iterador;
    unsigned int dim;

};


/*
* Pre: ---
* Post: Crea una lista vacía, sin elementos
*/
template<typename Dato>
ListaOrdenada<Dato>::ListaOrdenada() {
    this->primero = nullptr;
    this->ultimo = nullptr;
    this->dim = 0;
    this->iterador = nullptr;
}

/*
* Pre: ---
* Post: Libera la memoria usada por la lista
*/
template<typename Dato>
ListaOrdenada<Dato>::~ListaOrdenada() {
    while (this->dim > 0) {
        typename ListaOrdenada<Dato>::Nodo *aux = this->primero->siguiente;
        delete this->primero;
        this->primero = aux;
        aux = nullptr;
        this->dim = this->dim - 1;
    }
}

/*
* Pre: ---
* Post: Devuelve una lista vacia y libera la memoria usada por la lista anterior
*/
template<typename Dato>
void ListaOrdenada<Dato>::clear() {
    while (this->dim > 0) {
        typename ListaOrdenada<Dato>::Nodo *aux = this->primero->siguiente;
        delete this->primero;
        this->primero = aux;
        aux = nullptr;
        this->dim = this->dim - 1;
    }
    this->ultimo = nullptr;
    this->iterador = nullptr;
}

/*
* Pre: ---
* Post: Devuelve  la lista resultante de añadir d a lista
*/
template<typename Dato>
void ListaOrdenada<Dato>::add(const Dato &d) {
    if (this->size() > 0) {
        typename ListaOrdenada<Dato>::Nodo *actual;
        actual=this->primero;
        if(actual->dato>d){//Se inserta al principio
            this->primero= new typename ListaOrdenada<Dato>::Nodo;
            this->primero->dato = d;
            this->primero->siguiente=actual;
            this->dim = this->dim + 1;

        }else{
            for (int i = 0; i < this->dim; ++i) {
                if(actual->siguiente== nullptr){//El dato es el ultimo
                    this->ultimo->siguiente = new typename ListaOrdenada<Dato>::Nodo;
                    this->ultimo = this->ultimo->siguiente;
                    this->ultimo->dato = d;
                    this->ultimo->siguiente = nullptr;
                    this->dim = this->dim + 1;
                    break;
                }
                if (actual->siguiente->dato > d) {//Se inserta antes del final
                    typename ListaOrdenada<Dato>::Nodo *aux = actual->siguiente;
                    actual->siguiente = new typename ListaOrdenada<Dato>::Nodo;
                    actual->siguiente->dato = d;
                    actual->siguiente->siguiente = aux;
                    this->dim = this->dim + 1;
                    break;

                }
                actual=actual->siguiente;
            }
        }
    }
    else {
        this->ultimo = new typename ListaOrdenada<Dato>::Nodo;
        this->primero = this->ultimo;
        this->primero->siguiente = nullptr;
        this->primero->dato = d;
        this->dim = 1;
    }
}

/*
* Pre: ---
* Post: Devuelve cierto si y solo si d pertenece a la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
template<typename Dato>
bool ListaOrdenada<Dato>::belongs(const Dato &d) {
    typename ListaOrdenada<Dato>::Nodo *aux = this->primero;
    for (int i = 0; i < this->dim; ++i) {
        if (aux->dato == d) {
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

/*
* Pre: ---
* Post: Devuelve ciero en el caso de que no exista ningún elemento en la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
template<typename Dato>
bool ListaOrdenada<Dato>::empty() {
    return this->dim == 0;
}

/*
* Pre: ---
* Post: Devuelve el numero de elementos de la lista
*/
template<typename Dato>
int ListaOrdenada<Dato>::size() {
    return this->dim;
}

/*
* Pre: ---
* Post: Pone el iterador de la lista al principio
*/
template<typename Dato>
void ListaOrdenada<Dato>::begin() {
    this->iterador = this->primero;
}

/*
* Pre: Se ha ejecutado previamente begin
* Post: Asigna el elemento apuntado por el iterador a d y avanza el iterador
*       Devuelve ciero solo si no existen más elementos por visitar en la lista
*/
template<typename Dato>
bool ListaOrdenada<Dato>::next(Dato &d) {
    if (this->iterador == nullptr) {
        return false;
    } else {
        d = this->iterador->dato;
        this->iterador = this->iterador->siguiente;
        return true;
    }
}

/*
* Pre: ---
* Post: Copia la lista en copied
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
template<typename Dato>
void ListaOrdenada<Dato>::copy(ListaOrdenada<Dato> &copied) {
    copied.clear();
    if (this->size() > 0) {
        typename ListaOrdenada<Dato>::Nodo *aux = this->primero;
        /*
      typename Lista<Dato>::Nodo *cima_nueva_pila = new typename Lista<Dato>::Nodo;
      cima_nueva_pila->dato=aux->dato;
      typename Lista<Dato>::Nodo *nueva_lista_aux=cima_nueva_pila;
      aux=aux->siguiente;*/
        copied.primero = new typename ListaOrdenada<Dato>::Nodo;
        copied.ultimo = copied.primero;
        copied.ultimo->dato = aux->dato;
        aux = aux->siguiente;
        while (aux != nullptr) {
            copied.ultimo->siguiente = new typename ListaOrdenada<Dato>::Nodo;
            copied.ultimo = copied.ultimo->siguiente;
            copied.ultimo->dato = aux->dato;
            aux = aux->siguiente;
        }
        copied.ultimo->siguiente = nullptr;
        copied.dim = this->size();
    }
}

#endif //LISTAORDENADA_H
