//*****************************************************************
// File:   DbSesion.cpp
// Author: Grupo Abel Chils TP6
// Date:   diciembre 2016
// Coms:
//*****************************************************************

#include "DbSesion.h"

using namespace std;

void DbSesion::DbSesion() {
	crear(this->dicc);
}

void DbSesion::listarTodo(string& listado) {
	unique_lock<mutex> mtx_sesion;
	listado="";
	if (!esVacio(this->dicc)) {
		iniciarIterador(this->dicc);	// Iniciamos el iterador en el primer elemento del AVL
		Lista<string>* lAux;
		listado= "LISTANDO TODO :\n";
		while (existeSiguiente(this->dicc)) {	// Avanzamos recorriendo el AVL de menor a mayor ID de sus clientes registrados
			int nAux;
			string listadoAux;
			siguiente(this->dicc, nAux, lAux);	// Obtenemos la lista de pedidos del cliente que estamos visitando en este momento

			1/*
			 * Hacer parte de listado
			 * */


			listado=listado + listadoAux;
		}
		listado= listado + "PETICIONES TOTALES :" + to_string(nPeticionesTotales(s)) +  "\n";
	}
}

void DbSesion::listarCliente(int idCliente, string& listado) {
	unique_lock<mutex> mtx_sesion;
	listado="";
	if (pertenece(s.dicc, idCliente)) {
		listado="Peticiones del cliente " + to_string(idCliente) + ":\n";
		list<string> lAux;
		obtenerValor(s.dicc, idCliente, lAux); // Obtenemos la lista del cliente en particular al que queremos listar
		if(!lAux.empty()){
			list<string>::iterator it=lAux.begin();	// Nos posicionamos en el primer pedido
			string sAux;
			while(it!=lAux.end()){	// Listamos todos sus pedidos hasta llegar al final de la lista que los recoge
				sAux=*it;
				listado = listado + sAux + "\n";
				it++;	// Siguiente pedido
			}
			listado= listado + "TOTAL PETICIONES DEL CLIENTE: " + to_string(nPeticionesCliente(s,idCliente)) + "\n";
		}
	}
}

void DbSesion::nuevoPedido(int idCliente, string pedido, sesion& s) {
	unique_lock<mutex> mtx_sesion;
	list<string> lAux;
	// Si el cliente no tiene pedidos lo registramos en el arbol y le añadimos a su lista de pedidos el nuevo pedido
	if (!pertenece(s.dicc, idCliente)) {
		lAux.push_front(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
	// En caso contrario extraemos su lista de pedidos del arbol, añadimos el nuevo pedido, y la volvemos a introducir ya dispuesta
	else {
		obtenerValor(s.dicc, idCliente, lAux);
		lAux.push_back(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
}

void DbSesion::borrarCliente(int idCliente, sesion& s) {
	unique_lock<mutex> mtx_sesion;
	quitar(s.dicc, idCliente);
}

int DbSesion::nPeticionesCliente(sesion& s, int idCliente){
	unique_lock<mutex> mtx_sesion;
	if (!pertenece(s.dicc, idCliente)) {
		return 0;
	}
	else{
		list<string> lAux;
		obtenerValor(s.dicc,idCliente,lAux); // Obtenemos la lista de pedidos del cliente idCliente
		return lAux.size();					// Devolvemos su longitud, el numero de pedidos
	}
}

int DbSesion::nPeticionesTotales(sesion& s){
	unique_lock<mutex> mtx_sesion;
	if(!esVacio(s.dicc)){
		int acum=0;
		iniciarIterador(s.dicc); // Iniciamos el iterador del arbol en el primer elemento in-orden
		list<string> lAux;
		int idClienteAux;
		while(existeSiguiente(s.dicc)){	// Mientras existan elementos para recorrer avanzamos
			siguiente(s.dicc,idClienteAux,lAux); // Obtenemos la lista de pedidos del cliente idCliente con esta operacion
			acum=acum+lAux.size();		// Acumulamos su numero de pedidos al numero de pedidos totales hasta el momento
		}
		return acum; // Devolvemos el numero de pedidos totales
	}
	else{
		return 0;
	}
}
