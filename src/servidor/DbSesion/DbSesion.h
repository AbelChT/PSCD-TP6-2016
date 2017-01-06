//*****************************************************************
// File:   DbSesion.h
// Author: Grupo Abel Chils TP6
// Date:   diciembre 2016
// Coms:   Especificación de una librería
//*****************************************************************

#ifndef DBSESION_H
#define DBSESION_H

#include <iostream>
#include <mutex>

#include "../../librerias/Diccionario/diccionarios.h"
#include "../../librerias/Lista/Lista.h"

using namespace std;
class DbSesion {
public:

	/*
	 * Creamos una base de datos de sesion sin todavia ningun cliente
	 */
	DbSesion();

	/*
	 * Asigna a listado la cadena de caracteres correspondiente a listar todos los clientes y sus pedidos
	 * con el siguiente formato:
	 * LISTANDO TODO:
	 * Peticiones del cliente i:
	 * ...
	 * TOTAL PETICIONES CLIENTE i: nI
	 * Peticiones del cliente j:
	 * ...
	 * TOTAL PETICIONES CLIENTE j: nJ
	 * TOTAL PETICIONES: SUMATORIO peticiones(i-j)
	 */
	void listarTodo(string& listado);

	/*
	 * Asigna a listado la cadena de caracteres correspondiente a listar los pedidos de un cliente
	 * con el siguiente formato:
	 * Peticiones del cliente i:
	 * ...
	 * TOTAL PETICIONES CLIENTE i: nI
	 */
	void listarCliente(int idCliente,string& listado);

	/*
	 * En caso de que el cliente idCliente no tuviese ningun pedido previo realizado en la sesion s, lo añade al arbol tipo AVL dicc
	 * y le asigna pedido como su primer pedido en su lista de pedidos, en caso de que ya tuviera otro/s pedido/s añade este ultimo en
	 * la ultima posicion de su lista de pedidos
	 */
	void nuevoPedido(int idCliente, string pedido);

	/*
	 * Elimina al cliente idCliente de la sesion s
	 */
	void borrarCliente(int idCliente);

	/*
	 * Devuelve el entero correspondiente al numero de peticiones del cliente idCliente
	 */
	int nPeticionesCliente(int idCliente);

	/* Devuelve el entero correspondiente al numero de peticiones de la sesion s */

	int nPeticionesTotales();

private:

	/*
	 * Diccionario representado como un arbol tipo AVL que guarda pares tipo(cliente,lista de pedidos)
	 */
	Diccionario<int, Lista<string>*> dicc;

	/*
	 * Aseguramos la exclusion mutua en nuestras operaciones con este mutex mtx_sesion
	 */
	mutex mtx_sesion;
};

#endif
