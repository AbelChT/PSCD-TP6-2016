/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef DBMONUMENTOSRESTAURANTES_H
#define DBMONUMENTOSRESTAURANTES_H

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>

#include "../../librerias/JsonConverter/json.hpp"
#include "../../librerias/JsonDownload/JsonDownload.h"
#include "../../librerias/Monumento/monumento.h"
#include "../../librerias/Restaurante/restaurante.h"
#include "../../librerias/Lista/Lista.h"
#include "../../librerias/SuffixTree/SuffixTree.h"



class DbMonumentosRestaurantes {
public:
    /*
     * Pre: MAX_NUM_MONUMENTOS_DEVOLVER > 0 y MAX_NUM_RESTAURANTES_DEVOLVER > 0
* Post: Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
* zaragoza y los carga en las estructuras correspondientes
     *
*/
    DbMonumentosRestaurantes(int MAX_NUM_MONUMENTOS_DEVOLVER,int MAX_NUM_RESTAURANTES_DEVOLVER);

/*
* Pre: Se han cargado los monumentos
* Post: Busca coincidencias de dato_a_buscar entre los monumentos almacenados y
*       devuelve un máximo de MAX_NUM_MONUMENTOS_DEVOLVER monumentos en los que
*       existan coincidencias con dato_a_buscar
*/
    void buscarMonumento(string dato_a_buscar, Lista<Monumento> &resultados);

/*
* Pre: Se han cargado los monumentos
* Post: Almacena en resultados los MAX_NUM_RESTAURANTES_DEVOLVER más cercanos
*       a monumento_seleccionado en el caso de que existan al menos MAX_NUM_RESTAURANTES_DEVOLVER
*       en dbRestaurantes
*       En caso contrario devuelve todos los restaurantes almacenados en dbRestaurantes
*/
    void buscarRestaurante(double monumento_longitud, double monumento_latitud,
                         double& restaurante_longitud, double& restaurante_latitud);

private:
/*
* Función auxiliar utilizada para calcular la distancia entre un restaurante y un
* monumento
* Devuelve el cuadrado de la distancia de "a" a "b"
*/
    double calcular_distancia(Restaurante& a , Monumento& b);
    /*
* Maximo número de monumentos que se devolverá en la lista
*/
 int MAX_NUM_MONUMENTOS_DEVOLVER;

/*
* Maximo número de restaurantes que se devolverá en la lista
*/
 int MAX_NUM_RESTAURANTES_DEVOLVER;

    /*
* Estructura de datos donde se almacenan los monumentos
*/
    SuffixTree<Monumento *>* dbMonumentos;

/*
* Estructura de datos donde se almacenan los restaurantes
*/
    Lista<Restaurante> dbRestaurantes;

/*
* Mutex utilizado para garantizar que la operación buscarRestaurante se pueda
* invocar desde multiples threads
*/
    mutex mtx_buscarRestaurante;

};

#endif
