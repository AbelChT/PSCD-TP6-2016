//*****************************************************************
// File:   DbSesion.h
// Author: Grupo Abel Chils TP6
// Date:   diciembre 2016
// Coms:   Especificación de una librería
//*****************************************************************

#ifndef DBSESION_H
#define DBSESION_H

#include <iostream>
#include "../../librerias/Diccionario/diccionarios.h"
#include <list>
#include <mutex>

using namespace std;

struct sesion;

void crearSesion(sesion& s);

void listarTodo(sesion& s,string& listado);

void listarCliente(sesion &s,int idCliente,string& listado);

void nuevoPedido(int idCliente, string pedido,sesion& s);

void borrarCliente(int idCliente, sesion& s);

int nPeticionesCliente(sesion& s, int idCliente);

int nPeticionesTotales(sesion& s);

struct sesion {
public:
	friend void crearSesion(sesion& s);
	friend void listarTodo(sesion& s,string& listado);
	friend void listarCliente(sesion &s,int idCliente,string& listado);
	friend void nuevoPedido(int idCliente, string pedido, sesion& s);
	friend void borrarCliente(int idCliente, sesion& s);
	friend int nPeticionesCliente(sesion& s, int idCliente);
	friend int nPeticionesTotales(sesion& s);
private:
	Diccionario<int, list<string>*> dicc;
	mutex mtx_sesion;
};

#endif
