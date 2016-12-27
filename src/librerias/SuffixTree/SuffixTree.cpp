//
// Created by Abel on 27/12/2016.
//

#include "Suffixtree.h"

using namespace std;

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

template<typename Dato>
void SuffixTree::add(string clave, const Dato &valor) {
    for (int i = (int) clave.length() - 1; i >= 0; --i) {
        this->addTrie(clave, valor.substr(i));
    }
}

template<typename Dato>
void SuffixTree::search(const string clave, Lista<Dato> &resultados) {
    resultados.vaciar();
    if (clave.length() > 0) {
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
            int k=0;
            nodo_actual->valores.iniciarIterador();
            Dato d;
            while(nodo_actual->valores.siguienteElemento(d)){
                resultados.anyadir(d);
                ++k;
            }
        }
    }
}

template<typename Dato>
void SuffixTree::addTrie(string clave, const Dato &valor) {
    if (clave.length() > 0) {
        typename SuffixTree<Dato>::Nodo *nodo_actual = &(this->cima);

        for (int i = 0; i < clave.length(); i++) {
            char letra_actual = clave.at(i);
            if (nodo_actual->letra[obtener_pos_letra(letra_actual)] == nullptr) {
                nodo_actual->letra[obtener_pos_letra(letra_actual)] = new(typename SuffixTree<Dato>::Nodo);
                for (int j = 0; j < DIM_NUMERO_CARACTERES_RECONOCIDOS; j++) {//marco este nodo como una hoja
                    nodo_actual->letra[obtener_pos_letra(letra_actual)]->letra[j] = nullptr;
                }
                nodo_actual->letra[obtener_pos_letra(letra_actual)]->valores = nullptr;
            }
            nodo_actual = nodo_actual->letra[obtener_pos_letra(letra_actual)];
        }

        if((nodo_actual->valores.tamanyo()<MAX_NUM_DATOS_DEVOLVER)&&!nodo_actual->valores.pertenece(valor)){
            nodo_actual->valores.anyadir(valor);
        }
    }
}

template<typename Dato>
SuffixTree::SuffixTree() {
    this->cima.valores = nullptr;
    for (int i = 0; i < DIM_NUMERO_CARACTERES_RECONOCIDOS; i++) {//marco este nodo como una hoja
        this->cima.letra[i] = nullptr;
    }
}
