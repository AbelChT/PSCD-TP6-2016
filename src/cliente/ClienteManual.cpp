#include <iostream>
#include <chrono>
#include <thread>
#include "../librerias/Socket/Socket.h"
#include <stdlib.h>

using namespace std;

const int MESSAGE_SIZE = 4001; //TamaÃ±o mÃ¡ximo del mensaje (MODIFICABLE)

//-------------------------------------------------------------
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

//Pre: argv[1] se corresponde con la ip y argv[2] con el puerto correspondientes al Servicio
//     argv[3] es un nÃºmero entero Ãºnico correspondiente al id del cliente
int main(int argc, char *argv[]) {
      srand(time(NULL));
      const string MENS_FIN("Fin");
      // DirecciÃƒÂ³n y nÃƒÂºmero donde escucha el proceso servidor
      string SERVER_ADDRESS = argv[1];
      int SERVER_PORT = atoi(argv[2]);
      // CreaciÃ³n del socket con el que se llevarÃ¡ a cabo
      // la comunicaciÃ³n con el servidor.
      Socket socket(SERVER_ADDRESS, SERVER_PORT);
      // Conectamos con el servidor. Probamos varias conexiones
      const int MAX_ATTEMPS = 10;
      int count = 0;
      int socket_fd;
      do {
          // ConexiÃƒÂ³n con el servidor
          socket_fd = socket.Connect();
          count++;
          // Si error --> esperamos 1 segundo para reconectar
          if(socket_fd == -1){
              this_thread::sleep_for(chrono::seconds(1));
          }
      } while(socket_fd == -1 && count < MAX_ATTEMPS);

      // Chequeamos si se ha realizado la conexiÃƒÂ³n
      if(socket_fd == -1) return -1;

      //Mensaje que se enviarÃ¡ al servidor
      string mensaje;

      //String auxiliar para la generaciÃ³n del mensaje
      string mensaje_aux;

      //Buffer en el que se almacena la respuesta del servidor
      string buffer;

      //Array en el que almacenaremos la respuesta formateada del servidor
		  string p[5];

      //NÃºmero de parÃ¡metros de bÃºsqueda enviados al servidor
      int nParametros;

      //NÃºmero de monumentos recibidos como respuesta
      int nMon;

      //Valor auxiliar que utilizaremos para elegir una respuesta de forma aleatoria

	//Comprobamos que se acepta el servicio
      int msgFin;

      //Enviamos la id del cliente
	    mensaje = argv[3];
	    if(enviarMensaje (socket_fd, mensaje, socket)==-1) return -1;
	    cout << "Id enviada:" << argv[3] << endl;
	    if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
	    cout << "Respuesta recibida:" << buffer << endl;
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
                			cout<<ABRIRURL<<endl;
                			//system((ABRIRURL).data());
					//sleep(5); //Evitar que Xming se sature
              			}
        		  }
		}

/*Fin o restaurante
*************************************************************************/
                cout << "Finalizar ('Fin') o mostrar restaurante ('Número del restaurante [1-5]'): " << endl;
                getline(cin, mensaje);
                if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
                if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
		while (mensaje=="Informacion incorrecta"){
			cout << buffer << endl;
			cout << "Finalizar ('Fin') o mostrar restaurante ('Número del restaurante [1-5]'): " << endl;
                	getline(cin, mensaje);
                	if (enviarMensaje(socket_fd, mensaje, socket)==-1) return -1;
                	if (recibirMensaje(socket_fd, buffer, socket)==-1) return -1;
		}
                if(mensaje==MENS_FIN) {
                    cout << buffer << endl;
                    socket.Close(socket_fd);
                    return 0;
                }
               else {
            string coord[2];
            troceaFormatea(buffer, coord);
		string cmd("firefox https://www.google.com/maps/place/"+coord[0]+","+coord[1]);

		cout<<cmd<<endl;
		//"system" requiere un "char *", que es lo que nos da el operador "c_str()" de la clase string de C++
		//int resCall = system(cmd.c_str());
		/*
		if(resCall != 0){
		cerr << "Ha habido algún problema al abrir el navegador" << endl;
		return 1;
		}*/
            //string ABRIRURL="gnome-open https://www.google.com/maps/place/"+coord[0]+","+coord[1];
            //system((ABRIRURL).data());
          }

/*FinalizaciÃ³n o realizar otro pedido
**************************************************************************/
                cout << "Finalizar ('Fin') o continuar (Inserte nuevos parametros): " << endl;
                }
}