#include <iostream>
#include <thread>
#include "../../../src/librerias/Socket/Socket.h"
#include "../../../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje (MODIFICABLE)
int Nthreads = 0; //Número de threads activos
bool loop = true; //Control de finalización

//-------------------------------------------------------------
// Trocea y formatea el mensaje
//Para pruebas
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


void enviarMensaje(int client_fd, string &resp, Socket socket) {
    int send_bytes = socket.Send(client_fd, resp);
    if (send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(client_fd);
        Nthreads--;
        exit(1);
    }
}

void recibirMensaje(int client_fd, string &buffer, Socket socket) {
    int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
    if (rcv_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(client_fd);
        Nthreads--;
        exit(1);
    }

}


//---------------------------------------------------------------
//Función que utilizan los thread

void atender_cliente(Socket &socket, int client_fd,DbMonumentosRestaurantes& mon_rest) {
    Lista<Monumento> listm;
    Lista<Restaurante> listr;
    char MENS_FIN[] = "Fin";

    int length = 100;

    bool out = false; // Inicialmente no salir del bucle
    string buffer;
    string respuesta;
    while (!out) {
/*Primer mensaje
******************************************************/
        int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
        if (rcv_bytes == -1) {
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            out = true;
        }
        cout << "Mensaje recibido: '" << buffer << "'" << endl;
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } else {
            //Pruebas análisis de mensaje y respuesta en consecuencia
            string p[5];
            troceaFormatea(buffer, p);


            111//Aqui se almacena el numero de elementos que te han pasado para buscar
            int num_arg_busc;//supongo que ya lo he obtenido


            int n=0;
            bool no_lleno=true;
            listm.clear();

            while (n < num_arg_busc&&no_lleno){
                if(listm.size()<5){
                    Lista<Monumento> listm_aux;
                    mon_rest.buscarMonumento(p[n], listm_aux);
                    Monumento mon_aux;
                    listm_aux.begin();
                    while(listm_aux.next(mon_aux)&&listm.size<=5){
                        if (!listm.belongs(mon_aux)){
                          listm.add(mon_aux);
                        }
                    }
                    111//Comporbar si la suma de los elementos que estan
                    //en listm_aux y no estan en listm es mayor que 5
                    // Si no lo es juntarlos
                    //en caso contrario juntar hasta 5 y poner no lleno como false


                } else{
                    no_lleno= false;
                }
                ++n ;
            }
            listm.begin()
            while(listm.next(monumento_actual)){
              monumento_actual.link;
            }

            111 // A partir de aqui has de tranformar la lista a string y pasarla
            // La parte de restaurantes es igual
            // Cada vez que se recibe una respuesra hay que comprobar si es fin , si es fin hay que poner out a true
            // tambien puedes usar breack para salir del bucle

            for (int n = 0; n < 5; ++n) {
                if (p[n].length() != 0) {
                    mon_rest.buscarMonumento(p[n], listm);
                }
            }

            while (p[0] == "Denegar") {
                respuesta = "Servicio denegado";
                enviarMensaje(client_fd, resp, socket);
                recibirMensaje(client_fd, buffer, socket);
                troceaFormatea(buffer, p);
            }
            if (p[0] == "Nada") {
                respuesta = "Nada encontrado";
                enviarMensaje(client_fd, resp, socket);
                Nthreads--;
                return 0;
            } else {
                resp = "http://www.zaragoza.es/ciudad/artepublico/detalle_ArtePublico?id=145";
                enviarMensaje(client_fd, resp, socket);

/*Segundo mensaje
******************************************************/
                recibirMensaje(client_fd, buffer, socket);
                //Devuelve 0-4 (índice) del monumento?
                if (buffer == "Rest") {
                    prueba.buscarRestaurante(Monumento
                    monumento_seleccionado, listr);
                    string coordx = "41.6834";
                    string coordy = "-0.8874";
                    resp = coordx + " " + coordy;
                } else {
                    //Generar precio
                    resp = "Precio: 55";
                    enviarMensaje(client_fd, resp, socket);
                    Nthreads--;
                    return 0;
                }
                enviarMensaje(client_fd, resp, socket);

/*Tercer mensaje
******************************************************/
                recibirMensaje(client_fd, buffer, socket);
                if (buffer == MENS_FIN) {
                    //Generar precio
                    resp = "Precio: 55";
                    enviarMensaje(client_fd, resp, socket);
                    Nthreads--;
                    return 0;
                } else {
                    resp = "Reiniciando representante";
                    cout << resp << endl;
                    enviarMensaje(client_fd, resp, socket);
                }
            }
        }
    }
}

//ALERTA ESPERA ACTIVA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void controlar() {
    while (Nthreads != 0) sleep(5);
}

void finalizar() {
    string finalizar;
    getline(cin, finalizar);
    cout << "Cerrando servidores..." << endl;
    loop = false;
}

//-------------------------------------------------------------
int main(int argc, char *argv[]) {
    DbMonumentosRestaurantes db_monumentos_restaurantes(5, 1);//cargamos los datos de los monumentos y restaurantes
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(argv[1]);
    // CreaciÃ³n del socket con el que se llevarÃ¡ a cabo
    // la comunicaciÃ³n con el servidor.
    Socket socket(SERVER_PORT);
    // Bind
    int socket_fd = socket.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " << strerror(errno) << endl;
        exit(1);
    }
    // Listen
    int max_connections = 100;
    int error_code = socket.Listen(max_connections);
    if (error_code == -1) {
        cerr << "Error en el listen: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }

    //Controla el final del programa
    thread finalizador(&finalizar);

    while (loop) {
        // Accept
        int client_fd = socket.Accept();
        if (client_fd == -1) {
            cerr << "Error en el accept: " << strerror(errno) << endl;
            // Cerramos el socket
            socket.Close(socket_fd);
        }
        thread(atender_cliente, ref(socket), client_fd).detach();
    }

    finalizador.join();

    //Llamar a funcion de jorge
    socket.Close(socket_fd);
    return error_code;
}
