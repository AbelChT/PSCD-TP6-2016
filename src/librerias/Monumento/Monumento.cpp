/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/
#include "Monumento.h"

using namespace std;

/*
* Crea un monumento
*/
Monumento::Monumento() {
    /*VACIO*/
}

/*
* Crea un monumento con sitio web "link" y ubicado en "UTMNorthing" metros norte
* y "UTMEasting" metros este segun el sistema UTM
*/
Monumento::Monumento(string link, double UTMNorthing, double UTMEasting) {
    this->link = link;
    this->coordenadas[0] = UTMNorthing;
    this->coordenadas[1] = UTMEasting;
}

/*
* Establece link_in como la url del monumento
*/
void Monumento::setlink(string link_in) {
    this->link = link_in;
}

/*
* Devuelve la direccion web del monumento
*/
string Monumento::getlink() {
    return this->link;
}

/*
* Establece las coordenadas del monumento a "UTMNorthing" metros norte
* y "UTMEasting" metros este segun el sistema UTM
*/
void Monumento::setcoordenadas(double UTMNorthing, double UTMEasting) {
    this->coordenadas[0] = UTMNorthing;
    this->coordenadas[1] = UTMEasting;
}

/*
* UTMNorthing y UTMEasting tomarán el valor de la ubicación del monumento, expresada
* en metros norte y metros este respectivamente según el sistema UTM
*/
void Monumento::getcoordenadas(double &UTMNorthing, double &UTMEasting) {
    UTMNorthing = this->coordenadas[0];
    UTMEasting = this->coordenadas[1];
}
