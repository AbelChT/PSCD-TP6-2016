#include <string>
#include <iostream>
#include "../../../src/librerias/Lista/Lista.h"
#include "../../../src/librerias/SuffixTree/SuffixTree.h"

using namespace std;

//
// Created by Abel on 03/01/2017.
//
int main(){
    Lista<string> lista;
    Lista<string> lista2;
    SuffixTree<string> arbol(5);

    for (int i = 0; i < 10; ++i) {
        lista.add("prueba"+to_string(i));
    }
    lista.begin();
    string resultado;
    while (lista.next(resultado)){
        cout<<resultado<<endl;
    }
    lista.copy(lista2);
    lista2.begin();
    while (lista2.next(resultado)){
        cout<<resultado<<endl;
    }
    cout<<lista.size()<<endl;
    cout<<lista2.size()<<endl;

    string ref1="referencia1";
    arbol.add("prueba",ref1);

    string ref2="referencia2";
    arbol.add("perro",ref2);


    string ref3="referencia3";
    arbol.add("gato",ref3);

    string ref4="referencia4";
    arbol.add("parro",ref4);

    string ref5="referencia5";
    arbol.add("trac999r",ref5);


    arbol.search("rro",lista);
    lista.begin();

    while (lista.next(resultado)){
        cout<<resultado<<endl;
    }
    cout<<"pasado"<<endl;

    arbol.search("ue",lista);
    lista.begin();

    while (lista.next(resultado)){
        cout<<resultado<<endl;
    }
    cout<<"pasado"<<endl;

    arbol.search("to",lista);
    lista.begin();

    while (lista.next(resultado)){
        cout<<resultado<<endl;
    }
    cout<<"pasado"<<endl;

    arbol.search("9",lista);
    lista.begin();

    while (lista.next(resultado)){
        cout<<resultado<<endl;
    }
    cout<<"pasad9o"<<endl;


    return 0;
}
