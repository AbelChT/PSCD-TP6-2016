//*****************************************************************
// File:   diccionarios.h
// Author: Adrian Garcia Saez-Benito (Nip: 722292)
//         Abel Chils Trabanco (Nip: 718997)
// Date:   25 de noviembre 2016
//*****************************************************************
/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como una implementacion de un árbol tipo
AVL desarrollado en la asignatura de EDA
******************************************************************/

//*****************************************************************
// Equivalencia entre las funciones implementadas en este tad y las funciones
// proporcionadas en la especificacion:
//
// Especificacion                   Implementacion
// crear                            crear
// añadir                           anyadir
// pertenece?                       pertenece
// obtenerValor                     obtenerValor
// quitar                           quitar
// cardenal                         cardenal
// esVacio?                         esVacio
// iniciarIterador                  iniciarIterador
// existeSiguiente?                 existeSiguiente
// siguienteOrden                   siguiente
// siguienteValor                   siguiente
// avanza                           siguiente
//*****************************************************************

#ifndef DICCIONARIOS_H
#define DICCIONARIOS_H


/***********************/
/***  PARTE PUBLICA  ***/
/***********************/

#include "../Pila/pila.h"
using namespace std;

/*
* Los valores del TAD representan conjuntos de pares (clave,valor) en los que no se
* permiten claves repetidas, y que cuentan con las operaciones de un iterador que
* permite recorrer los datos del diccionario segun el orden por clave, de menor a mayor.
*
* Para el funcionamiento de este modulo generico, es necesario que el tipo Clave con el
* que se invoque tenga sobrecargado el operador de comparacion: { < }
*/
template <typename Clave, typename Valor> struct Diccionario;

/*
* Pre:  ---
* Post: Devuelve un diccionario vacio, sin elementos (pares)
*/
template<typename Clave, typename Valor> void crear(Diccionario<Clave, Valor>& d);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Si en d no hay ningun par con clave c, devuelve en d un diccionario igual al resultante de añadir
*       el par (c,v) a d; si en d hay un par (c,v'), entonces devuelve en d un diccionario igual al
*       resultante de sustituir (c,v') por el par (c,v).
*/
template<typename Clave, typename Valor> void anyadir(Diccionario<Clave, Valor>& d, const Clave& c, const Valor& v);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve verdad si y solo si en d hay algun par (c,v).
*/
template<typename Clave, typename Valor> bool pertenece(const Diccionario<Clave, Valor>& d, const Clave& c);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve en v el valor asociado a la clave c en d y la función devuelve verdad.
*       Si c no esta en d, la funcion devuelve falso, y asigna a v un valor indeterminado.
*/
template<typename Clave, typename Valor> bool obtenerValor(const Diccionario<Clave, Valor>& d, const Clave& c, Valor& v);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Si c esta en d, devuelve un diccionario igual al resultante de borrar en d el par con clave c.
*       Si c no esta en d, devuelve un diccionario igual a d.
*/
template<typename Clave, typename Valor> void quitar(Diccionario<Clave, Valor>& d, const Clave& c);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve el numero de elementos (de pares) en el diccionario d.
*/
template<typename Clave, typename Valor> unsigned int cardinal(const Diccionario<Clave, Valor>& d);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve verdad si y solo si d no tiene elementos..
*/
template<typename Clave, typename Valor> bool esVacio(const Diccionario<Clave, Valor>& d);

/*
* Pre:  El diccionario ha de estar creado.
* Post: Prepara el iterador y su cursor para que el siguiente elemento (par) a visitar sea el primero
*       del diccionario d (situacion de no haber visitado ningun elemento).
*/
template<typename Clave, typename Valor> void iniciarIterador(Diccionario<Clave, Valor>& d);

/*
* Pre:  El diccionario ha de estar creado
* Post: Devuelve falso si ya se ha visitado el ultimo elemento, devuelve verdad en caso contrario.
*/
template<typename Clave, typename Valor> bool existeSiguiente(const Diccionario<Clave, Valor>& d);

/*
* Pre:  El diccionario ha de estar creado.
* Post: c y v toman el clave y valor del siguiente par, clave valor por visitar de d.
*       Prepara el iterador para visitar el siguiente par del diccionario d.
*       Devuelve verdad en caso de que la operacion se realize correctamente.
*/
template<typename Clave, typename Valor> bool siguiente(Diccionario<Clave, Valor>& d, Clave& c, Valor& v);


/***********************/
/***  PARTE PRIVADA  ***/
/***********************/
template<typename Clave, typename Valor> void rotacionIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo);
template<typename Clave, typename Valor> void rotacionIzqDer(typename Diccionario<Clave, Valor>::Nodo*& nodo);
template<typename Clave, typename Valor> void rotacionDer(typename Diccionario<Clave, Valor>::Nodo*& nodo);
template<typename Clave, typename Valor> void rotacionDerIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo);
template<typename Clave, typename Valor> bool pertenece_aux(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c);
template<typename Clave, typename Valor> void insertar_recursive(typename Diccionario<Clave, Valor>::Nodo*& nodo, const Clave& c, const Valor& v, bool& altura_modificada, bool& dim_modificado);
template<typename Clave, typename Valor> void equilIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& alturaModificada);
template<typename Clave, typename Valor> void equilDer(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& alturaModificada);
template<typename Clave, typename Valor> void borrarMaxClave(typename Diccionario<Clave, Valor>::Nodo*& nodo, Clave& c, Valor& v, bool& alturaModificada);
template<typename Clave, typename Valor> void borrarRec(typename Diccionario<Clave, Valor>::Nodo*& nodo, const Clave& c, bool& alturaModificada, bool& dim_modificado);
template<typename Clave, typename Valor> bool obtener_rec(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c, Valor& v);

template<typename Clave, typename Valor>
struct Diccionario {
    friend void crear<Clave, Valor>(Diccionario<Clave, Valor>& d);
    friend void anyadir<Clave, Valor>(Diccionario<Clave, Valor>& d, const Clave& c, const Valor& v);
    friend bool pertenece<Clave, Valor>(const Diccionario<Clave, Valor>& d, const Clave& c);
    friend bool obtenerValor<Clave, Valor>(const Diccionario<Clave, Valor>& d, const Clave& c, Valor& v);
    friend void quitar<Clave, Valor>(Diccionario<Clave, Valor>& d, const Clave& c);
    friend unsigned int cardinal<Clave, Valor>(const Diccionario<Clave, Valor>& d);
    friend bool esVacio<Clave, Valor>(const Diccionario<Clave, Valor>& d);
    friend void iniciarIterador<Clave, Valor>(Diccionario<Clave, Valor>& d);
    friend bool existeSiguiente<Clave, Valor>(const Diccionario<Clave, Valor>& d);
    friend bool siguiente<Clave, Valor>(Diccionario<Clave, Valor>& d, Clave& c, Valor& v);

private:
    struct Nodo {
        Nodo *izquierdo;
        Nodo *derecho;
        /*
        * -1 desequilibrado izquierda
        * 0 equilibrado
        * 1 desequilibrado derecha
        */
        int equilibrio;
        Clave clave;
        Valor valor;
    };

    friend void rotacionIzq<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo);
    friend void rotacionIzqDer<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo);
    friend void rotacionDer<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo);
    friend void rotacionDerIzq<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo);
    friend bool pertenece_aux<Clave, Valor>(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c);
    friend void insertar_recursive<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo, const Clave& c, const Valor& v, bool& altura_modificada, bool& dim_modificado);
    friend void equilIzq<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& alturaModificada);
    friend void equilDer<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& alturaModificada);
    friend void borrarMaxClave<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo, Clave& c, Valor& v, bool& alturaModificada);
    friend void borrarRec<Clave, Valor>(typename Diccionario<Clave, Valor>::Nodo*& nodo, const Clave& c, bool& alturaModificada, bool& dim_modificado);
    friend bool obtener_rec<Clave, Valor>(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c, Valor& v);

    Pila<Nodo*> iterador;
    Nodo *raiz;
    unsigned int size;
};

/*****************************************/
/***  IMPLEMENTACION DE LAS FUNCIONES  ***/
/*****************************************/

/*
* Pre:  ---
* Post: Devuelve un diccionario vacio, sin elementos (pares)
*/
template<typename Clave, typename Valor> void crear(Diccionario<Clave, Valor>& d) {
    d.size = 0;
    d.raiz = nullptr;
    crear(d.iterador);
}

/*
 * Auxiliares a anyadir y quitar
 */
template<typename Clave, typename Valor> void rotacionIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo) {
    typename Diccionario<Clave, Valor>::Nodo* aux;
    aux = nodo->izquierdo;
    nodo->izquierdo = aux->derecho;
    nodo->equilibrio = 0;
    aux->derecho = nodo;
    nodo = aux;
    nodo->equilibrio = 0;
}

template<typename Clave, typename Valor> void rotacionIzqDer(typename Diccionario<Clave, Valor>::Nodo*& nodo) {
    typename Diccionario<Clave, Valor>::Nodo* aux1;
    typename Diccionario<Clave, Valor>::Nodo* aux2;
    aux1 = nodo->izquierdo;
    aux2 = aux1->derecho;
    aux1->derecho = aux2->izquierdo;
    aux2->izquierdo = aux1;
    if (aux2->equilibrio == -1) {
        aux1->equilibrio = 0;
        nodo->equilibrio = 1;
    }
    else if (aux2->equilibrio == 0) {
        aux1->equilibrio = 0;
        nodo->equilibrio = 0;
    }
    else {
        aux1->equilibrio = -1;
        nodo->equilibrio = 0;
    }
    nodo->izquierdo = aux2->derecho;
    aux2->derecho = nodo;
    aux2->equilibrio = 0;
    nodo = aux2;
}

template<typename Clave, typename Valor> void rotacionDer(typename Diccionario<Clave, Valor>::Nodo*& nodo) {
    typename Diccionario<Clave, Valor>::Nodo* aux;
    aux = nodo->derecho;
    nodo->derecho = aux->izquierdo;
    nodo->equilibrio = 0;
    aux->izquierdo = nodo;
    nodo = aux;
    nodo->equilibrio = 0;
}

template<typename Clave, typename Valor> void rotacionDerIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo) {
    typename Diccionario<Clave, Valor>::Nodo* aux1;
    typename Diccionario<Clave, Valor>::Nodo* aux2;
    aux1 = nodo->derecho;
    aux2 = aux1->izquierdo;
    aux1->izquierdo = aux2->derecho;
    aux2->derecho = aux1;

    if (aux2->equilibrio == 1) {
        aux1->equilibrio = 0;
        nodo->equilibrio = -1;
    }
    else if (aux2->equilibrio == 0) {
        aux1->equilibrio = 0;
        nodo->equilibrio = 0;
    }
    else {
        aux1->equilibrio = 1;
        nodo->equilibrio = 0;
    }
    nodo->derecho = aux2->izquierdo;
    aux2->izquierdo = nodo;
    aux2->equilibrio = 0;
    nodo = aux2;
}

/*
 * Auxiliar a anyadir
 */
template<typename Clave, typename Valor> void insertar_recursive(typename Diccionario<Clave, Valor>::Nodo*& nodo,
    const Clave& c, const Valor& v, bool& altura_modificada, bool& dim_modificado) {
    if (nodo == nullptr) {
        nodo = new typename Diccionario<Clave, Valor>::Nodo;
        nodo->clave = c;
        nodo->valor = v;
        nodo->equilibrio = 0;
        nodo->derecho = nullptr;
        nodo->izquierdo = nullptr;
        altura_modificada = true;
    }
    else if (c < nodo->clave) {
        insertar_recursive<Clave, Valor>(nodo->izquierdo, c, v, altura_modificada, dim_modificado);
        if (altura_modificada) {
            switch (nodo->equilibrio) {
            case -1://des izq
                if (nodo->izquierdo->equilibrio == -1) {
                    rotacionIzq<Clave, Valor>(nodo);
                }
                else {
                    rotacionIzqDer<Clave, Valor>(nodo);
                }
                altura_modificada = false;
                break;
            case 0://equ
                nodo->equilibrio = -1;
                break;
            default://des derecha
                nodo->equilibrio = 0;
                altura_modificada = false;
            }
        }
    }
    else if (nodo->clave < c) {
        insertar_recursive<Clave, Valor>(nodo->derecho, c, v, altura_modificada, dim_modificado);
        if (altura_modificada) {
            switch (nodo->equilibrio) {
            case 1://des izq
                if (nodo->derecho->equilibrio == 1) {
                    rotacionDer<Clave, Valor>(nodo);
                }
                else {
                    rotacionDerIzq<Clave, Valor>(nodo);
                }
                altura_modificada = false;
                break;
            case 0://equ
                nodo->equilibrio = 1;
                break;
            default://des derecha
                nodo->equilibrio = 0;
                altura_modificada = false;
            }
        }
    }
    else {//c==nodo->clave
        nodo->valor = v;
        altura_modificada = false;
        dim_modificado = false;
    }
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Si en d no hay ningun par con clave c, devuelve en d un diccionario igual al resultante de añadir
*       el par (c,v) a d; si en d hay un par (c,v'), entonces devuelve en d un diccionario igual al
*       resultante de sustituir (c,v') por el par (c,v).
*/
template<typename Clave, typename Valor> void anyadir(Diccionario<Clave, Valor>& d, const Clave& c, const Valor& v) {
    bool altura_modificada = false;
    bool dim_modificado = true;
    insertar_recursive<Clave, Valor>(d.raiz, c, v, altura_modificada, dim_modificado);
    if (dim_modificado) {
        ++d.size;
    }
}

/*
 * Auxiliar a pertenece
 */
template<typename Clave, typename Valor> bool pertenece_aux(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c) {
    if (nodo == nullptr) return false;
    else if (c < nodo->clave) return pertenece_aux<Clave, Valor>(nodo->izquierdo, c);
    else if (nodo->clave < c) return pertenece_aux<Clave, Valor>(nodo->derecho, c);
    else return true;
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve verdad si y solo si en d hay algun par (c,v).
*/
template<typename Clave, typename Valor> bool pertenece(const Diccionario<Clave, Valor>& d, const Clave& c) {
    return pertenece_aux<Clave, Valor>(d.raiz, c);
}

/*
* Auxiliar a obtenerValor.
*/
template<typename Clave, typename Valor> bool obtener_rec(const typename Diccionario<Clave, Valor>::Nodo* nodo, const Clave& c, Valor& v) {
    if (nodo == nullptr) return false;
    else if (c < nodo->clave) return obtener_rec(nodo->izquierdo, c, v);
    else if (nodo->clave < c) return obtener_rec(nodo->derecho, c, v);
    else {
        v = nodo->valor;
        return true;
    }
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve en v el valor asociado a la clave c en d y la función devuelve verdad.
*       Si c no esta en d, la funcion devuelve falso, y asigna a v un valor indeterminado.
*/
template<typename Clave, typename Valor> bool obtenerValor(const Diccionario<Clave, Valor>& d, const Clave& c, Valor& v) {
    return obtener_rec(d.raiz, c, v);
}

/*
 * Auxiliares a quitar
 */
template<typename Clave, typename Valor> void equilIzq(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& altura_modificada) {
    switch (nodo->equilibrio) {
    case -1://pesado izquierda
        nodo->equilibrio = 0;
        break;
    case 0://equilibrado
        nodo->equilibrio = 1;
        altura_modificada = false;
        break;
    default://pesado derecha
        typename Diccionario<Clave, Valor>::Nodo* aux = nodo->derecho;
        int equilibrio = aux->equilibrio;
        if (equilibrio != -1) {
            nodo->derecho = aux->izquierdo;
            aux->izquierdo = nodo;
            if (equilibrio == 0) {
                nodo->equilibrio = 1;
                aux->equilibrio = -1;
                altura_modificada = false;
            }
            else {
                nodo->equilibrio = 0;
                aux->equilibrio = 0;
            }
            nodo = aux;
        }
        else {
            rotacionDerIzq<Clave, Valor>(nodo);
        }
    }
}

template<typename Clave, typename Valor> void equilDer(typename Diccionario<Clave, Valor>::Nodo*& nodo, bool& altura_modificada) {
    switch (nodo->equilibrio) {
    case 1://pesado derecha
        nodo->equilibrio = 0;
        break;
    case 0://equilibrado
        nodo->equilibrio = -1;
        altura_modificada = false;
        break;
    default://pesado izquierda
        typename Diccionario<Clave, Valor>::Nodo* a_izq = nodo->izquierdo;
        int equilibrio = a_izq->equilibrio;
        if (equilibrio != 1) {
            nodo->izquierdo = a_izq->derecho;
            a_izq->derecho = nodo;
            if (equilibrio == 0) {
                nodo->equilibrio = -1;
                a_izq->equilibrio = 1;
                altura_modificada = false;
            }
            else {
                nodo->equilibrio = 0;
                a_izq->equilibrio = 0;
            }
            nodo = a_izq;
        }
        else {
            rotacionIzqDer<Clave, Valor>(nodo);
        }
    }
}

template<typename Clave, typename Valor> void borrarMaxClave(typename Diccionario<Clave, Valor>::Nodo*& nodo, Clave& c, Valor& v,
    bool& alturaModificada) {
    typename Diccionario<Clave, Valor>::Nodo* aux = new typename Diccionario<Clave, Valor>::Nodo;

    if (nodo->derecho == nullptr) {
        c = nodo->clave;
        v = nodo->valor;
        aux = nodo;
        nodo = nodo->izquierdo;
        delete aux;
        aux = nullptr;
        alturaModificada = true;
    }
    else {
        borrarMaxClave(nodo->derecho, c, v, alturaModificada);
        if (alturaModificada) {
            equilDer<Clave, Valor>(nodo, alturaModificada);
        }
    }
}

template<typename Clave, typename Valor> void borrarRec(typename Diccionario<Clave, Valor>::Nodo*& nodo, const Clave& c, bool& alturaModificada, bool& dim_modificado) {
    typename Diccionario<Clave, Valor>::Nodo* aux;
    if (nodo != nullptr) {
        if (c < nodo->clave) {
            borrarRec<Clave, Valor>(nodo->izquierdo, c, alturaModificada, dim_modificado);
            if (alturaModificada)
                equilIzq<Clave, Valor>(nodo, alturaModificada);
        }
        else if (nodo->clave < c) {
            borrarRec<Clave, Valor>(nodo->derecho, c, alturaModificada, dim_modificado);
            if (alturaModificada)
                equilDer<Clave, Valor>(nodo, alturaModificada);
        }
        else {
            if (nodo->izquierdo == nullptr) {
                aux = nodo;
                nodo = nodo->derecho;
                delete aux;
                aux = nullptr;
                alturaModificada = true;
            }
            else if (nodo->derecho == nullptr) {
                aux = nodo;
                nodo = nodo->izquierdo;
                delete aux;
                aux = nullptr;
                alturaModificada = true;
            }
            else {
                borrarMaxClave(nodo->izquierdo, nodo->clave, nodo->valor, alturaModificada);
                if (alturaModificada)
                    equilIzq<Clave, Valor>(nodo, alturaModificada);
            }
        }
    }
    else
    {
        dim_modificado = false;
    }
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Si c esta en d, devuelve un diccionario igual al resultante de borrar en d el par con clave c.
*       Si c no esta en d, devuelve un diccionario igual a d.
*/
template<typename Clave, typename Valor> void quitar(Diccionario<Clave, Valor>& d, const Clave& c) {
    bool alturaModificada = false;
    bool dim_modificado = true;
    borrarRec<Clave, Valor>(d.raiz, c, alturaModificada, dim_modificado);
    if (dim_modificado) {
        --d.size;
    }
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve el numero de elementos (de pares) en el diccionario d.
*/
template<typename Clave, typename Valor> unsigned int cardinal(const Diccionario<Clave, Valor>& d) {
    return d.size;
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Devuelve verdad si y solo si d no tiene elementos..
*/
template<typename Clave, typename Valor> bool esVacio(const Diccionario<Clave, Valor>& d) {
    return cardinal(d) == 0;
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: Prepara el iterador y su cursor para que el siguiente elemento (par) a visitar sea el primero
*       del diccionario d (situacion de no haber visitado ningun elemento).
*/
template<typename Clave, typename Valor> void iniciarIterador(Diccionario<Clave, Valor>& d) {//inicializar la pila
    typename Diccionario<Clave, Valor>::Nodo* aux = new typename Diccionario<Clave, Valor>::Nodo;
    if (!esVacio(d)) {
        liberar(d.iterador);
        aux = d.raiz;
        while (aux != nullptr) {
            apilar(d.iterador, aux);
            aux = aux->izquierdo;
        }
    }
}

/*
* Pre:  El diccionario ha de estar creado
* Post: Devuelve falso si ya se ha visitado el ultimo elemento, devuelve verdad en caso contrario.
*/
template<typename Clave, typename Valor> bool existeSiguiente(const Diccionario<Clave, Valor>& d) {
    return !esVacia(d.iterador);
}

/*
* Pre:  El diccionario ha de estar creado.
* Post: c y v toman el clave y valor del siguiente par, clave valor por visitar de d.
*       Prepara el iterador para visitar el siguiente par del diccionario d.
*       Devuelve verdad en caso de que la operacion se realize correctamente.
*/
template<typename Clave, typename Valor> bool siguiente(Diccionario<Clave, Valor>& d, Clave& c, Valor& v) {
    typename Diccionario<Clave, Valor>::Nodo* aux = new typename Diccionario<Clave, Valor>::Nodo;
    if (existeSiguiente(d)) {
        desapilar(d.iterador, aux);
        c = aux->clave;
        v = aux->valor;
        aux = aux->derecho;
        while (aux != nullptr) {
            apilar(d.iterador, aux);
            aux = aux->izquierdo;
        }
        return true;
    }
    else return false;
}

#endif//DICCIONARIOS_H
