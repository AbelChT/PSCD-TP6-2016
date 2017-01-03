/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef MONUMENTO_H
#define MONUMENTO_H

#include <string>

using namespace std;
/*
* TAD que representa un monumento
*/
struct Monumento{

  //Titulo del monumento
  string title;

  //Link al monumento
  string link;

  //Coordenadas del monumento
  double coordenadas[2];// [0] long
                        //[1] lat
};
#endif//MONUMENTO_H
