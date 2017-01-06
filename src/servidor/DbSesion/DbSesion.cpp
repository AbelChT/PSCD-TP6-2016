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
    numero_peticiones_totales=0;
}

void DbSesion::listarCliente(int idCliente, string& listado) {
    listado.clear();
    mtx_sesion.lock();//Pillo mutex

	if (pertenece(this->dicc, idCliente)) {

		Lista<string>* pedidos_cliente;
		obtenerValor(this->dicc, idCliente, pedidos_cliente); // Obtenemos la lista del cliente en particular al que queremos listar
        mtx_sesion.unlock();//Suelto mutex

        listado="Peticiones del cliente " + to_string(idCliente) + ":\n";

		pedidos_cliente->begin();	// Nos posicionamos en el primer pedido
		string pedido_actual;
		while(pedidos_cliente->next(pedido_actual)){	// Listamos todos sus pedidos hasta llegar al final de la lista que los recoge
			listado = listado + pedido_actual + "\n";
		}
		listado= listado + "TOTAL PETICIONES DEL CLIENTE: " + to_string(pedidos_cliente->size()) + "\n";

	} else{
        mtx_sesion.unlock();//Suelto mutex
    }
}

void DbSesion::nuevoPedido(int idCliente, string pedido) {
    mtx_sesion.lock();
	// Si el cliente no tiene pedidos lo registramos en el arbol y le añadimos a su lista de pedidos el nuevo pedido
	if (!pertenece(this->dicc, idCliente)) {
        Lista<string>* lista_pedidos=new Lista<string>;
        lista_pedidos->add(pedido);
		anyadir(this->dicc, idCliente, lista_pedidos);
	}
	// En caso contrario extraemos su lista de pedidos del arbol, añadimos el nuevo pedido, y la volvemos a introducir ya dispuesta
	else {
        Lista<string>* lista_pedidos;
		obtenerValor(this->dicc, idCliente, lista_pedidos);
        lista_pedidos->add(pedido);//estoy trabajando directamente sobre el dato almacenado
	}
    mtx_sesion.unlock();

    mtx_numero_peticiones_totales.lock();
    this->numero_peticiones_totales=this->numero_peticiones_totales+1;
    mtx_numero_peticiones_totales.unlock();
}

void DbSesion::borrarCliente(int idCliente) {
	mtx_sesion.lock();
    if (pertenece(this->dicc,idCliente)){
        Lista<string>* dato_a_borrar;
        obtenerValor(this->dicc,idCliente,dato_a_borrar);
        quitar(this->dicc, idCliente);
        delete dato_a_borrar;//llama automaticamente a ~Lista()
    };
    mtx_sesion.unlock();
}

int DbSesion::numeroPeticionesCliente(int idCliente){
	unique_lock<mutex> mtx_sesion;
	if (!pertenece(this->dicc, idCliente)) {
		return -1;
	}
	else{
		Lista<string>* lAux;
		obtenerValor(this->dicc,idCliente,lAux); // Obtenemos la lista de pedidos del cliente idCliente
		return lAux->size();					// Devolvemos su longitud, el numero de pedidos
	}
}

int DbSesion::numeroPeticionesTotales(){
		return this->numero_peticiones_totales;
}
