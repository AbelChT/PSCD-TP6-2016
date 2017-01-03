//*****************************************************************
// File:   DbSesion.cpp
// Author: Grupo Abel Chils TP6
// Date:   diciembre 2016
// Coms:   
//*****************************************************************

#include <iostream>

#include "diccionario.h"
#include <list>
#include "DbSesion.h"
#include <string>

using namespace std;

void crearSesion(sesion& s) {
	crear(s.dicc);
}

void listarTodo(sesion& s,string& listado) {
	listado="";
	if (!esVacio(s.dicc)) {
		iniciarIterador(s.dicc);
		list<string> lAux;
		listado= "LISTANDO TODO :\n";
		while (existeSiguiente(s.dicc)) {
			int nAux;
			string listadoAux;
			siguiente(s.dicc, nAux, lAux);
			listarCliente(s,nAux,listadoAux);
			listado=listado + listadoAux;
		}
		listado= listado + "PETICIONES TOTALES :" + to_string(nPeticionesTotales(s)) +  "\n";
	}
}

void listarCliente(sesion &s,int idCliente,string& listado) {
	listado="";
	if (pertenece(s.dicc, idCliente)) {
		listado="Peticiones del cliente " + to_string(idCliente) + ":\n";
		list<string> lAux;
		obtenerValor(s.dicc, idCliente, lAux);
		if(!lAux.empty()){
			list<string>::iterator it=lAux.begin();
			string sAux;
			while(it!=lAux.end()){
				sAux=*it;
				listado = listado + sAux + "\n";
				it++;
			}
			listado= listado + "TOTAL PETICIONES DEL CLIENTE: " + to_string(nPeticionesCliente(s,idCliente)) + "\n";
		}
	}
}

void nuevoPedido(int idCliente, string pedido, sesion& s) {
	list<string> lAux;
	if (!pertenece(s.dicc, idCliente)) {
		lAux.push_front(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
	else {
		obtenerValor(s.dicc, idCliente, lAux);
		lAux.push_back(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
}

void borrarCliente(int idCliente, sesion& s) {
	quitar(s.dicc, idCliente);
}

int nPeticionesCliente(sesion& s, int idCliente){
	if (!pertenece(s.dicc, idCliente)) {
		return 0;
	}
	else{
		list<string> lAux;
		obtenerValor(s.dicc,idCliente,lAux);
		return lAux.size();
	}
}

int nPeticionesTotales(sesion& s){
	if(!esVacio(s.dicc)){
		int acum=0;
		iniciarIterador(s.dicc);
		list<string> lAux;
		int idClienteAux;
		while(existeSiguiente(s.dicc)){
			siguiente(s.dicc,idClienteAux,lAux);
			acum=acum+lAux.size();
		}
		return acum;
	}
	else{
		return 0;
	}
}
