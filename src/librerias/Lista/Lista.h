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
        Nodo* anterior;
    };
    Nodo* cima;
    Nodo* iterador;
    unsigned int dim;

};


/*
* Pre: ---
* Post: Crea una lista vacía, sin elementos
*/
template<typename Dato>
Lista<Dato>::Lista() {
    this->cima = nullptr;
    this->dim = 0;
    this->iterador = this->cima;
}

/*
* Pre: ---
* Post: Libera la memoria usada por la lista
*/
template<typename Dato>
Lista<Dato>::~Lista() {
    while (this->dim > 0) {
        typename Lista<Dato>::Nodo *aux = this->cima->anterior;
        delete this->cima;
        this->cima = aux;
        aux = nullptr;
        --this->dim;
    }
}

/*
* Pre: ---
* Post: Devuelve una lista vacia y libera la memoria usada por la lista anterior
*/
template<typename Dato>
void Lista<Dato>::clear() {
    while (this->dim > 0) {
        typename Lista<Dato>::Nodo *aux = this->cima->anterior;
        delete this->cima;
        this->cima = aux;
        aux = nullptr;
        --this->dim;
    }
}

/*
* Pre: ---
* Post: Devuelve  la lista resultante de añadir d a lista
*/
template<typename Dato>
void Lista<Dato>::add(const Dato &d) {

    typename Lista<Dato>::Nodo *aux = new (typename Lista<Dato>::Nodo);
    aux->anterior = this->cima;
    aux->dato = d;
    this->cima = aux;
    ++this->dim;
}

/*
* Pre: ---
* Post: Devuelve cierto si y solo si d pertenece a la lista
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
template<typename Dato>
bool Lista<Dato>::belongs(const Dato &d) {
    typename Lista<Dato>::Nodo *aux = this->cima;
    for (int i = 0; i < this->dim; ++i) {
        if (aux->dato == d) {
            return true;
        }
        aux = aux->anterior;
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
    this->iterador = this->cima;
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
        this->iterador = this->iterador->anterior;
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
  if(this.size>0){
    typename Lista<Dato>::Nodo *aux = this->cima;
    typename Lista<Dato>::Nodo *cima_nueva_pila = new typename Lista<Dato>::Nodo;
    cima_nueva_pila->dato=aux->dato;
    typename Lista<Dato>::Nodo *nueva_lista_aux=cima_nueva_pila;
    aux=aux->anterior;
    while (aux!=nullptr) {
      nueva_lista_aux->anterior = new typename Lista<Dato>::Nodo;
      nueva_lista_aux=nueva_lista_aux->anterior;
      nueva_lista_aux->dato=aux->dato;
      aux=aux->anterior;
    }
    copied.cima=cima_nueva_pila;
    copied.dim=this.size();
  }
}

#endif //PRUEBASSTRUCTS_LISTA_H
