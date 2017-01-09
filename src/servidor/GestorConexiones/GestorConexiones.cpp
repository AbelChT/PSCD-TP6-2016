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
#include <cstdlib>

#include "../../../src/librerias/Socket/Socket.h"
#include "../../../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"
#include "../../../src/servidor/GestorPrecios/GestorPrecios.h"
#include "../../../src/servidor/DbSesion/DbSesion.h"
#include "../../../src/librerias/UTM2LL/LatLong-UTMconversion.h"
#include "../../../src/librerias/Semaphore/Semaphore.h"


using namespace std;

/*
 * Tama�o m�ximo del mensaje
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
 string MENS_FIN = "Fin";

/*
 * Numero de monumentos a devolver al cliente
 */
const int NUM_MONUMENTOS_DEVOLVER=5;

/*
 * Control de finalizai�n del servicio
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

sesion s;

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
 * establecimiento de servicio y euro y medio por petici�n
 */
GestorPrecios gestor_precios(1.5, 2);

/*
 * Funcion que simula a un representante
 */
void representante(Socket &socket, int client_fd, DbMonumentosRestaurantes &db_monumentos_restaurantes) {
    int length = 100;
    string buffer;
    string respuesta;
    // Controlo que hay un nuevo cliente
    if (end_service.load()) {
        string den = "Servicio denegado";
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
	   	 mtx_end.lock();
   	    if (representantes_activos == 0) {
            		mtx_end.unlock();
            		fin.signal();
    	  	}
        socket.Close(client_fd);
	    	return;
    }
    // Se puede atender al cliente
    mtx_end.lock();
    ++numero_total_clientes;
    ++representantes_activos;//Aumento el numero de representantes activos
    mtx_end.unlock();
    /*
    * Recibo primer mensaje con id del cliente
    */
    if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
        mtx_pantalla.lock();
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        mtx_pantalla.unlock();
        socket.Close(client_fd);
	      mtx_end.lock();
   	    --representantes_activos;
   	    if (representantes_activos == 0 && end_service.load()) {
          mtx_end.unlock();
          fin.signal();
    	  }
	      return;
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
           		  mtx_end.unlock();
           		  fin.signal();
    	   	  }
	          return;
    }
    /*
     * Almacenar en id_cliente el id del cliente
     * */
    int id_cliente=stoi(buffer);

    while (true) {//itero mientras el cliente envie peticiones
        /*
         * Recibo primer mensaje
         */
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            mtx_pantalla.lock();
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            break;//Salgo del bucle
        }
        if(buffer==MENS_FIN){//El cliente ha decidido finalizar la comunicaci�n
            break;
        }

        //No se ha decidido finalizar la comunicaci�n
	/*cout << "Almacenando...." << endl;
        Almacenar en la lista los par�metros a buscar
        Lista<string> parametros_buscar;
        int j = -1;
    	  int cont = 0;
    	  int aux;
    	  for (int i = 0; i < buffer.length(); i++) {
        	if (buffer[i] == ' ') {
            		aux = j + 1;
            		j = i;
            		parametros_buscar.add(buffer.substr(aux, j - aux));
            		cont++;
        	}
		      else if (i == (buffer.length()) - 1) {
            		aux = j + 1;
            		j = i + 1;
            		parametros_buscar.add(buffer.substr(aux, j - aux));
        	}
    	 }*/

        //A�ado el pedido a la base de datos
        nuevoPedido(id_cliente, "Buscar monumento "+buffer,s);
        //Monumentos para enviar al cliente
        Lista<Monumento *> enviar_al_cliente;
	db_monumentos_restaurantes.buscarMonumento(buffer, enviar_al_cliente)


	  /*Auxiliar para obtener datos de cierto monumento
	      Monumento * mon_aux;
        //Auxiliar
        Lista<Monumento *> aux_monum;
        parametros_buscar.begin();

        string buscar_esta_iteracion;
        //Algoritmo de buscar si esta
        while ((enviar_al_cliente.size() < NUM_MONUMENTOS_DEVOLVER)&&parametros_buscar.next(buscar_esta_iteracion)) {
            db_monumentos_restaurantes.buscarMonumento(buscar_esta_iteracion, aux_monum);
            aux_monum.begin();
            Monumento* actual_mirar;
            while(aux_monum.next(actual_mirar)&&(enviar_al_cliente.size() < NUM_MONUMENTOS_DEVOLVER)){
                if(!enviar_al_cliente.belongs(actual_mirar)){
                    enviar_al_cliente.add(actual_mirar);
                }
            }
        }
	*/
        //Si la lista est� vac�a (nada encontrado)
        if (enviar_al_cliente.size() == 0) {
            respuesta = "Nada encontrado";
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                break;
            }
            continue;//Vuelvo al inicio
        } else{
            enviar_al_cliente.begin();
            enviar_al_cliente.next(mon_aux);
            respuesta=mon_aux->getlink();
            while(enviar_al_cliente.next(mon_aux)){
                respuesta = respuesta + " " + mon_aux->getlink();
            }
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                mtx_pantalla.lock();
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                mtx_pantalla.unlock();
                break;
            }
        }

        /*
        * Recibo segundo mensaje
         */
        if (socket.Recv(client_fd, buffer, MESSAGE_SIZE) < 0) {
            mtx_pantalla.lock();
            cerr << "Error al recibir datos: " << strerror(errno) << endl;
            mtx_pantalla.unlock();
            break;//Salgo del bucle
        }
        if(buffer==MENS_FIN){//El cliente ha decidido finalizar la comunicaci�n
            break;
        }
	/*char *bufferaux = strdup(buffer.c_str());
        if(atoi(bufferaux)<1||atoi(bufferaux)>enviar_al_cliente.size()){//La respuesta es no coherente
            mtx_pantalla.lock();
            cerr << "La respuesta del cliente no es coherente"<< endl;
            mtx_pantalla.unlock();
	    respuesta = "Informacion incorrecta";
            if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
            	mtx_pantalla.lock();
            	cerr << "Error al enviar datos: " << strerror(errno) << endl;
            	mtx_pantalla.unlock();
            	break;
            }
	   continue;
        }*/
        nuevoPedido(client_fd, "Buscar restaurante "+buffer,s);
        int indmon = atoi(bufferaux);
        double coord_monumento[2];
        enviar_al_cliente.begin();
        while (enviar_al_cliente.next(mon_aux)) {
            if (indmon == 0) {
                 mon_aux->getcoordenadas(coord_monumento[0], coord_monumento[1]);
             }
            indmon--;
        }
        double coord_rest[2];
        db_monumentos_restaurantes.buscarRestaurante(coord_monumento[0], coord_monumento[1], coord_rest[0], coord_rest[1]);//suponemos que siempre se carga bien
	      //Convertimos UTM a Lat, Long
	      int RefEllipsoid = 23;
	      double Lat, Long;
	      char UTMZoneZgza[4] = "30T";
	      UTMtoLL(RefEllipsoid, coord_rest[0], coord_rest[1], UTMZoneZgza, Lat, Long);
        respuesta = to_string(Lat) +" " + to_string(Long);

        if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                mtx_pantalla.lock();
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                mtx_pantalla.unlock();
                break;
        }
    }
	//Coste del servicio
    int precio_local=gestor_precios.precio(nPeticionesCliente(s,id_cliente));
    respuesta=to_string(precio_local);

    //Envio al cliente el precio por el servicio
    if (socket.Send(client_fd, respuesta) < 0) {//Caso en el que falle el envio del mensaje
                mtx_pantalla.lock();
                cerr << "Error al enviar datos: " << strerror(errno) << endl;
                mtx_pantalla.unlock();
    }

    string informacion_cliente;
    listarCliente(s,id_cliente,informacion_cliente);
    borrarCliente(id_cliente,s);

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
    factura_total=factura_total+precio_local;
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
    if (representantes_activos!=0) fin.wait();
    //Cerramos el socket para si se ha quedado esperando el accept falle
    if (socket.Close(socket_fd) == -1) {
        cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    }
}

//-------------------------------------------------------------
int main(int argc, char *argv[]) {
    crearSesion(s);
    if (argc != 2) {
        cerr << "Numero de argumentos incorrecto" << endl;
        return -1;
    }
    cout << "Cargando base de datos" << endl;
    /*
     * Base de datos donde se guardan los monumentos y restaurantes
     * La inicializamos para que devuelva un maximo de 5 monumentos
     */
    DbMonumentosRestaurantes db_monumentos_restaurantes(5);//cargamos los datos de los monumentos y restaurantes
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

        if (client_fd < 0) {
            cerr << "Error en el accept: " << strerror(errno) << endl;
		break;
        } else {//Se ha realizado el accept con exito
            thread(representante, ref(socket), client_fd, ref(db_monumentos_restaurantes)).detach();
        }
    }
    if(representantes_activos==0)fin.signal();
    finalizador.join();//En este momento ya han terminado todos los threads y se ha cerrado el socket
    string listado;
    listarTodo(s,listado);
    cout << listado << endl;
	cout << endl;
	cout << "Total recaudado: " << factura_total << endl;
    // Mensaje de despedida
    cout << "Bye bye" << endl;

    return 0;
}
