#include "GestorPrecios.h"

/*
* Devuelve el precio asociado a un servicio en el cual se realicen np peticiones
*/
double obtener_precio_servicio(unsigned int np){
    return TASA_POR_ESTABLECIMIENTO_DE_SERVICIO + TASA_POR_PETICION * np;
}
