#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)

int main(int argc, char *argv[]) {
    const string MENS_FIN("Fin");
    // DirecciÃ³n y nÃºmero donde escucha el proceso servidor
    string SERVER_ADDRESS = argv[1];
    int SERVER_PORT = atoi(argv[2]);
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_ADDRESS, SERVER_PORT);
    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// ConexiÃ³n con el servidor
    	socket_fd = socket.Connect();
    	count++;

    	// Si error --> esperamos 1 segundo para reconectar
    	if(socket_fd == -1){
    	    this_thread::sleep_for(chrono::seconds(1));
    	}
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexiÃ³n
    if(socket_fd == -1){
    	return socket_fd;
    }

/*Solicitud inicial
**********************************************************************/
    string mensaje;
   // Leer mensaje de la entrada estandar
	cout << "Inserte hasta cinco parametros de busqueda: ";
	getline(cin, mensaje);
	// Enviamos el mensaje
	   int send_bytes = socket.Send(socket_fd, mensaje);
	   if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	  // Buffer para almacenar la respuesta
 	string buffer;
	// Recibimos la respuesta del servidor  
 	int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	if (buffer=="Servicio denegado"||buffer=="Nada encontrado") cout << buffer << endl;
	else {
		string ABRIRURL="gnome-open "+ buffer;
		system((ABRIRURL).data());
	}

/*Fin o restaurante
*************************************************************************/
	cout << "Finalizar ('Fin') o mostrar restaurante ('Rest'): " << endl;
	getline(cin, mensaje);
	send_bytes = socket.Send(socket_fd, mensaje);
	if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	// Recibimos la respuesta del servidor  
 	 read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
	// PARA TESTEAR:
	cout << "Mensaje enviado: '" << mensaje << "'" << endl;
	cout << "Mensaje recibido: " << buffer << endl;

/*Finalización
**************************************************************************/
	cout << "Finalizar ('Fin'): " << endl;
	getline(cin, mensaje);
	send_bytes = socket.Send(socket_fd, mensaje);
	 if(send_bytes == -1){
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	// Recibimos la respuesta del servidor  
 	read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);

	// Mostramos la respuesta
	cout << "Mensaje enviado: '" << mensaje << "'" << endl;
	cout << "Mensaje recibido: " << buffer << endl;


    // Cerramos el socket
    int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    return error_code;
}
