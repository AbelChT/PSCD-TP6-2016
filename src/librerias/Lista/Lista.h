//
// Created by Abel on 27/12/2016.
//

#ifndef PRUEBASSTRUCTS_LISTA_H
#define PRUEBASSTRUCTS_LISTA_H

/*
* Los valores del TAD pila representan secuencias de elementos con
* acceso LIFO (last in, first out), esto es, el último elemento añadido
* será el primero en ser borrado.
*/
template<typename Dato>
class Lista {
public:
    /*
* Devuelve en pila la pila vacía, sin elementos
*/
    Lista();

    /*
* Devuelve en pila la pila vacía y además libera la memoria utilizada previamente por pila
*/
    ~Lista();

    /*
* Devuelve en pila la pila vacía y además libera la memoria utilizada previamente por pila
*/
    void vaciar();

    /*
* Devuelve en pila la pila resultante de añadir d a pila
*/
    void anyadir(const Dato& d);

    /*
* Devuelve en pila la pila resultante de añadir d a pila
*/
     bool pertenece(const Dato& d);

    /*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
    bool esVacia();

    /*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
    int tamanyo();
    /*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
    void iniciarIterador();
    /*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
    bool siguienteElemento(Dato& pila);

private:
    struct Nodo{
        Dato dato;
        Nodo* anterior;
    };
    Nodo* cima;
    Nodo* iterador;
    unsigned int size;

};


template<typename Dato>
Lista<Dato>::Lista() {
    this->cima = nullptr;
    this->size = 0;
    this->iterador = this->cima;
}

/*
* Devuelve en pila la pila vacía y además libera la memoria utilizada previamente por pila
*/
template<typename Dato>
Lista<Dato>::~Lista() {
    while (this->size > 0) {
        typename Lista<Dato>::Nodo *aux = this->cima->anterior;
        delete this->cima;
        this->cima = aux;
        aux = nullptr;
        --this->size;
    }
}

/*
* Devuelve en pila la pila vacía y además libera la memoria utilizada previamente por pila
*/
template<typename Dato>
void Lista<Dato>::vaciar() {
    while (this->size > 0) {
        typename Lista<Dato>::Nodo *aux = this->cima->anterior;
        delete this->cima;
        this->cima = aux;
        aux = nullptr;
        --this->size;
    }
}

/*
* Devuelve en pila la pila resultante de añadir d a pila
*/
template<typename Dato>
void Lista<Dato>::anyadir(const Dato &d) {

    typename Lista<Dato>::Nodo *aux = new (typename Lista<Dato>::Nodo);
    aux->anterior = this->cima;
    aux->dato = d;
    this->cima = aux;
    ++this->size;
}

/*
* Devuelve en pila la pila resultante de añadir d a pila
*/
template<typename Dato>
bool Lista<Dato>::pertenece(const Dato &d) {
    typename Lista<Dato>::Nodo *aux = this->cima;
    for (int i = 0; i < this->size; ++i) {
        if (aux->dato == d) {
            return true;
        }
        aux = aux->anterior;
    }
    return false;
}

/*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
template<typename Dato>
bool Lista<Dato>::esVacia() {
    return this->size == 0;
}

/*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
template<typename Dato>
int Lista<Dato>::tamanyo() {
    return this->size;
}

/*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
template<typename Dato>
void Lista<Dato>::iniciarIterador() {
    this->iterador = this->cima;
}

/*
* Devuelve true en el caso de que no exista ningún elemento en pila
*/
template<typename Dato>
bool Lista<Dato>::siguienteElemento(Dato &d) {
    if (this->iterador == nullptr) {
        return false;
    } else {
        d = this->iterador->dato;
        this->iterador = this->iterador->anterior;
        return true;
    }
}

#endif //PRUEBASSTRUCTS_LISTA_H
