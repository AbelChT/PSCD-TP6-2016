/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
******************************************************************/

#ifndef PRECIOS_H
#define PRECIOS_H

class GestorPrecios {
public:

    /*
     * Pre: ---
     * Post: Crea un gestor de precios en el cual se cobre "TASA_POR_PETICION" unidades monetarias
     *       por petición realizada y "TASA_POR_ESTABLECIMIENTO_DE_SERVICIO" unidades monetarias por
     *       establecer el servicio
     */
    GestorPrecios(double TASA_POR_PETICION, double TASA_POR_ESTABLECIMIENTO_DE_SERVICIO);

    /*
    * Pre: ---
    * Post: Devuelve el precio asociado a un servicio en el cual se realicen np peticiones
    */
    double precio(unsigned int np);

private:

    /*
    * La constante TASA_POR_PETICION denota el precio que tiene cada petición al servicio.
    */
    double TASA_POR_PETICION;

    /*
    * La constante TASA_POR_ESTABLECIMIENTO_DE_SERVICIO denota el precio que se cobra por establecer el servicio
    */
    double TASA_POR_ESTABLECIMIENTO_DE_SERVICIO;

};

#endif
