#include "GestorPrecios.h"

/*
 * Pre: ---
 * Post: Crea un gestor de precios en el cual se cobre "TASA_POR_PETICION" unidades monetarias
 *       por petición realizada y "TASA_POR_ESTABLECIMIENTO_DE_SERVICIO" unidades monetarias por
 *       establecer el servicio
 */
GestorPrecios::GestorPrecios(double TP, double TES){
    this->TASA_POR_PETICION=TP;
    this->TASA_POR_ESTABLECIMIENTO_DE_SERVICIO=TES;
}

/*
* Pre: ---
* Post: Devuelve el precio asociado a un servicio en el cual se realicen np peticiones
*/
double GestorPrecios::precio(unsigned int np){
    return this->TASA_POR_ESTABLECIMIENTO_DE_SERVICIO + this->TASA_POR_PETICION * np;
}
