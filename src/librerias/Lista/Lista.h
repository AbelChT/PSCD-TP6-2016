/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef LISTA_H
#define LISTA_H

/*
* Los valores del TAD lista representan una secuencias de elementos
*/

template<typename Dato> class Lista {
public:
/*
* Pre: ---
* Post: Crea una lista vacía, sin elementos
*/
    Lista();

/*
* Pre: ---
* Post: Libera la memoria usada por la lista
*/
    ~Lista();

/*
* Pre: ---
* Post: Devuelve una lista vacia y libera la memoria usada por la lista anterior
*/
    void clear();

/*
* Pre: ---
* Post: Devuelve  la lista resultante de añadir d a lista
*/
    void add(const Dato& d);

/*
* Pre: ---
* Post: Devuelve cierto si y solo si d pertenece a la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
     bool belongs(const Dato& d);

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
*       Devuelve ciero solo si no existen más elementos por visitar en la lista
*/
    bool next(Dato& d);

/*
* Pre: ---
* Post: Copia la lista en copied
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    void copy(Lista<Dato>& copied);

private:
    struct Nodo{
        Dato dato;
        Nodo* siguiente;
    };
    Nodo* primero;
    Nodo* ultimo;
    Nodo* iterador;
    unsigned int dim;

};


/*
* Pre: ---
* Post: Crea una lista vacía, sin elementos
*/
template<typename Dato>
Lista<Dato>::Lista() {
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
Lista<Dato>::~Lista() {
    while (this->dim > 0) {
        typename Lista<Dato>::Nodo *aux = this->primero->siguiente;
        delete this->primero;
        this->primero = aux;
        aux = nullptr;
        this->dim=this->dim-1;
    }
}

/*
* Pre: ---
* Post: Devuelve una lista vacia y libera la memoria usada por la lista anterior
*/
template<typename Dato>
void Lista<Dato>::clear() {
    while (this->dim > 0) {
        typename Lista<Dato>::Nodo *aux = this->primero->siguiente;
        delete this->primero;
        this->primero = aux;
        aux = nullptr;
        this->dim=this->dim-1;
    }
    this->ultimo = nullptr;
    this->iterador = nullptr;
}

/*
* Pre: ---
* Post: Devuelve  la lista resultante de añadir d a lista
*/
template<typename Dato>
void Lista<Dato>::add(const Dato &d) {
    if(this->size()>0){
        this->ultimo->siguiente = new typename Lista<Dato>::Nodo;
        this->ultimo=this->ultimo->siguiente;
        this->ultimo->dato=d;
        this->ultimo->siguiente= nullptr;
        this->dim=this->dim+1;
    }else{
        this->ultimo=new typename Lista<Dato>::Nodo;
        this->primero=this->ultimo;
        this->primero->siguiente= nullptr;
        this->primero->dato=d;
        this->dim=1;
    }
}

/*
* Pre: ---
* Post: Devuelve cierto si y solo si d pertenece a la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
template<typename Dato>
bool Lista<Dato>::belongs(const Dato &d) {
    typename Lista<Dato>::Nodo *aux = this->primero;
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
bool Lista<Dato>::empty() {
    return this->dim == 0;
}

/*
* Pre: ---
* Post: Devuelve el numero de elementos de la lista
*/
template<typename Dato>
int Lista<Dato>::size() {
    return this->dim;
}

/*
* Pre: ---
* Post: Pone el iterador de la lista al principio
*/
template<typename Dato>
void Lista<Dato>::begin() {
    this->iterador = this->primero;
}

/*
* Pre: Se ha ejecutado previamente begin
* Post: Asigna el elemento apuntado por el iterador a d y avanza el iterador
*       Devuelve ciero solo si no existen más elementos por visitar en la lista
*/
template<typename Dato>
bool Lista<Dato>::next(Dato &d) {
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
void Lista<Dato>::copy(Lista<Dato>& copied){
  copied.clear();
  if(this->size()>0){
    typename Lista<Dato>::Nodo *aux = this->primero;
      /*
    typename Lista<Dato>::Nodo *cima_nueva_pila = new typename Lista<Dato>::Nodo;
    cima_nueva_pila->dato=aux->dato;
    typename Lista<Dato>::Nodo *nueva_lista_aux=cima_nueva_pila;
    aux=aux->siguiente;*/
      copied.primero=new typename Lista<Dato>::Nodo;
      copied.ultimo=copied.primero;
      copied.ultimo->dato=aux->dato;
      aux=aux->siguiente;
    while (aux!=nullptr) {
        copied.ultimo->siguiente = new typename Lista<Dato>::Nodo;
        copied.ultimo=copied.ultimo->siguiente;
        copied.ultimo->dato=aux->dato;
        aux=aux->siguiente;
    }
    copied.ultimo->siguiente= nullptr;
    copied.dim=this->size();
  }
}

#endif //PRUEBASSTRUCTS_LISTA_H
