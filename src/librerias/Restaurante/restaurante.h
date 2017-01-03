/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <string>

using namespace std;

/*
* TAD que representa un restaurante
*/

struct Restaurante{

  //Nombre del restaurante
  string nombre;

  //Link al restaurante
  string link;

  //Descripcion del retaurante
  string direccion;

  //Categoria del retaurante
  string categoria;

  //Ubicación del restaurante
  double coordenadas[2];// [0] long
                        //[1] lat
};

#endif
