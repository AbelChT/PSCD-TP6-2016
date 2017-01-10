//*****************************************************************
// File:   DbSesion.cpp
// Author: Grupo Abel Chils TP6
// Date:   diciembre 2016
// Coms:   
//*****************************************************************

#include <iostream>
#include <list>
#include "DbSesion.h"
#include <string>

using namespace std;


void crearSesion(sesion& s) {
	std::unique_lock<std::mutex> lck(s.mtx_sesion);
	crear(s.dicc);
}

void listarTodo(sesion& s,string& listado) {
	listado="";
	s.mtx_sesion.lock();
	if (!esVacio(s.dicc)) {
		iniciarIterador(s.dicc);
		list<string> *lAux=new list<string>;
		listado= "LISTANDO TODO :\n";
		while (existeSiguiente(s.dicc)) {
			int nAux;
			string listadoAux;
			siguiente(s.dicc, nAux, lAux);
			s.mtx_sesion.unlock();
			listarCliente(s,nAux,listadoAux);
			listado=listado + listadoAux;
		}
		listado= listado + "PETICIONES TOTALES :" + to_string(nPeticionesTotales(s)) +  "\n";
	}
}

void listarCliente(sesion &s,int idCliente,string& listado) {
	listado="";
	s.mtx_sesion.lock();
	if (pertenece(s.dicc, idCliente)) {
		listado="Peticiones del cliente " + to_string(idCliente) + ":\n";
		list<string> *lAux=new list<string>;
		obtenerValor(s.dicc, idCliente, lAux);
		s.mtx_sesion.unlock();
		if(!lAux->empty()){
			list<string>::iterator it=lAux->begin();
			string sAux;
			while(it!=lAux->end()){
				sAux=*it;
				listado = listado + sAux + "\n";
				it++;
			}
			listado= listado + "TOTAL PETICIONES DEL CLIENTE: " + to_string(nPeticionesCliente(s,idCliente)) + "\n";
		}
	}
}

void nuevoPedido(int idCliente, string pedido, sesion& s) {
	std::unique_lock<std::mutex> lck(s.mtx_sesion);
	list<string> *lAux=new list<string>;
	if (!pertenece(s.dicc, idCliente)) {
		lAux->push_front(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
	else {
		obtenerValor(s.dicc, idCliente, lAux);
		lAux->push_back(pedido);
		anyadir(s.dicc, idCliente, lAux);
	}
}

void borrarCliente(int idCliente, sesion& s) {
	std::unique_lock<std::mutex> lck(s.mtx_sesion);
    if (pertenece(s.dicc, idCliente)) {
        list<string> *dato_a_borrar=new list<string>;
        obtenerValor(s.dicc, idCliente, dato_a_borrar);
        quitar(s.dicc, idCliente);
        delete dato_a_borrar;
    }
}

int nPeticionesCliente(sesion& s, int idCliente){
	std::unique_lock<std::mutex> lck(s.mtx_sesion);
	if (!pertenece(s.dicc, idCliente)) {
		return 0;
	}
	else{
		list<string> *lAux=new list<string>;
		obtenerValor(s.dicc,idCliente,lAux);
		return lAux->size();
	}
}

int nPeticionesTotales(sesion& s){
	std::unique_lock<std::mutex> lck(s.mtx_sesion);
	if(!esVacio(s.dicc)){
		int acum=0;
		iniciarIterador(s.dicc);
		list<string> *lAux=new list<string>;
		int idClienteAux;
		while(existeSiguiente(s.dicc)){
			siguiente(s.dicc,idClienteAux,lAux);
			acum=acum+lAux->size();
		}
		return acum;
	}
	else{
		return 0;
	}
}
