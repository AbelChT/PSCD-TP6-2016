#include <string>
#include <iostream>
#include "../../../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"

using namespace std;

//
// Created by Abel on 03/01/2017.
//
int main(){
    DbMonumentosRestaurantes prueba(5,5);
    Lista<Monumento> list;
    prueba.buscarMonumento("a",list);


    return 0;
}
