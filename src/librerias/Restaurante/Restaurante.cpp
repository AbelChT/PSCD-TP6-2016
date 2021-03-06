/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
******************************************************************/

#include "Restaurante.h"

/*
 * Crea un restaurante
 */
Restaurante::Restaurante() {
    /* Vacio */
}

/*
* Crea un restaurante ubicado en "UTMNorthing" metros norte
* y "UTMEasting" metros este segun el sistema UTM
*/
Restaurante::Restaurante(double UTMNorthing, double UTMEasting) {
    this->coordenadas[0] = UTMNorthing;
    this->coordenadas[1] = UTMEasting;
}

/*
* Establece las coordenadas del restaurante a "UTMNorthing" metros norte
* y "UTMEasting" metros este segun el sistema UTM
*/
void Restaurante::setcoordenadas(double UTMNorthing, double UTMEasting) {
    this->coordenadas[0] = UTMNorthing;
    this->coordenadas[1] = UTMEasting;
}

/*
* UTMNorthing y UTMEasting tomarán el valor de la ubicación del restaurante, expresada
* en metros norte y metros este respectivamente según el sistema UTM
*/
void Restaurante::getcoordenadas(double &UTMNorthing, double &UTMEasting) {
    UTMNorthing = this->coordenadas[0];
    UTMEasting = this->coordenadas[1];
}
