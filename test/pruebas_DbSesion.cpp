#include "DbSesion.h"
#include "diccionarios.h"

#include <iostream>
#include <string>
#include <thread>

using namespace std;

int main() {
	sesion s;
	crearSesion(s);
	cout << "Aniadiendo diversos pedidos" << endl;
	nuevoPedido(1, "HolaC1", s);
	nuevoPedido(2, "HolaC2", s);
	nuevoPedido(3, "HolaC3", s);
	nuevoPedido(4, "HolaC4", s);
	nuevoPedido(1, "HolaC12", s);
	nuevoPedido(1, "HolaC13", s);
	nuevoPedido(2, "HolaC22", s);
	nuevoPedido(2, "HolaC23", s);
	nuevoPedido(2, "HolaC34", s);
	string s1, s2, s3, s4, s5, s6;
	cout << "Procediendo a listar los cuatro clientes en orden y luego todos " << endl;
	listarCliente(s, 1,s1);
	cout << s1 << endl;
	listarCliente(s, 2,s2);
	cout << s2 << endl;
	listarCliente(s, 3,s3);
	cout << s3 << endl;
	listarCliente(s, 4,s4);
	cout << s4 << endl;
	listarTodo(s,s5);
	cout << s5 << endl;
}
