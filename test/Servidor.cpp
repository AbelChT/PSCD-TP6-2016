#include <iostream>
#include "Socket.h"
#include <thread>

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)

//-------------------------------------------------------------
// Trocea y formatea el mensaje
//Para pruebas
void troceaFormatea(string message, string p[]) {
	int j=-1;
	int cont=0;
	int aux;
	for (int i=0; i<message.length(); i++){
		if (message[i]== ' '){
			aux=j+1;
		 	j=i;
			p[cont]=message.substr(aux, j-aux);
			cont++;
		}
		else if (i==(message.length())-1){
			aux=j+1;
		 	j=i+1;
			p[cont]=message.substr(aux, j-aux);
		}
	}		
}

//---------------------------------------------------------------
//Función que utilizan los thread
int auxiliar(int client_fd, int socket_fd, int error_code, Socket socket){
// Buffer para recibir el mensaje
	char MENS_FIN[]="Fin";
	int length = 100;
	string buffer;
	string resp;
	/*Primer mensaje
	******************************************************/
	// Recibimos el mensaje del cliente
	int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
	if(rcv_bytes == -1) {
		cerr << "Error al recibir datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		socket.Close(socket_fd);
	}
	//Pruebas análisis de mensaje y respuesta en consecuencia
	string p[5];
	troceaFormatea(buffer, p);
	for (int n=0; n<5; ++n) if (p[n].length()!=0) cout << p[n] << endl;
	if (p[0]=="Denegar") resp="Servicio denegado";
	else if (p[0]=="Nada") resp="Nada encontrado";
	else resp="http://www.zaragoza.es/ciudad/artepublico/detalle_ArtePublico?id=145";
	// Enviamos la respuesta
	int send_bytes = socket.Send(client_fd, resp);
	if(send_bytes == -1) {
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		exit(1);
	}
	
	/*Segundo mensaje
	******************************************************/
	// Recibimos el mensaje del cliente
	rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
	if(rcv_bytes == -1) {
		cerr << "Error al recibir datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		socket.Close(socket_fd);
	}
	if (buffer=="Rest"){
		string coordx="41.6834";
		string coordy="-0.8874";
		resp=coordx+" "+coordy;
	}
	else {
		resp="Precio: 55";
		send_bytes = socket.Send(client_fd, resp);
		if(send_bytes == -1) {
			cerr << "Error al enviar datos: " << strerror(errno) << endl;
			// Cerramos los sockets
			socket.Close(client_fd);
			exit(1);
		}
		error_code = socket.Close(client_fd);
   		if(error_code == -1){
    			cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
    		}

    		// Cerramos el socket del servidor
    		error_code = socket.Close(socket_fd);
   		if(error_code == -1){
    			cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    		}
	}
	send_bytes = socket.Send(client_fd, resp);
	if(send_bytes == -1) {
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		exit(1);
	}
	
	/*Tercer mensaje
	******************************************************/
	// Recibimos el mensaje del cliente
	rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
	if(rcv_bytes == -1) {
		cerr << "Error al recibir datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		socket.Close(socket_fd);
	}
	resp="Precio: 55";
	// Enviamos la respuesta
	send_bytes = socket.Send(client_fd, resp);
	if(send_bytes == -1) {
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		exit(1);
	}
	// Cerramos el socket del cliente
   	error_code = socket.Close(client_fd);
   	if(error_code == -1){
    		cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
   	}
   	return error_code;
}

//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	//Número de threads que operan a la vez y por tanto número máximo de clientes operando simultáneamente
	thread P[10];
	int i=0;
	// Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(argv[1]);
	// CreaciÃ³n del socket con el que se llevarÃ¡ a cabo
	// la comunicaciÃ³n con el servidor.
	Socket socket(SERVER_PORT);
	// Bind 
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		cerr << "Error en el bind: " << strerror(errno) << endl;
		exit(1);
	}
	// Listen
    int max_connections = 1;
	int error_code = socket.Listen(max_connections);
	if(error_code == -1) {
		cerr << "Error en el listen: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}
	//Prueba para tratar con un número limitado de clientes simultáneamente
	while(i<10){
		// Accept
		int client_fd = socket.Accept();
		if(client_fd == -1) {
			cerr << "Error en el accept: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		P[i]=thread(&auxiliar, client_fd, socket_fd, error_code, ref(socket));
		i++;
	}
    socket.Close(socket_fd);
    return error_code;
}
