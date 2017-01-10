/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
******************************************************************/

#include "DbSesion.h"

using namespace std;

DbSesion::DbSesion() {
    crear(this->dicc);
}
void DbSesion::nuevoCliente(int idCliente) {
    mtx_sesion.lock();
    if (pertenece(this->dicc, idCliente)) {
        Lista<string> *dato_a_limpiar;
        obtenerValor(this->dicc, idCliente, dato_a_limpiar);
        dato_a_limpiar->clear();
    }
    else {
        Lista<string> *dato_a_add = new Lista<string>;
        anyadir(this->dicc, idCliente, dato_a_add);
    }
    mtx_sesion.unlock();
}

void DbSesion::nuevoPedido(int idCliente, string pedido) {
    mtx_sesion.lock();
    // Si el cliente no tiene pedidos lo registramos en el arbol y le añadimos a su lista de pedidos el nuevo pedido
    if (!pertenece(this->dicc, idCliente)) {
        Lista<string> *dato_a_add = new Lista<string>;
        dato_a_add->add(pedido);
        anyadir(this->dicc, idCliente, dato_a_add);
    }
    else {
        Lista<string> *dato_a_add;
        obtenerValor(this->dicc, idCliente, dato_a_add);
        dato_a_add->add(pedido);
    }
    mtx_sesion.unlock();
}




void DbSesion::borrarCliente(int idCliente) {
    mtx_sesion.lock();
    if (pertenece(this->dicc, idCliente)) {
        Lista<string> *dato_a_borrar;
        obtenerValor(this->dicc, idCliente, dato_a_borrar);
        quitar(this->dicc, idCliente);
        delete dato_a_borrar;//llama automaticamente a ~Lista()
    };
    mtx_sesion.unlock();
}

int DbSesion::numeroPeticionesCliente(int idCliente) {
    mtx_sesion.lock();
    if (!pertenece(this->dicc, idCliente)) {
        mtx_sesion.unlock();
        return -1;
    }
    else {
        Lista<string> *lAux;
        obtenerValor(this->dicc, idCliente, lAux);
        mtx_sesion.unlock();		 // Obtenemos la lista de pedidos del cliente idCliente
        return lAux->size();                    // Devolvemos su longitud, el numero de pedidos
    }
}

void DbSesion::listarCliente(int idCliente, string &listado) {
    listado.clear();
    mtx_sesion.lock();
    if (pertenece(this->dicc, idCliente)) {
        listado = "Peticiones del cliente " + to_string(idCliente) + ":\n";
        Lista<string> *pedidos_cliente;
        obtenerValor(this->dicc, idCliente,
            pedidos_cliente); // Obtenemos la lista del cliente en particular al que queremos listar
        mtx_sesion.unlock();
        pedidos_cliente->begin();
        string pedido;
        while (pedidos_cliente->next(pedido)) {
            listado = listado + pedido + "\n";
        }
        listado = listado + "TOTAL PETICIONES DEL CLIENTE: " + to_string(this->numeroPeticionesCliente(idCliente)) + "\n";
    }
    else mtx_sesion.unlock();
}
