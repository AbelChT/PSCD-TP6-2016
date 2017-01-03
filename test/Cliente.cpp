#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)



void gen_random(string &msg) {
    srand(time(NULL));
    int len=3;
    char s[len];
    static const char alpha[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alpha[rand() % (sizeof(alpha) - 1)];
    }
    s[len] = 0;
    msg=s;
}


//-------------------------------------------------------------
// Obtiene coordenadas
//Para pruebas
void obtenerCoord(string message, string p[]) {
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



void enviarMensaje (int socket_fd, string mensaje, Socket socket){
    int send_bytes = socket.Send(socket_fd, mensaje);
    if(send_bytes == -1){
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        exit(1);
    }
}

void recibirMensaje (int socket_fd, string &respuesta, Socket socket){
    int read_bytes = socket.Recv(socket_fd, respuesta, MESSAGE_SIZE);
    if(read_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(socket_fd);
    }
}

int main(int argc, char *argv[]) {
    while(1){
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
        string mensaje, buffer;
        while (mensaje!=MENS_FIN){
/*Solicitud inicial
**********************************************************************/
                // Leer mensaje de la entrada estandar
                cout << "Inserte hasta cinco parametros de busqueda: ";
                getline(cin, mensaje);
                //gen_random(mensaje);
                //Comprobar el mensaje a enviar
                //cout << mensaje << endl;
                enviarMensaje(socket_fd, mensaje, socket);
                recibirMensaje(socket_fd, buffer, socket);
                while (buffer=="Servicio denegado") {
                    cout << buffer << endl;
                    cout << "Inserte hasta cinco parametros de busqueda: ";
                    getline(cin, mensaje);
                    sleep(5);
                    enviarMensaje(socket_fd, mensaje, socket);
                    recibirMensaje(socket_fd, buffer, socket);
                }
            if (buffer=="Nada encontrado") cout << buffer << endl;
            else {
                string ABRIRURL="gnome-open "+ buffer;
                system((ABRIRURL).data());
                
/*Fin o restaurante
*************************************************************************/
                cout << "Finalizar ('Fin') o mostrar restaurante ('Rest'): " << endl;
                getline(cin, mensaje);
                enviarMensaje(socket_fd, mensaje, socket);
                recibirMensaje(socket_fd, buffer, socket);
                if(mensaje==MENS_FIN) {
                    cout << buffer << endl;
                    int error_code = socket.Close(socket_fd);
                    if(error_code == -1){
                        cerr << "Error cerrando el socket: " << strerror(errno) << endl;
                    }
                    return error_code;
                }
                else {
                    string coord[2];
                    obtenerCoord(buffer, coord);
                    string ABRIRURL="gnome-open https://www.google.com/maps/place/"+coord[0]+","+coord[1];
                    system((ABRIRURL).data());
                }
                
/*Finalización o realizar otro pedida
**************************************************************************/
                cout << "Finalizar ('Fin') o continuar ('Otro'): " << endl;
                getline(cin, mensaje);
                enviarMensaje(socket_fd, mensaje, socket);
                recibirMensaje(socket_fd, buffer, socket);
                if (mensaje=="Fin") cout << buffer << endl;
			}
		}
                // Cerramos el socket
                cout << "Cerrando socket" << endl;
                int error_code = socket.Close(socket_fd);
                if(error_code == -1){
                    cerr << "Error cerrando el socket: " << strerror(errno) << endl;
                }
            }
	//return error_code;
}

