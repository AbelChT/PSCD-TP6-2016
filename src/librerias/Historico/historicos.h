//*****************************************************************
// File:   historicos.h
// Author: Adrián García Sáez-Benito (Nip: 722292)
//         Abel Chils Trabanco (Nip: 718997)
// Date:   25 de noviembre 2016
//*****************************************************************

//*****************************************************************
// Equivalencia entre las funciones implementadas en este tad y las funciones
// proporcionadas en la especificación:
//
// Especificación                   Implementación
// crear                            crear
// introducir                       introducir
// máximoValor                      maximoValor
// ordenDeMayorValor                maximoValor
// ordenDeÚltimo                    ultimo
// valorDeÚltimo                    ultimo
// tamaño                           tamanyo
// esVacío?                         esVacio
// listar                           listar
// iniciarIterador                  iniciarIterador
// existeSiguiente?                 existeSiguiente
// siguienteOrden                   siguiente
// siguienteValor                   siguiente
// avanza                           siguiente
//*****************************************************************


#ifndef HISTORICOS_H
#define HISTORICOS_H

/***********************/
/***  PARTE PÚBLICA  ***/
/***********************/

#include<string>

using namespace std;
/*
* Los valores del TAD representan conjuntos de pares (orden,valor), en los que
* un par solo puede introducirse en el histórico si el orden del par es mayor
* o igual que el orden de los demás pares ya registrados en el histórico.
* El TAD cuenta con operaciones para consultar los datos del par con máximo
* valor de todos los registrados en el histórico, y operaciones para consultar
* los datos del último par registrado en el histórico.
*
* Para el funcionamiento de este módulo genérico, es necesario que los tipos
* Valor y Orden con los que se invoque tengan sobrecargados los operadores de
* comparación:{ < > <= >= == != }
* Como también poseer una función to_string, la cual como parámetro de entrada
* tendrá un dato del tipo en cuestión y devolverá un dato de tipo string.
*/

template<typename Orden, typename Valor> struct Historico;

/*
* Pre:  En el caso de haberse invocado anteriormente a la función crear sobre h, ha de invocarse
*       la funcion destruir sobre h.
* Post: Crea en h un historico vacío, sin pares.
*/
template<typename Orden, typename Valor> void crear(Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado
* Post: Destruye el historico h.
*/
template<typename Orden, typename Valor> void destruir(Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado. El tamaño del historico ha de ser menor al máximo valor que
*       puede tomar un entero sin signo.
* Post: Si el último par introducido en h es (o’,v’) y o es mayor o igual que o’,
*       entonces h toma el valor del histórico resultante de añadir el par (o,v) al
*       h introducido como último elemento del histórico.
*/
template<typename Orden, typename Valor> void introducir(Historico<Orden, Valor>& h, const Orden& o, const Valor& v);

/*
* Pre:  El historico h ha de estar creado y no puede ser vacio (esVacio(h)==false)
* Post: v=max(valor|(orden,valor) ∈ h), o=max(orden|(orden,v) ∈ h)
*       Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor> bool maximoValor(const Historico<Orden, Valor>& h, Orden& o, Valor& v);

/*
* Pre:  El historico h ha de estar creado y no puede ser vacio (esVacio(h)==false)
* Post: o y v tomarán el orden y valor del último par orden, valor  introducido en h.
* Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor> bool ultimo(const Historico<Orden, Valor>& h, Orden& o, Valor& v);

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve el nº de pares en el historico h.
*/
template<typename Orden, typename Valor>unsigned int tamanyo(const Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve true en caso de que h no contenga ningun par, en caso contrario
*       devuelve false
*/
template<typename Orden, typename Valor>bool esVacio(const Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado
* Post: listado toma el valor de la representación de todos los pares (o,v)
*       de h, en orden desde el primero al último introducido, y de tal forma que: los
*       pares estén separados entre sí con el carácter de salto de línea; y para
*       cada par, el orden y el valor estén separados por un carácter ‘;’.
*/
template<typename Orden, typename Valor>void listar(Historico<Orden, Valor>& h, string& listado);

/*
* Pre:  El historico h ha de estar creado
* Post: Inicializa el iterador para recorrer los pares del histórico h, de forma
*       que el siguiente par a visitar sea el primero (situación de no haber
*       visitado ningún par)
*/
template<typename Orden, typename Valor>void iniciarIterador(Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve true si y solo si queda algún par por visitar con el iterador del historico h.
*/
template<typename Orden, typename Valor>bool existeSiguiente(const Historico<Orden, Valor>& h);

/*
* Pre:  El historico h ha de estar creado y existeSiguiente(h)==true
* Post: o y v toman el orden y valor del siguiente par, orden valor por visitar de h.
*       Prepara el iterador para visitar el siguiente par del histórico h.
*       Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor>bool siguiente(Historico<Orden, Valor>& h, Orden& o, Valor& v);


/***********************/
/***  PARTE PRIVADA  ***/
/***********************/

template<typename Orden, typename Valor>
struct Historico {
    friend void crear<Orden, Valor>(Historico<Orden, Valor>& h);
    friend void destruir<Orden, Valor>(Historico<Orden, Valor>& h);
    friend void introducir<Orden, Valor>(Historico<Orden, Valor>& h, const Orden& o, const Valor& v);
    friend bool maximoValor<Orden, Valor>(const Historico<Orden, Valor>& h, Orden& o, Valor& v);
    friend bool ultimo<Orden, Valor>(const Historico<Orden, Valor>& h, Orden& o, Valor& v);
    friend unsigned int tamanyo<Orden, Valor>(const Historico<Orden, Valor>& h);
    friend bool esVacio<Orden, Valor>(const Historico<Orden, Valor>& h);
    friend void listar<Orden, Valor>(Historico<Orden, Valor>& h, string& c);
    friend void iniciarIterador<Orden, Valor>(Historico<Orden, Valor>& h);
    friend bool existeSiguiente<Orden, Valor>(const Historico<Orden, Valor>& h);
    friend bool siguiente<Orden, Valor>(Historico<Orden, Valor>& h, Orden& o, Valor& v);


private:
    struct Nodo {
        Nodo *siguiente;
        Orden orden;
        Valor valor;
    };
    Nodo *primer_elemento;
    Nodo *ultimo_elemento;
    Nodo *mayor_valor;
    Nodo *iterador;
    unsigned int size;
};


/*****************************************/
/***  IMPLEMENTACIÓN DE LAS FUNCIONES  ***/
/*****************************************/

/*
* Pre:  En el caso de haberse invocado anteriormente a la función crear sobre h, ha de invocarse
*       la funcion destruir sobre h.
* Post: Crea en h un historico vacío, sin pares.
*/
template<typename Orden, typename Valor> void crear(Historico<Orden, Valor>& h) {
    h.primer_elemento = nullptr;
    h.ultimo_elemento = nullptr;
    h.mayor_valor = nullptr;
    h.size = 0;
    h.iterador = nullptr;
}

/*
* Pre:  El historico h ha de estar creado
* Post: Destruye el historico h.
*/
template<typename Orden, typename Valor> void destruir(Historico<Orden, Valor>& h) {
    if (!esVacio(h)) {
        typename Historico<Orden, Valor>::Nodo* actual;
        typename Historico<Orden, Valor>::Nodo* siguiente;
        actual = h.primer_elemento;
        siguiente = actual->siguiente;
        delete actual;

        while (siguiente != nullptr) {
            actual = siguiente;
            siguiente = actual->siguiente;
            delete actual;
        }
        h.primer_elemento = nullptr;
        h.ultimo_elemento = nullptr;
        h.mayor_valor = nullptr;
        h.iterador = nullptr;
        h.size = 0;
    }
}

/*
* Pre:  El historico h ha de estar creado. El tamaño del historico ha de ser menor al máximo valor que
*       puede tomar un entero sin signo.
* Post: Si el último par introducido en h es (o’,v’) y o es mayor o igual que o’,
*       entonces h toma el valor del histórico resultante de añadir el par (o,v) al
*       h introducido como último elemento del histórico.
*/
template<typename Orden, typename Valor> void introducir(Historico<Orden, Valor>& h, const Orden& o, const Valor& v) {

    if (esVacio(h) || (h.ultimo_elemento->orden <= o))
    {
        typename Historico<Orden, Valor>::Nodo* nuevo = new typename Historico<Orden, Valor>::Nodo;
        nuevo->siguiente = nullptr;
        nuevo->orden = o;
        nuevo->valor = v;

        if (esVacio(h)) {
            h.mayor_valor = nuevo;
            h.iterador = nuevo;
            h.primer_elemento = nuevo;
        }
        else {
            h.ultimo_elemento->siguiente = nuevo;
            if (nuevo->valor >= h.mayor_valor->valor) {
                h.mayor_valor = nuevo;
            }
        }
        h.ultimo_elemento = nuevo;
        ++h.size;
    }
}

/*
* Pre:  El historico h ha de estar creado y no puede ser vacio (esVacio(h)==false)
* Post: v=max(valor|(orden,valor) ∈ h), o=max(orden|(orden,v) ∈ h)
* Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor> bool maximoValor(const Historico<Orden, Valor>& h, Orden& o, Valor& v) {
    if (!esVacio(h)) {
        v = h.mayor_valor->valor;
        o = h.mayor_valor->orden;
        return true;
    }
    else
    {
        return false;
    }
}

/*
* Pre:  El historico h ha de estar creado y no puede ser vacio (esVacio(h)==false)
* Post: o y v tomarán el orden y valor del último par orden, valor  introducido en h.
* Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor> bool ultimo(const Historico<Orden, Valor>& h, Orden& o, Valor& v) {
    if (!esVacio(h)) {
        o = h.ultimo_elemento->orden;
        v = h.ultimo_elemento->valor;
        return true;
    }
    else {
        return false;
    }
}

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve el nº de pares en el historico h.
*/
template<typename Orden, typename Valor>unsigned int tamanyo(const Historico<Orden, Valor>& h) {
    return h.size;
}

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve true en casi de que h no contenga ningun par, en caso contrario
*       devuelve false
*/
template<typename Orden, typename Valor>bool esVacio(const Historico<Orden, Valor>& h) {
    return tamanyo(h) == 0;
}

/*
* Pre:  El historico h ha de estar creado
* Post: listado toma el valor de la representación de todos los pares (o,v)
*       de h, en orden desde el primero al último introducido, y de tal forma que: los
*       pares estén separados entre sí con el carácter de salto de línea; y para
*       cada par, el orden y el valor estén separados por un carácter ‘;’.
*/
template<typename Orden, typename Valor>void listar(Historico<Orden, Valor>& h, string& c) {
    typename Historico<Orden, Valor>::Nodo* savediterador = h.iterador;//guardo el iterador actual
    Orden o;
    Valor v;
    c.clear();
    iniciarIterador(h);
    while (existeSiguiente(h)) {
        siguiente(h, o, v);
        c = c + to_string(o) + ";" + to_string(v) + "\n";
    }
    h.iterador = savediterador;//restauro el iterador
}

/*
* Pre:  El historico h ha de estar creado
* Post: Inicializa el iterador para recorrer los pares del histórico h, de forma
*       que el siguiente par a visitar sea el primero (situación de no haber
*       visitado ningún par)
*/

template<typename Orden, typename Valor>void iniciarIterador(Historico<Orden, Valor>& h) {
    if (!esVacio(h))
        h.iterador = h.primer_elemento;
}

/*
* Pre:  El historico h ha de estar creado
* Post: Devuelve true si y solo si queda algún par por visitar con el iterador del historico h.
*/

template<typename Orden, typename Valor>bool existeSiguiente(const Historico<Orden, Valor>& h) {
    return h.iterador != nullptr;
}

/*
* Pre:  El historico h ha de estar creado y existeSiguiente(h)==true
* Post: o y v toman el orden y valor del siguiente par, orden valor por visitar de h.
*       Prepara el iterador para visitar el siguiente par del histórico h.
* Devuelve true en caso de que la operación se realize correctamente
*/
template<typename Orden, typename Valor> bool siguiente(Historico<Orden, Valor>& h, Orden& o, Valor& v) {
    if (existeSiguiente(h)) {
        o = h.iterador->orden;
        v = h.iterador->valor;
        h.iterador = h.iterador->siguiente;
        return true;
    }
    else
    {
        return false;
    }
}

#endif
