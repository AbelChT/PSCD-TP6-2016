/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como un monitor
*       por lo que las operaciones buscarMonumento y
*       buscar restaurante se pueden ejecutar en multiples
*       threads de una forma segura
******************************************************************/

#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <string>
#include "../Lista/ListaOrdenada.h" //libreria lista

using namespace std;

/*
* Caracteres reconocidos
* El resto de caracteres se representan como 0
* letras del abecedario inglés (('z'-'a')+1) + numeros (10)
+ espacio en blanco (1)
*/
#define DIM_NUMERO_CARACTERES_RECONOCIDOS (('z'-'a')+12)

/*
* El TAD SuffixTree representa un arbol de sufijos.
* Es un tipo de dato compuesto por una clave y un valor. La clave es siempre
* un dato de tipo string.
* Este tipo de dato esta optimizado para realizar busquedas en él.
*/
template<typename Dato>
class SuffixTree {
public:

/*
* Pre: ---
* Post: Crea un arbol de sufijos
*/
    SuffixTree();

/*
* Pre: ---
* Post: Agrega el dato "valor", asociado a la clave "clave" al arbol de sufijos
*/
    void add(const string clave, const Dato &valor);

/*
* Pre: ---
* Post: Elimina los datos contenidos en "resultados" y almacena en la variable
*       resultado los valores en cuyas claves
*       estuviese contenido el substring sub_clave
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    void search(const string sub_clave, ListaOrdenada<Dato> &resultados);

private:

    /*
    * Añade el par clave y valor al arbol de sufijos, tratando a este como si fuese
    * un trie
    */
    unsigned int obtposletr(char letra);

    void addTrie(const string clave, const Dato &valor);

    struct Nodo {
        Nodo *letra[DIM_NUMERO_CARACTERES_RECONOCIDOS];
        ListaOrdenada<Dato> valores;//Datos asociados al nodo en cuestion
    };

    //Cima del arbol de sufijos
    Nodo cima;

};

/*
* Transforma el caracter letra a un caracter que se pueda guardar en el trie
*/
template<typename Dato>
unsigned int SuffixTree<Dato>::obtposletr(char letra) {
    if (letra <= 'z' && letra >= 'a') {//letras minúsculas
        return (unsigned int) (letra - 'a');
    } else if (letra <= 'Z' && letra >= 'A') {//letra mayuscula
        return (unsigned int) (letra - 'A');
    } else if (letra >= '0' && letra <= '9') {
        return letra - '0' + 'z' - 'a' + 2;
    } else {// Todo caracter que no sea una letra del alfabeto ingles o un numero lo
        //trato como un caracter "raro"/espacio en blanco
        return 'z' - 'a' + 1;
    }
}

/*
* Pre: ---
* Post: Agrega el dato "valor", asociado a la clave "clave" al arbol de sufijos
*/
template<typename Dato>
void SuffixTree<Dato>::add(string clave, const Dato &valor) {
    for (int i = (int) clave.length() - 1; i >= 0; --i) {
        string a_insertar = clave.substr(i);
        this->addTrie(a_insertar, valor);
    }
}

/*
* Pre: ---
* Post: Elimina los datos contenidos en "resultados" y almacena en la variable
*       resultado un máximo de "MAX_NUM_DATOS_DEVOLVER" valores en cuyas claves
*       estuviese contenido el substring sub_clave
*/
template<typename Dato>
void SuffixTree<Dato>::search(const string clave, ListaOrdenada<Dato> &resultados) {
    resultados.clear();
    if (clave.length() > 0) {//string clave no es vacio
        typename SuffixTree<Dato>::Nodo *nodo_actual = &(this->cima);
        int i = 0;//iterador
        char letra_actual;
        bool estaContenido = true;
        while (estaContenido && (i < clave.length())) {//encuentro el substring que me proporcionan
            letra_actual = clave.at(i);
            if (nodo_actual->letra[this->obtposletr(letra_actual)] == nullptr) {
                estaContenido = false;
            } else {
                nodo_actual = nodo_actual->letra[this->obtposletr(letra_actual)];
            }
            ++i;
        }
        if (estaContenido) {
            nodo_actual->valores.copy(resultados);
        }
    }
}

/*
* Añade el par clave y valor al arbol de sufijos, tratando a este como si fuese
* un trie
*/
template<typename Dato>
void SuffixTree<Dato>::addTrie(string clave, const Dato &valor) {
    if (clave.length() > 0) {
        typename SuffixTree<Dato>::Nodo *nodo_actual = &(this->cima);
        for (int i = 0; i < clave.length(); i++) {
            char letra_actual = clave.at(i);
            if (nodo_actual->letra[this->obtposletr(letra_actual)] == nullptr) {
                nodo_actual->letra[this->obtposletr(letra_actual)] = new(typename SuffixTree<Dato>::Nodo);
                for (int j = 0; j < DIM_NUMERO_CARACTERES_RECONOCIDOS; j++) {//marco este nodo como una hoja
                    nodo_actual->letra[this->obtposletr(letra_actual)]->letra[j] = nullptr;
                }
            }
            nodo_actual = nodo_actual->letra[this->obtposletr(letra_actual)];

            if (!nodo_actual->valores.belongs(valor)) {//añado el dato al nodo
                nodo_actual->valores.add(valor);
            }
        }
    }
}

template<typename Dato>
SuffixTree<Dato>::SuffixTree() {
    for (int i = 0; i < DIM_NUMERO_CARACTERES_RECONOCIDOS; i++) {//marco este nodo como una hoja
        this->cima.letra[i] = nullptr;
    }
}

#endif //SUFFIXTREE_H
