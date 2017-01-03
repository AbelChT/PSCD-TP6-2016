#include <iostream>
#include "Socket.h"
#include <thread>
#include "../../../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"
using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)
int Nthreads=0; //Número de threads activos
bool loop=true; //Control de finalización
Lista<Monumento> listm;
Lista<Restaurante> listr;

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


void enviarMensaje (int client_fd, string &resp, Socket socket){
	int send_bytes = socket.Send(client_fd, resp);
	if(send_bytes == -1) {
		cerr << "Error al enviar datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		Nthreads--;
		exit(1);
	}
}

void recibirMensaje (int client_fd, string &buffer, Socket socket){
	int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
	if(rcv_bytes == -1) {
		cerr << "Error al recibir datos: " << strerror(errno) << endl;
		// Cerramos los sockets
		socket.Close(client_fd);
		Nthreads--;
		exit(1);
	}

}


//---------------------------------------------------------------
//Función que utilizan los thread

int auxiliar(int client_fd, int socket_fd, int error_code, Socket socket){
	char MENS_FIN[]="Fin";
	int length = 100;
	string buffer;
	string resp;
while (buffer!=MENS_FIN){
/*Primer mensaje
******************************************************/
	recibirMensaje (client_fd, buffer, socket);
	//Pruebas análisis de mensaje y respuesta en consecuencia
	string p[5];
	troceaFormatea(buffer, p);
	for (int n=0; n<5; ++n){
		 if (p[n].length()!=0){
			  prueba.buscarMonumento(p[n],listm);
		 }
	 }
	while (p[0]=="Denegar"){
		resp="Servicio denegado";
		enviarMensaje (client_fd, resp, socket);
		recibirMensaje (client_fd, buffer, socket);
		troceaFormatea(buffer, p);
	}
	if (p[0]=="Nada"){
		resp="Nada encontrado";
		enviarMensaje (client_fd, resp, socket);
		Nthreads--;
		return 0;
	}
	else{
	resp="http://www.zaragoza.es/ciudad/artepublico/detalle_ArtePublico?id=145";
	enviarMensaje (client_fd, resp, socket);

/*Segundo mensaje
******************************************************/
	recibirMensaje (client_fd, buffer, socket);
	//Devuelve 0-4 (índice) del monumento?
	if (buffer=="Rest"){
		prueba.buscarRestaurante(Monumento monumento_seleccionado, listr);
		string coordx="41.6834";
		string coordy="-0.8874";
		resp=coordx+" "+coordy;
	}
	else {
		//Generar precio
		resp="Precio: 55";
		enviarMensaje (client_fd, resp, socket);
		Nthreads--;
		return 0;
	}
	enviarMensaje (client_fd, resp, socket);

/*Tercer mensaje
******************************************************/
	recibirMensaje (client_fd, buffer, socket);
	if (buffer==MENS_FIN){
		//Generar precio
		resp="Precio: 55";
		enviarMensaje (client_fd, resp, socket);
		Nthreads--;
		return 0;
	}

	else{
		resp="Reiniciando representante";
		 cout << resp << endl;
		enviarMensaje (client_fd, resp, socket);
	}
}
}
}

//ALERTA ESPERA ACTIVA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void controlar(){
	while(Nthreads!=0) sleep(5);
}

void finalizar(){
	string finalizar;
	getline(cin, finalizar);
	cout << "Cerrando servidores..." << endl;
	loop=false;
}

//-------------------------------------------------------------
int main(int argc, char *argv[]) {
	DbMonumentosRestaurantes prueba(5,5);
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
	thread finalizador(&finalizar);
	while(loop){
		// Accept
		int client_fd = socket.Accept();
		if(client_fd == -1) {
			cerr << "Error en el accept: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			exit(1);
		}
		Nthreads++;
		thread operador(&auxiliar, client_fd, socket_fd, error_code, ref(socket));
    		operador.detach();
	}
    thread controlador(&controlar);
    finalizador.join();
    controlador.join();
    cout << "Recaudacion total: 1500000." << endl;
    cout << "Otros datos: ...." << endl;
    socket.Close(socket_fd);
    return error_code;
}
