//*****************************************************************
// File:   trie.h
// Date:   18 de diciembre 2016
//*****************************************************************

#ifndef TRIE_H
#define TRIE_H
/***********************/
/***  PARTE PUBLICA  ***/
/***********************/
using namespace std;

/*
* Los valores del TAD trie representan un conjunto de substring optimizado para
* realizar busquedas
*/
template <typename Dato> struct Trie;

/*
* Devuelve en Trie vacio, sin elementos
*/
template <typename Dato> void crear(Trie<Dato>& trie);

/*
* Devuelve en trie el Trie resultante de añadir la clave clave,la cual tiene asociado el valor
*/
template <typename Dato> void anyadir(Trie<Dato>& trie,string clave, const Dato& valor);

/*
* Devuelve en resultados un máximo de 5 valores, en cuyas claves estuviese contenido
* el substring sub_clave
*/
template <typename Dato> void buscar(Trie<Dato>& trie, string sub_clave, Pila<Dato>& resultados);

/***********************/
/***  PARTE PRIVADA  ***/
/***********************/
template <typename Dato>
struct Trie{
  friend void crear<Dato>(Trie<Dato>& trie);
  friend void anyadir<Dato>(Trie<Dato>& trie,string clave, const Dato& valor);
  friend void buscar<Dato>(Trie<Dato>& trie, string sub_clave, Pila<Dato>& resultados);

private:
    struct Nodo{
        Nodo* letra[(a-z)+2];//letras del abecedario inglés + ñ
        Dato* valor;
    };
    Nodo cima;
}

/*
* Devuelve en Trie vacio, sin elementos
*/
template <typename Dato> void crear(Trie<Dato>& trie){
  for (int i = 0; i < (a-z)+2; i++) {//marco este nodo como una hoja
    trie.cima.letra[i]=nullptr;
    trie.cima.valor=nullptr;
  }
}
unsigned int obtener_pos_letra(char letra){

}

/*
* Devuelve en trie el Trie resultante de añadir la clave clave, la cual tiene asociado el valor
*/
template <typename Dato> void anyadir(Trie<Dato>& trie,string clave, const Dato& valor){
    typename Trie<Dato>::Nodo* nodo_actual = &(trie.cima);
  for (int i = 0; i < clave.length(); i++) {
    char letra_actual=clave.at(i);

    if(nodo_actual.letra[]){

    }

  }
}

#endif //TRIE_H
