/******************************************************************
* Author: Abel Chils Trabanco
* Date:   2 de enero de 2017
******************************************************************/

#ifndef DBMONUMENTOSRESTAURANTES_H
#define DBMONUMENTOSRESTAURANTES_H

#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>

/*
* Maximo número de monumentos que se devolverá en la lista
*/
#define MAX_NUM_MONUMENTOS_DEVOLVER 5

/*
* Maximo número de restaurantes que se devolverá en la lista
*/
#define MAX_NUM_RESTAURANTES_DEVOLVER 5

/*
* Estructura de datos donde se almacenan los monumentos
*/
SuffixTree<*Monumento> dbMonumentos(MAX_NUM_MONUMENTOS_DEVOLVER);

/*
* Estructura de datos donde se almacenan los restaurantes
*/
Lista<Restaurante> dbRestaurantes;

/*
* Mutex utilizado para garantizar que la operación buscarRestaurante se pueda
* invocar desde multiples threads
*/
mutex mtx_buscarRestaurante;

/*
* Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
* zaragoza y los carga en las estructuras correspondientes
*/
void cargarMonumentosRestaurantes();

/*
* Pre: Se han cargado los monumentos
* Post: Busca coincidencias de dato_a_buscar entre los monumentos almacenados y
*       devuelve un máximo de MAX_NUM_MONUMENTOS_DEVOLVER monumentos en los que
*       existan coincidencias con dato_a_buscar
*/
void buscarMonumento(string dato_a_buscar, Pila<Monumento> resultados);

/*
* Pre: Se han cargado los monumentos
* Post: Almacena en resultados los MAX_NUM_RESTAURANTES_DEVOLVER más cercanos
*       a monumento_seleccionado en el caso de que existan al menos MAX_NUM_RESTAURANTES_DEVOLVER
*       en dbRestaurantes
*       En caso contrario devuelve todos los restaurantes almacenados en dbRestaurantes
*/
void buscarRestaurante(Monumento monumento_seleccionado, Pila<Restautante> resultados);

#endif
