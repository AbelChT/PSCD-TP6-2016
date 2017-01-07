/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef RESTAURANTE_H
#define RESTAURANTE_H

/*
* TAD que representa un restaurante
*/

class Restaurante {
public:
    /*
     * Crea un restaurante
     */
    Restaurante();

    /*
    * Crea un restaurante ubicado en "UTMNorthing" metros norte
    * y "UTMEasting" metros este segun el sistema UTM
    */
    Restaurante(double UTMNorthing, double UTMEasting);

    /*
    * Establece las coordenadas del restaurante a "UTMNorthing" metros norte
    * y "UTMEasting" metros este segun el sistema UTM
    */
    void setcoordenadas(double UTMNorthing, double UTMEasting);

    /*
    * UTMNorthing y UTMEasting tomarán el valor de la ubicación del restaurante, expresada
    * en metros norte y metros este respectivamente según el sistema UTM
    * Coordenadas UTM30
    */
    void getcoordenadas(double &UTMNorthing, double &UTMEasting);

private:

    /*
    * Coordenadas del restaurante
    * [0] UTMNorthing
    * [1] UTMEasting
    */
    double coordenadas[2];

};

#endif
