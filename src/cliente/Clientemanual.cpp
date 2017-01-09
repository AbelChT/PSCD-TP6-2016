/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como un cliente que realiza su
        ejecución de forma manual, es el operador el que introduce
        los mensajes que desea enviar al servidor
******************************************************************/
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.h"
#include <stdlib.h>

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)

//-------------------------------------------------------------
// Trocea y formatea el mensaje
int troceaFormatea(string message, string p[]) {
    int j = -1;
    int cont = 0;
    int aux;
    for (int i = 0; i < message.length(); i++) {
        if (message[i] == ' ') {
            aux = j + 1;
            j = i;
            p[cont] = message.substr(aux, j - aux);
            cont++;
        } else if (i == (message.length()) - 1) {
            aux = j + 1;
            j = i + 1;
            p[cont] = message.substr(aux, j - aux);
        }
    }
    return cont-1;
}



int enviarMensaje (int socket_fd, string mensaje, Socket socket){
    int send_bytes = socket.Send(socket_fd, mensaje);
    if(send_bytes == -1){
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }
}

int recibirMensaje (int socket_fd, string &respuesta, Socket socket){
    int read_bytes = socket.Recv(socket_fd, respuesta, MESSAGE_SIZE);
    if(read_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(socket_fd);
	return -1;
    }
}

bool es_numero(string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//Pre: argv[1] se corresponde con la ip y argv[2] con el puerto correspondientes al Servicio
//     argv[3] es un número entero único correspondiente al id del cliente
int main(int argc, char *argv[]) {
      srand(time(NULL));
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
      if(socket_fd == -1) return -1;

      //Mensaje que se enviará al servidor
      string mensaje;

      //String auxiliar para la generación del mensaje
      string mensaje_aux;

      //Buffer en el que se almacena la respuesta del servidor
      string buffer;

      //Array en el que almacenaremos la respuesta formateada del servidor
		  string p[5];

      //Número de parámetros de búsqueda enviados al servidor
      int nParametros;

      //Número de monumentos recibidos como respuesta
      int nMon;

      //Valor auxiliar que utilizaremos para elegir una respuesta de forma aleatoria

	//Comprobamos que se acepta el servicio
      int msgFin;

      //Enviamos la id del cliente
	    mensaje = argv[3];
	    if(enviarMensaje (socket_fd, mensaje, socket)==-1) return -1;
	    if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
	    if (buffer=="Servicio denegado") {
            cout << buffer << endl;
            return 1;
      }
	while (1){
/*Solicitud inicial
**********************************************************************/
                // Leer mensaje de la entrada estandar
                cout << "Inserte hasta cinco parametros de busqueda: ";
                getline(cin, mensaje);
		string strings_aux[10]; //Checkear si entran +10 hay problemas
		int comprobar_tamanyo=troceaFormatea(mensaje,strings_aux);
		while (comprobar_tamanyo<1||comprobar_tamanyo>5){
			cout << "Informacion no valida" << endl;
           		cout << "Inserte hasta cinco parametros de busqueda: ";
                	getline(cin, mensaje);
		}
                if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
                if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
            	if (buffer=="Nada encontrado"){
			 cout << buffer << endl;
			 continue;
		}
		if (mensaje==MENS_FIN){
			cout << buffer << endl;
                	socket.Close(socket_fd);
                	return 0;
       		}
           	else {
			  string p[5];
			  int nMon;
			  troceaFormatea(buffer, p);
        		  for (int n=0; n<5; ++n){
		          	if (p[n].length()!=0){
					nMon++;
					cout << p[n] << endl;
                			string ABRIRURL="gnome-open "+ p[n];
                			system((ABRIRURL).data());
					//sleep(5); //Evitar que Xming se sature
              			}
        		  }
		}

/*Fin o restaurante
*************************************************************************/
                cout << "Finalizar ('Fin') o mostrar restaurante ('N�mero del restaurante [1-5]'): ";
                getline(cin, mensaje);
		char *msgaux = strdup(mensaje.c_str());
	       while(atoi(msgaux)<1||atoi(msgaux)>nMon||!es_numero(mensaje)){//La respuesta es no coherente
            		cout << "Informacion no valida" << endl;
           		cout << "Finalizar ('Fin') o mostrar restaurante ('N�mero del restaurante [1-5]'): " << endl;
                	getline(cin, mensaje);
		}
                if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
                if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
                if(mensaje==MENS_FIN) {
                    cout << buffer << endl;
                    socket.Close(socket_fd);
                    return 0;
                }
               else {
            string coord[2];
            troceaFormatea(buffer, coord);
		string cmd("firefox https://www.google.com/maps/place/"+coord[0]+","+coord[1]);
		//"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
		int resCall = system(cmd.c_str());
		if(resCall != 0){
		cerr << "Ha habido alg�n problema al abrir el navegador" << endl;
		return 1;
		}
            //string ABRIRURL="gnome-open https://www.google.com/maps/place/"+coord[0]+","+coord[1];
            //system((ABRIRURL).data());
          }

/*Finalización o realizar otro pedido
**************************************************************************/
                cout << "Finalizar ('Fin') o continuar (Inserte nuevos parametros): " << endl;
                }
}
