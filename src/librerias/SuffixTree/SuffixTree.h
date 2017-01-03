/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <string>
#include "../Lista/Lista.h"//libreria lista

using namespace std;

/*
* Caracteres reconocidos
* El resto de caracteres se representan como 0
* letras del abecedario inglés (('z'-'a')+1) + numeros (10)
* + ñ (1) + espacio en blanco (1)
*/
#define DIM_NUMERO_CARACTERES_RECONOCIDOS (('z'-'a')+13)

/*
* El TAD SuffixTree representa un arbol de sufijos.
* Es un tipo de dato compuesto por una clave y un valor. La clave es siempre
* un dato de tipo string.
* Este tipo de dato esta optimizado para realizar busquedas en él.
*/
template<typename Dato> class SuffixTree {
public:

/*
* Pre: num_datos_devolver > 0
* Post: Crea un arbol de sufijos y establece el numero maximo de datos a devolver a
*       num_datos_devolver
*/
    SuffixTree(int num_datos_devolver);

/*
* Pre: ---
* Post: Agrega el dato "valor", asociado a la clave "clave" al arbol de sufijos
*/
    void add(const string clave, const Dato &valor);

/*
* Pre: ---
* Post: Elimina los datos contenidos en "resultados" y almacena en la variable
*       resultado un máximo de "MAX_NUM_DATOS_DEVOLVER" valores en cuyas claves
*       estuviese contenido el substring sub_clave
* Note: Esta función se puede ejecutar simultaneamente en diferentes hilos
*/
    void search(const string sub_clave, Lista<Dato> &resultados);

private:
  /*
  * Maximo número de datos que se devolverá en la lista
  */
  int MAX_NUM_DATOS_DEVOLVER;

  /*
  * Añade el par clave y valor al arbol de sufijos, tratando a este como si fuese
  * un trie
  */
    void addTrie(const string clave, const Dato &valor);

    struct Nodo{
        Nodo* letra[DIM_NUMERO_CARACTERES_RECONOCIDOS];
        Lista<Dato> valores;//Datos asociados al nodo en cuestion
    };

    //Cima del arbol de sufijos
    Nodo cima;

};

/*
* Transforma el caracter letra a un caracter que se pueda guardar en el trie
*/
unsigned int obtener_pos_letra(char letra) {
    if(letra<='z'&&letra>='a'){//letras minúsculas
        return (unsigned int)(letra-'a');
    }
    else if(letra<='Z'&&letra>='A'){//letra mayuscula
        return (unsigned int)(letra-'A');
    }
    else if((letra<='Ä'&&letra>='À')||(letra<='ä'&&letra>='à')){//a con tilte
        return (unsigned int)('a'-'a');
    }
    else if((letra<='Ë'&&letra>='È')||(letra<='ë'&&letra>='è')){//a con tilte
        return (unsigned int)('e'-'a');
    }
    else if((letra<='Ï'&&letra>='Ì')||(letra<='ï'&&letra>='ì')){//a con tilte
        return (unsigned int)('i'-'a');
    }
    else if((letra<='Ö'&&letra>='Ò')||(letra<='ö'&&letra>='ò')){//a con tilte
        return (unsigned int)('o'-'a');
    }
    else if((letra<='Ü'&&letra>='Ù')||(letra<='ü'&&letra>='ù')){//a con tilte
        return (unsigned int)('u'-'a');
    }
    else if(letra=='ñ'||letra=='Ñ'){
        return 'z'-'a'+1;
    }
    else{
        return 'z'-'a' + 2;
    }
}

/*
* Pre: ---
* Post: Agrega el dato "valor", asociado a la clave "clave" al arbol de sufijos
*/
template<typename Dato>
void SuffixTree<Dato>::add(string clave, const Dato &valor) {
    for (int i = (int) clave.length() - 1; i >= 0; --i) {
        string a_insertar=clave.substr(i);
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
void SuffixTree<Dato>::search(const string clave, Lista<Dato> &resultados) {
    resultados.clear();
    if (clave.length() > 0) {//string clave no es vacio
        typename SuffixTree<Dato>::Nodo *nodo_actual = &(this->cima);
        int i=0;//iterador
        char letra_actual;
        bool estaContenido = true;
        while (estaContenido && (i < clave.length())){//encuentro el substring que me proporcionan
            letra_actual = clave.at(i);
            if (nodo_actual->letra[obtener_pos_letra(letra_actual)] == nullptr) {
                estaContenido= false;
            } else {
                nodo_actual = nodo_actual->letra[obtener_pos_letra(letra_actual)];
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
            if (nodo_actual->letra[obtener_pos_letra(letra_actual)] == nullptr) {
                nodo_actual->letra[obtener_pos_letra(letra_actual)] = new(typename SuffixTree<Dato>::Nodo);
                for (int j = 0; j < DIM_NUMERO_CARACTERES_RECONOCIDOS; j++) {//marco este nodo como una hoja
                    nodo_actual->letra[obtener_pos_letra(letra_actual)]->letra[j] = nullptr;
                }
            }
            nodo_actual = nodo_actual->letra[obtener_pos_letra(letra_actual)];

            if((nodo_actual->valores.size()<this.MAX_NUM_DATOS_DEVOLVER)&&!nodo_actual->valores.belongs(valor)){//añado el dato al nodo
                nodo_actual->valores.add(valor);
            }
        }
    }
}

template<typename Dato>
SuffixTree<Dato>::SuffixTree(int num_datos_devolver) {

    this.MAX_NUM_DATOS_DEVOLVER=num_datos_devolver;
    for (int i = 0; i < DIM_NUMERO_CARACTERES_RECONOCIDOS; i++) {//marco este nodo como una hoja
        this->cima.letra[i] = nullptr;
    }
}

#endif //SUFFIXTREE_H
