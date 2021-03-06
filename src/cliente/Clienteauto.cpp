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
#include "../librerias/Socket/Socket.h"
#include <stdlib.h>

using namespace std;

const int MESSAGE_SIZE = 4001; //Tamaño máximo del mensaje


// Generación aleatoria de los parámetros de búsqueda

void gen_random(string &msg) {
    string parametros[17] = { "goya","19","monumento","escultura",
        "catedral","20","palacio","iglesia","plaza","museo",
        "teatro","parque","puente","ag","3","5","toros" };
    srand(time(NULL));
    msg = parametros[rand() % (sizeof(parametros) / sizeof(parametros[0]))];
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
        }
        else if (i == (message.length()) - 1) {
            aux = j + 1;
            j = i + 1;
            p[cont] = message.substr(aux, j - aux);
        }
    }
}


//Funcion para enviar el mensaje al servidor
//Cierra el socket y devuelve -1 en caso de error

int enviarMensaje(int socket_fd, string mensaje, Socket socket) {
    int send_bytes = socket.Send(socket_fd, mensaje);
    if (send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }
}


//Funcion para recibir el mensaje al servidor
//Cierra el socket y devuelve -1 en caso de error

int recibirMensaje(int socket_fd, string &respuesta, Socket socket) {
    int read_bytes = socket.Recv(socket_fd, respuesta, MESSAGE_SIZE);
    if (read_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(socket_fd);
        return -1;
    }
}


//Pre: argv[1] se corresponde con la ip y argv[2] con el puerto correspondientes al Servicio
//     argv[3] es un número entero único correspondiente al id del cliente
//Post: ejecuta un cliente que gestiona automaticamente la creación de los mensajes
int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Numero de argumentos incorrecto" << endl;
        return -1;
    }
    srand(time(NULL));
    const string MENS_FIN("Fin");
    // Dirección y número donde escucha el proceso servidor
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
        // Conexión con el servidor
        socket_fd = socket.Connect();
        count++;
        // Si error --> esperamos 1 segundo para reconectar
        if (socket_fd == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while (socket_fd == -1 && count < MAX_ATTEMPS);

    // Comprobamos si se ha realizado la conexión
    if (socket_fd == -1) return -1;

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
    int nMon = 0;

    //Valor auxiliar que utilizaremos para elegir una respuesta de forma aleatoria
    int msgFin;

    //Comprobamos que se acepta el servicio: enviamos la id del cliente
    mensaje = argv[3];
    if (enviarMensaje(socket_fd, mensaje, socket) == -1) return -1;
    if (recibirMensaje(socket_fd, buffer, socket) == -1) return -1;
    if (buffer == "Servicio denegado") {
        cout << buffer << endl;
        return 1;
    }

    //Bucle del que saldremos cuando haya un problema o finalice el servicio
    while (1) {

        /*Solicitud inicial
        **********************************************************************/
        mensaje.clear();
        msgFin = rand() % 10;
        if (msgFin == 0) mensaje = "Fin";
        else {
            nParametros = (rand() % 5) + 1;
            for (int i = 0; i < nParametros; ++i) {
                gen_random(mensaje_aux);
                mensaje = mensaje + " " + mensaje_aux;
                sleep(1);
            }
        }
        //Mostramos que mensaje vamos a enviar
        cout << "Vamos a enviar: " << mensaje << endl;
        if (enviarMensaje(socket_fd, mensaje, socket) == -1) return -1;
        if (recibirMensaje(socket_fd, buffer, socket) == -1) return -1;
        //Si no se encuentra un monumento se nos da la opción de seguir preguntando
        while (buffer == "Nada encontrado") {
            cout << buffer << endl;
            mensaje.clear();
            nParametros = (rand() % 5) + 1;
            for (int i = 0; i < nParametros; ++i) {
                gen_random(mensaje_aux);
                mensaje = mensaje + " " + mensaje_aux;
                sleep(1);
            }
            cout << "Vamos a enviar: " << mensaje << endl;
            if (enviarMensaje(socket_fd, mensaje, socket) == -1) return -1;
            if (recibirMensaje(socket_fd, buffer, socket) == -1) return -1;
            sleep(3);
        }
        //Si hemos enviado el mensaje de finalización recibiremos el precio que
        //pasaremos a mostrar por pantalla
        if (mensaje == MENS_FIN) {
            cout << "Precio del servicio: " << buffer << endl;
            socket.Close(socket_fd);
            return 0;
        }
        nMon = 0;
        //En otro caso habremos recibido las url de los monumentos, las mostramos por
        // pantalla y abrimos una pestaña por cada una
        troceaFormatea(buffer, p);
        for (int n = 0; n < 5; ++n) {
            if (p[n].length() != 0) {
                nMon++;
                cout << p[n] << endl;
                string ABRIRURL = "gnome-open " + p[n];
                system((ABRIRURL).data());
                sleep(5); //Evitar que Xming se sature
            }
        }

        /*Fin o restaurante
        *************************************************************************/
        mensaje.clear();
        //Enviamos "Fin" o escogemos un monumento sobre el cual solicitar el restaurante
        msgFin = rand() % 4;
        if (msgFin == 0) mensaje = "Fin";
        else mensaje = to_string((rand() % nMon) + 1); //MON ALEATORIO
        cout << "Vamos a enviar: " << mensaje << endl;
        if (enviarMensaje(socket_fd, mensaje, socket) == -1) return -1;
        if (recibirMensaje(socket_fd, buffer, socket) == -1) return -1;
        if (mensaje == MENS_FIN) {
            cout << buffer << endl;
            socket.Close(socket_fd);
            return 0;
        }

        //Si no hemos enviado el mensaje de finalización recibiremos las coordenadas
        //del restaurante más próximo al monumento seleccionado, abriremos una pestaña
        //el mapa en google centrado en las coordenadas recibidas
        string coord[2];
        troceaFormatea(buffer, coord);
        string cmd("firefox https://www.google.com/maps/place/" + coord[0] + "," + coord[1]);
        int resCall = system(cmd.c_str());
        if (resCall != 0) {
            cerr << "Ha habido algún problema al abrir el navegador" << endl;
            return 1;
        }
    }
    cout << endl;
    sleep(10);
}
