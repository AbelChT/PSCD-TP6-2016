/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como un cliente que realiza su
        ejecución de forma automática sin necesidad de interacción
        con el operador
******************************************************************/
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.h"
#include <stdlib.h>

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)


//Generacion aleatoria de los parametros de busqueda que son strings
//formados por una consonante seguida de una vocal y otra consonante

void gen_random(string &msg) {
    srand(time(NULL));
    int len=4;
    char s[len];
    static const char cons[] =
            "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
    static const char voc[] =
            "AEIOUaeiou";
    s[0] = cons[rand() % (sizeof(cons) - 1)];
    s[1] = voc[rand() % (sizeof(cons) - 1)];
    s[2] = cons[rand() % (sizeof(cons) - 1)];
    s[len] = 0;
    msg=s;
}


// Trocea y formatea el mensaje

void troceaFormatea(string message, string p[]) {
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
}


//Funcion para enviar el mensaje al servidor
//Cierra el socket y devuelve -1 en caso de error

int enviarMensaje (int socket_fd, string mensaje, Socket socket){
    int send_bytes = socket.Send(socket_fd, mensaje);
    if(send_bytes == -1){
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }
}


//Funcion para recibir el mensaje al servidor
//Cierra el socket y devuelve -1 en caso de error

int recibirMensaje (int socket_fd, string &respuesta, Socket socket){
    int read_bytes = socket.Recv(socket_fd, respuesta, MESSAGE_SIZE);
    if(read_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(socket_fd);
	       return -1;
    }
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
      int nMon=0;

      //Valor auxiliar que utilizaremos para elegir una respuesta de forma aleatoria
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
          nParametros=rand() % 5 +1;
		mensaje.clear();
	        for (int i=0; i<nParametros; ++i){
                gen_random(mensaje_aux);
		            mensaje=mensaje+ " " + mensaje_aux;
				sleep(1);
          }

          //PRUEBAS Comprobacion del mensaje a enviar PRUEBAS
          cout << "Mensaje enviado: " << mensaje << endl;

          if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
          if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;

          //Si no se encuentra un monumento se nos da la opción de seguir preguntando
		        while (buffer=="Nada encontrado"){
              			 cout << buffer << endl;
				sleep(3);
				mensaje.clear();
			          nParametros=rand() % 5 +1;
			          for (int i=0; i<nParametros; ++i){
               				 gen_random(mensaje_aux);
				             mensaje=mensaje+ " " + mensaje_aux;
						sleep(1);
			          }

       		      //PRUEBAS Comprobacion del mensaje a enviar PRUEBAS
         	      cout << "Mensaje enviado: " << mensaje << endl;
        	      if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
       		      if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
			}
            }
          //Si hemos enviado el mensaje de finalización recibiremos el precio que
          //pasaremos a mostrar por pantalla
			    if (mensaje==MENS_FIN){
				        cout << "Precio del servicio: " << buffer << endl;
                socket.Close(socket_fd);
                return 0;
       		}

          //En otro caso habremos recibido las url de los monumentos, las mostramos por
          //pantalla y abrimos una pestaña por cada una
          else {
		          troceaFormatea(buffer, p);
        	     for (int n=0; n<5; ++n){
		               if (p[n].length()!=0){
				                nMon++;
				                cout << p[n] << endl;
                		    string ABRIRURL="gnome-open "+ buffer;
                		    system((ABRIRURL).data());
					sleep(2);
              		}
        	      }
	        }


/*Fin o restaurante
*************************************************************************/
	mensaje.clear();
          //Enviamos "Fin" o escogemos un monumento sobre el cual solicitar el restaurante
          msgFin = rand() % 4;
          if (msgFin==0) mensaje="Fin";
          else mensaje= to_string (rand() % nMon); //MON ALEATORIO
          cout << "Mensaje enviado: " << mensaje << endl;
          if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
          if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
          if(mensaje==MENS_FIN) {
            cout << buffer << endl;
            socket.Close(socket_fd);
            return 0;
          }

          //Si no hemos enviado el mensaje de finalización recibiremos las coordenadas
          //del restaurante más próximo al monumento seleccionado, abriremos una pestaña
          //el mapa en google centrado en las coordenadas recibidas

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
		mensaje.clear();
	       msgFin = rand() % 2;
		     if (msgFin==0) mensaje="Fin";
		     else{
			        nParametros=rand() % 5 +1;
			        for (int i=0; i<nParametros; ++i){
                		gen_random(mensaje_aux);
				            mensaje=mensaje+ " " + mensaje_aux;
			        }
		      }
		      cout << "Mensaje enviado: " << mensaje << endl; //Comprobacion del substring
          if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
          if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
          if (mensaje==MENS_FIN){
			         cout << buffer << endl;
			            socket.Close(socket_fd);
			               return 0;
		      }
    }
