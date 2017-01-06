/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/
 #include "Restaurante.h"
 
/*
* Crea un restaurante ubicado en "UTMNorthing" metros norte
* y "UTMEasting" metros este segun el sistema UTM
*/
Restaurante::Restaurante(double UTMNorthing, double UTMEasting){
  this->coordenadas[0]=UTMNorthing;
  this->coordenadas[1]=UTMEasting;
}

/*
* UTMNorthing y UTMEasting tomarán el valor de la ubicación del restaurante, expresada
* en metros norte y metros este respectivamente según el sistema UTM
*/
void Restaurante::getcoordenadas(double& UTMNorthing, double& UTMEasting){
  UTMNorthing=this->coordenadas[0];
  UTMEasting=this->coordenadas[1];
}
