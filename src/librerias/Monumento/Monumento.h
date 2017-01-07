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

class Monumento {
public:

    /*
    * Crea un monumento
    */
    Monumento();

    /*
    * Crea un monumento con sitio web "link" y ubicado en "UTMNorthing" metros norte
    * y "UTMEasting" metros este segun el sistema UTM
    */
    Monumento(string link, double UTMNorthing, double UTMEasting);

    /*
    * Establece link_in como la url del monumento
    */
    void setlink(string link_in);

    /*
    * Devuelve la direccion web del monumento
    */
    string getlink();

    /*
    * Establece las coordenadas del monumento a "UTMNorthing" metros norte
    * y "UTMEasting" metros este segun el sistema UTM
    */
    void setcoordenadas(double UTMNorthing, double UTMEasting);

    /*
    * UTMNorthing y UTMEasting tomarán el valor de la ubicación del monumento, expresada
    * en metros norte y metros este respectivamente según el sistema UTM
    * Coordenadas UTM30
    */
    void getcoordenadas(double &UTMNorthing, double &UTMEasting);

private:
    //Link al monumento
    string link;

    /*
    * Coordenadas del monumento
    * [0] UTMNorthing
    * [1] UTMEasting
    */
    double coordenadas[2];
};

#endif//MONUMENTO_H
