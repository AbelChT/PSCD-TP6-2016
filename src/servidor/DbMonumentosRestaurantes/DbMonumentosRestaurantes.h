#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>

void cargarMonumentosRestaurantes();
bool buscarMonumento(string substring, Pila<Monumento> resultados);
bool buscarRestaurante(Monumento seleccionado, Pila<Restautante> resultados);
