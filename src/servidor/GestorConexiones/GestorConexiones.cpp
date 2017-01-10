/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como el servidor del programa
******************************************************************/
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

#include "../../librerias/Lista/Lista.h"
#include "../../librerias/Monumento/Monumento.h"
#include "../../librerias/Socket/Socket.h"
#include "../../servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"
#include "../../servidor/GestorPrecios/GestorPrecios.h"
#include "../../servidor/DbSesion/DbSesion.h"
#include "../../librerias/UTM2LL/LatLong-UTMconversion.h"

#include "../../librerias/Semaphore/Semaphore.h"


using namespace std;

// Tama�o m�ximo del mensaje
const int MESSAGE_SIZE = 4001;

// Numero de representantes activos
int representantes_activos = 0;

// Mensaje usado para marcar el fin de la comunicacion entre cliente
// y servidor
string MENS_FIN = "Fin";

// Numero de monumentos a devolver al cliente
const int NUM_MONUMENTOS_DEVOLVER = 5;

// Control de finalizai�n del servicio
atomic_bool end_service;

// Semaforo que controla el final sel servicio
Semaphore fin(0);

// Mutex usado para tener el control de los representantes y la
// sincronizacion final
mutex mtx_end;

// Mutex usado para controlar la pantalla
mutex mtx_pantalla;

// Numero total de clientes atendidos
int numero_total_clientes;


// Factura total
double factura_total;


// Gestor de precios, inicializado para cobrar 2 euros por
// establecimiento de servicio y euro y medio por petici�n
GestorPrecios gestor_precios(1.5, 2);


/*
* Funcion que simula a un representante
*/
void representante(Socket &socket, int client_fd, DbMonumentosRestaurantes &db_monumentos_restaurantes, DbSesion& db_sesion) {

    mtx_end.lock();

    string buffer,// Almaceno los mensajes del cliente
        respuesta;// Almaceno la respuesta que dare al cliente

    // Controlo que hay un nuevo cliente
    if (end_service.load()) {
        string den = "Servicio denegado";

        //Limpio buffer entrada
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            mtx_pantalla.lock();
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            socket.Close(client_fd);
        }

        if (socket.Send(client_fd, den) < 0) {//Caso en el que falle el envio del mensaje
            socket.Close(client_fd);
            mtx_pantalla.lock();
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
        }
        /*
                if (representantes_activos == 0) {
                    fin.signal();
                }
        */
        socket.Close(client_fd);

        mtx_end.unlock();//Desbloqueo el mutex para que otro cliente pueda finalizar
        return;//Salimos
    }
    else {
        // Se puede atender al cliente
        ++numero_total_clientes;
        ++representantes_activos;//Aumento el numero de representantes activos
    }

    mtx_end.unlock();//Finalizamos registro del cliente


    // Recibo primer mensaje con id del cliente
    if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {

        mtx_pantalla.lock();
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        mtx_pantalla.unlock();

        socket.Close(client_fd);
        mtx_end.lock();
        --representantes_activos;
        if (representantes_activos == 0 && end_service.load()) {
            fin.signal();
        }
        mtx_end.unlock();

        return;//Salimos del programa
    }

    string acept = "Servicio aceptado";
    if (socket.Send(client_fd, acept) < 0) {//Caso en el que falle el envio del mensaje
        mtx_pantalla.lock();
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        mtx_pantalla.unlock();
        socket.Close(client_fd);
        mtx_end.lock();
        --representantes_activos;
        if (representantes_activos == 0 && end_service.load()) {
            fin.signal();
        }
        mtx_end.unlock();

        return;//Salimos del programa
    }


    // Almacenar en id_cliente el id del cliente
    int id_cliente = stoi(buffer);

    //Crea un nuevo cliente
    db_sesion.nuevoCliente(id_cliente);

    while (true) {//itero mientras el cliente envie peticiones

        //Recibo busqueda de monumentos
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            mtx_pantalla.lock();
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            break;//Salgo del bucle
        }

        if (buffer == MENS_FIN) {//El cliente ha decidido finalizar la comunicaci�n
            break;//Salimos del bucle
        }

        //No se ha decidido finalizar la comunicaci�n
        //A�ado el pedido a la base de datos
        db_sesion.nuevoPedido(id_cliente, "Buscar monumento " + buffer);

        //Monumentos para enviar al cliente

        Lista<Monumento *> enviar_al_cliente;
        db_monumentos_restaurantes.buscarMonumento(buffer, enviar_al_cliente);

        //Auxiliar para obtener datos de cierto monumento
        Monumento * mon_aux;

        if (enviar_al_cliente.size() == 0) {
            respuesta = "Nada encontrado";
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                mtx_pantalla.lock();
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                mtx_pantalla.unlock();
                break;
            }
            continue;//Vuelvo al inicio
        }
        else {
            enviar_al_cliente.begin();
            enviar_al_cliente.next(mon_aux);
            respuesta = mon_aux->getlink();
            while (enviar_al_cliente.next(mon_aux)) {
                respuesta = respuesta + " " + mon_aux->getlink();
            }
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                mtx_pantalla.lock();
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                mtx_pantalla.unlock();
                break;
            }
        }

        //Recibo segundo mensaje
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            mtx_pantalla.lock();
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            break;//Salgo del bucle
        }
        if (buffer == MENS_FIN) {//El cliente ha decidido finalizar la comunicaci�n
            break;
        }
        //Se ha tenido respuesta
        db_sesion.nuevoPedido(id_cliente, "Buscar restaurante " + buffer);

        int indice_monumento_seleccionado = stoi(buffer);

        double monumento_UTMNorthing, monumento_UTMEasting;

        enviar_al_cliente.begin();

        for (int i = 0; i < indice_monumento_seleccionado - 1; ++i) {//Avanzo el iterador
            enviar_al_cliente.next(mon_aux);
        }

        enviar_al_cliente.next(mon_aux);
        mon_aux->getcoordenadas(monumento_UTMNorthing, monumento_UTMEasting);



        double restaurante_UTMNorthing, restaurante_UTMEasting;

        db_monumentos_restaurantes.buscarRestaurante(monumento_UTMNorthing, monumento_UTMEasting, restaurante_UTMNorthing, restaurante_UTMEasting);//suponemos que siempre se carga bien

        //Convertimos UTM a Lat, Long
        int RefEllipsoid = 23;

        double Lat, Long;

        char UTMZoneZgza[4] = "30T";

        UTMtoLL(RefEllipsoid, restaurante_UTMNorthing, restaurante_UTMEasting, UTMZoneZgza, Lat, Long);

        respuesta = to_string(Lat) + " " + to_string(Long);

        if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje

            mtx_pantalla.lock();
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            break;
        }
    }

    //Coste del servicio
    double precio_local = gestor_precios.precio(db_sesion.numeroPeticionesCliente(id_cliente));

    respuesta = to_string(precio_local);

    //Envio al cliente el precio por el servicio
    if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
        mtx_pantalla.lock();
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        mtx_pantalla.unlock();
    }

    //Obtengo la información respectiva a este cliente
    string informacion_cliente;
    db_sesion.listarCliente(id_cliente, informacion_cliente);

    //Muestro informacion sobre el cliente
    mtx_pantalla.lock();
    cout << informacion_cliente << endl;
    mtx_pantalla.unlock();

    db_sesion.borrarCliente(id_cliente);

    //Cierro socket cliente
    int error_code = socket.Close(client_fd);

    if (error_code == -1) {
        mtx_pantalla.lock();
        cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
        mtx_pantalla.unlock();
    }

    //Controlo fin del representante
    mtx_end.lock();
    factura_total = factura_total + precio_local;
    --representantes_activos;

    if (representantes_activos == 0 && end_service.load()) {
        fin.signal();
    }
    mtx_end.unlock();
}


/*
* Funcion utilizada para dar una correcta finalización al programa
*/
void control_fin(int socket_fd, Socket &socket) {
    string finalizar;
    cin >> finalizar;//Esperamis hasta que el suario escriba algo por pantalla

    mtx_end.lock();
    end_service.store(true);//Marcamos fin de servicio

    if (representantes_activos == 0) {//No hay representantes activos
        fin.signal();//Saltamos el wait que hay debajo
    }
    mtx_end.unlock();

    fin.wait();//Espero hasta que no existan representantes activos

    //Cerramos el socket para si se ha quedado esperando el accept falle
    if (socket.Close(socket_fd) == -1) {
        cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    }
}


//Pre: argv[1] se corresponde con el puerto donde escucha el proceso servidor
//Post: lanza el servidor con sus correspondientes módulos, funciones, etc

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Numero de argumentos incorrecto" << endl;
        return -1;
    }
    /* Creamos la sesion*/
    cout << "Creando sesion...." << endl;
    // Base de datos de clientes atendidos actualmente
    DbSesion s;

    cout << "Sesion creada" << endl;


    /* Creamos y cargamos la base de datos*/
    cout << "Cargando base de datos...." << endl;

    // Base de datos donde se guardan los monumentos y restaurantes
    // La inicializamos para que devuelva un maximo de 5 monumentos
    DbMonumentosRestaurantes db_monumentos_restaurantes(NUM_MONUMENTOS_DEVOLVER);//cargamos los datos de los monumentos y restaurantes

    cout << "Base de datos cargada" << endl;

    //Marcamos el fin de servicio como falso
    end_service.store(false);

    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = atoi(argv[1]);
    // Creacion del socket con el que se llevara a cabo
    // la comunicacion con el servidor.
    Socket socket(SERVER_PORT);

    // Bind
    int socket_fd = socket.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " << strerror(errno) << endl;
        return -1;
    }

    // Listen
    int max_connections = 100;//Numero de conexiones suficientemente grande

    int error_code = socket.Listen(max_connections);

    if (error_code == -1) {
        cerr << "Error en el listen: " << strerror(errno) << endl;

        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }

    // Controla el final del programa
    thread finalizador(control_fin, socket_fd, ref(socket));

    //Mientras no se solicite la finalización
    while (true) {
        // Accept
        int client_fd = socket.Accept();
        if (client_fd < 0) {
            if (!end_service.load()) {
                cerr << "Error en el accept: " << strerror(errno) << endl;
            }
            break;//Si falla el accept salimos del bucle

        }
        else {//Se ha realizado el accept con exito
            thread(representante, ref(socket), client_fd, ref(db_monumentos_restaurantes), ref(s)).detach();
        }
    }

    //Salimos del bucle
    mtx_end.lock();
    if (representantes_activos == 0) {
        fin.signal();//Dejamos finalizar el programa
    }
    mtx_end.unlock();

    finalizador.join();//En este momento ya han terminado todos los threads y se ha cerrado el socket


    cout << "Numero total de clientes: " << numero_total_clientes << endl;

    cout << "Total recaudado: " << factura_total << endl;
    // Mensaje de despedida
    cout << "Bye bye" << endl;
    return 0;
}
