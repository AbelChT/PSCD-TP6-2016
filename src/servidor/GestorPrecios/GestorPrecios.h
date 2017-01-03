/*
* La constante TASA_POR_PETICION denota el precio que tiene cada petición al servicio.
*/
const double TASA_POR_PETICION=0.12;

/*
* La constante TASA_POR_ESTABLECIMIENTO_DE_SERVICIO denota el precio que se cobra por establecer el servicio
*/
const double TASA_POR_ESTABLECIMIENTO_DE_SERVICIO=1.3;

/*
* Devuelve el precio asociado a un servicio en el cual se realicen np peticiones
*/
double obtener_precio_servicio(unsigned int np);
