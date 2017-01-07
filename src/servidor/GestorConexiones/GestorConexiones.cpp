#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

#include "../../../src/librerias/Socket/Socket.h"
#include "../../../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"
#include "../../../src/servidor/GestorPrecios/GestorPrecios.h"
#include "../../../src/servidor/DbSesion/DbSesion.h"

#include "../../../src/librerias/Semaphore/Semaphore.h"


using namespace std;

/*
 * Tamaño máximo del mensaje
 */
const int MESSAGE_SIZE = 4001;

/*
 * Numero de representantes activos
 */
int representantes_activos = 0;

/*
 * Mensaje usado para marcar el fin de la comunicacion entre cliente
 * y servidor
 */
 string MENS_FIN = "FIN";

/*
 * Numero de monumentos a devolver al cliente
 */
const int NUM_MONUMENTOS_DEVOLVER=5;

/*
 * Control de finalizaión del servicio
 */
atomic_bool end_service;

/*
 * Semaforo que controla el final sel servicio
 */
Semaphore fin(0);

/*
 * Mutex usado para tener el control de los representantes y la
 * sincronizacion final
 */
mutex mtx_end;

/*
 * Mutex usado para controlar la pantalla
 */
mutex mtx_pantalla;

/*
 * Base de datos de clientes atendidos actualmente
 */
DbSesion db_sesion;

/*
 * Numero total de clientes atendidos
 */
int numero_total_clientes;

/*
 * Factura total
 */
double factura_total;

/*
 * Gestor de precios, inicializado para cobrar 2 euros por
 * establecimiento de servicio y euro y medio por petición
 */
GestorPrecios gestor_precios(1.5, 2);

/*
 * Base de datos donde se guardan los monumentos y restaurantes
 * La inicializamos para que devuelva un maximo de 5 monumentos
 */
DbMonumentosRestaurantes db_monumentos_restaurantes(5);//cargamos los datos de los monumentos y restaurantes

/*
 * Funcion que simula a un representante
 */
void representante(Socket &socket, int client_fd) {

    // Controlo que hay un nuevo cliente
    if (end_service.load()) {
        string den = "Servicio denegado";
        enviarMensaje(client_fd, den, socket);
        socket.Close(client_fd);
        return;//Salimos
    } else {// Se puede atender al cliente
        mtx_end.lock();
        ++numero_total_clientes;
        ++representantes_activos;//Aumento el numero de representantes activos
        mtx_end.unlock();
    }

    int length = 100;
    string buffer;
    string respuesta;

    1111
    /*
    * Recibo primer mensaje con id del cliente
    */
    if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {

        mtx_pantalla.lock();
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        mtx_pantalla.unlock();

    }

   1111
    /*
     * Tratar este mensaje y almacenar en id_cliente el id del cliente
     * */
    int id_cliente;


    while (true) {//itero mientras el cliente envie peticiones
        /*
         * Recibo primer mensaje
         */
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            break;//Salgo del bucle
        }
        if(buffer.compare(MENS_FIN)==0){//El cliente ha decidido finalizar la comunicación
            break;
        }

        //No se ha decidido finalizar la comunicación

        1111
        //Tratar mensaje, es decir rellenar todos los campos de abajo

        //Almacenar en la lista los parámetros a buscar
        Lista<string> parametros_buscar;

        //Almacenar en parametros_en_string los parámetros a buscar pero como un string
        string parametros_en_string;

        //Añado el pedido a la base de datos
        db_sesion.nuevoPedido(id_cliente, parametros_en_string);


        //Monumentos para enviar al cliente
        Lista<Monumento *> enviar_al_cliente;

        //Auxiliar
        Monumento *mon_aux;


        //Algoritmo de buscar si esta
        1111
        while ((enviar_al_cliente.size() < NUM_MONUMENTOS_DEVOLVER)&&) {
            if (listm.size() <= 5) {
                Lista<Monumento *> listm_aux;
                mon_rest.buscarMonumento(p[n], listm_aux);
                listm_aux.begin();
                while (listm_aux.next(mon_aux) && listm.size() <= 5) {
                    if (!listm.belongs(mon_aux)) {
                        listm.add(mon_aux);
                    }
                }
            } else {
                no_lleno = false;
            }
            ++n;
        }


        //Si la lista está vacía (nada encontrado)
        if (enviar_al_cliente.size() == 0) {
            respuesta = "Nada encontrado";
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                break;
            }
            continue;//Vuelvo al inicio
        } else{
            enviar_al_cliente.begin();
            respuesta.clear();
            while(enviar_al_cliente.next(mon_aux)){
                respuesta = respuesta + " " + mon_aux->getlink();
            }
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                break;
            }
        }

        /*
        * Recibo segundo mensaje
         */
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            break;//Salgo del bucle
        }
        if(buffer.compare(MENS_FIN)==0){//El cliente ha decidido finalizar la comunicación
            break;
        }

        sesion.nuevoPedido(client_fd, buffer);
        if (stoi(buffer) >= 0) {
            int indmon = stoi(buffer);
            double coord[2];
            listm.begin();
            while (listm.next(mon_aux)) {
                if (indmon == 0) {
                    mon_aux->getcoordenadas(coord[0], coord[1]);
                }
                indmon--;
            }
            double coordrest[2];
            mon_rest.buscarRestaurante(coord[0], coord[1], coordrest[0], coordrest[1]);
            //Transformar double en string, comprobar si funciona
            c1 << coordrest[0];
            c2 << coordrest[1];
            respuesta = c1.str();
            +' ' + c2.str();
            enviarMensaje(client_fd, respuesta, socket);
        } else {
            precio = GestorPrecios.precio(sesion.numeroPeticionesCliente(client_fd));
            precioTot = precioTot + precio;
            c1 << precio;
            respuesta = c1.str();
            enviarMensaje(client_fd, respuesta, socket);
            mtx.lock();
            dentro--;
            nTotClientes++;
            if (dentro == 0 && endSERV) Fin.signal();
            return 0;
        }

        /*
        * Recibo tercer mensaje
         */
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            break;//Salgo del bucle
        }


        sesion.nuevoPedido(client_fd, buffer);
        if (buffer == MENS_FIN) {
            precio = GestorPrecios.precio(sesion.numeroPeticionesCliente(client_fd));
            precioTot = precioTot + precio;
            c1 << precio;
            respuesta = c1.str();
            enviarMensaje(client_fd, respuesta, socket);
            mtx.lock();
            dentro--;
            nTotClientes++;
            if (dentro == 0 && endSERV) Fin.signal();
            return 0;
        }
    }

    string informacion_cliente;

    db_sesion.listarCliente(id_cliente,informacion_cliente);
    db_sesion.borrarCliente(id_cliente);

    //Muestro informacion sobre el cliente
    mtx_pantalla.lock();

    cout<<informacion_cliente<<endl;

    mtx_pantalla.unlock();

    //Cierro socket cliente
    int error_code = socket.Close(client_fd);
    if (error_code == -1) {
        mtx_pantalla.lock();

        cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;

        mtx_pantalla.unlock();
    }

    //Controlo fin del representante
    mtx_end.lock();
    --representantes_activos;
    if (representantes_activos == 0 && end_service.load()) {
        mtx_end.unlock();
        fin.signal();
    } else {
        mtx_end.unlock();
    }
}

/*
 * Controla el final del programa
 */
void control_fin(int socket_fd, Socket &socket) {
    string finalizar;
    cin >> finalizar;
    end_service.store(true);
    fin.wait();

    //Cerramos el socket para si se ha quedado esperando el accept falle
    if (socket.Close(socket_fd) == -1) {
        cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    }
}

//-------------------------------------------------------------
int main(int argc, char *argv[]) {

    if (argc != 2) {
        cerr << "Numero de argumentos incorrecto" << endl;
        return -1;
    }

    //Marco el fin de servicio como falso
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
    int max_connections = 100;
    int error_code = socket.Listen(max_connections);
    if (error_code == -1) {
        cerr << "Error en el listen: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        return -1;
    }

    /*
    * Controla el final del programa
    */
    thread finalizador(control_fin, socket_fd, ref(socket));


    while (!end_service.load()) {
        // Accept
        int client_fd = socket.Accept();

        if ((client_fd < 0) && (!end_service.load())) {
            cerr << "Error en el accept: " << strerror(errno) << endl;
        } else {//Se ha realizado el accept con exito
            thread(representante, ref(socket), client_fd).detach();
        }
    }

    finalizador.join();//En este momento ya han terminado todos los threads y se ha cerrado el socket

    // Mensaje de despedida
    cout << "Bye bye" << endl;

    return 0;
}
