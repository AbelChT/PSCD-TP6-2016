/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
* Coms: Este módulo se comporta como un monitor
*       por lo que las operaciones buscarMonumento y
*       buscar restaurante se pueden ejecutar en multiples
*       threads de una forma segura
******************************************************************/

#include "DbMonumentosRestaurantes.h"

using json = nlohmann::json;
using namespace std;

/*
 * Divide el string input en palabras y las almacena en output
 */
void formatear_y_separar(string input, Lista<string> &output) {
    int j = 0;
    int length = input.length();
    for (int i = 0; i < length; i++) {
        if (input[j] == ' ') {
            if (j != 0) {
                output.add(input.substr(0, j));
            }
            input = input.substr(j + 1);
            j = 0;
        }
        else {
            ++j;
        }
    }

    if (input.length() > 0) {
        output.add(input);
    }
}


/*
* Pre: MAX_NUM_MONUMENTOS_DEVOLVER > 0
* Post: Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
* zaragoza y los carga en las estructuras correspondientes y establece
* MAX_NUM_MONUMENTOS_DEVOLVER como el máximo numero de monumentos a devolver
*/
DbMonumentosRestaurantes::DbMonumentosRestaurantes(int MAX_NUM_MONUMENTOS_DEVOLVER1) {

    //Seteo el maximo numero de datos a devolver
    this->MAX_NUM_MONUMENTOS_DEVOLVER = MAX_NUM_MONUMENTOS_DEVOLVER1;
    /*
    * Url obtenida haciendo una consulta al buscador Solr proporcionado por la api de zaragoza. He liminato a que simplemente devuelve los
    * 300 primeros monumentos debido a que a partir del monumento 360 hay algunos que no poseen direccion (campo geometry) lo cual provoca
    * una interrupcion en el programa
    * Me he decantado por utilizar esta url y no la que proporcionan ellos para json debido a que esta segunda posee datos repetidos,
    * ademas de no ofrecerte las posibilidades que te ofrece el buscador solr (con este puedes acceder directamente a la informacion del
    * monumento)
    */
    char URL_MONUMENTOS[500] = "http://www.zaragoza.es/buscador/select?wt=json&q=*:*%20AND%20-tipocontenido_s:estatico%20AND%20category:(%22Arte%20P\\u00fablico%22)&fl=id,uri,title,texto_t,emplazamiento_smultiple,barrio_smultiple,epoca_smultiple,autor_smultiple,material_smultiple,iconografia_smultiple,categoria_smultiple,x_coordinate,y_coordinate,text&rows=300";

    char path_MONUMENTOS[100] = "monumentos.json";

    JsonDownload downloader;

    downloader.downloadJSON(URL_MONUMENTOS, path_MONUMENTOS);

    std::ifstream fichero_monumentos(path_MONUMENTOS);

    json json_monumentos = json::parse(fichero_monumentos);//preparo el archivo json de monumentos descargado

    json::iterator it_monumentos = json_monumentos["response"].at("docs").begin();//coloco el iterador en el primer monumento

    int rango_buscar = json_monumentos["response"].at("docs").size();

    for (int i = 0; i < rango_buscar; ++i) {//itero hasta haber insertado todos los monumentos a la estructura
        //Valores monumento
        string link_monumento = (*it_monumentos).at("uri").get<string>();//url del monumento

        double UTM_north = (*it_monumentos).at("y_coordinate").get<double>();//coordenadas del monumento
        double UTM_east = (*it_monumentos).at("x_coordinate").get<double>();//coordenadas del monumento
        Monumento *monum_actual = new Monumento(link_monumento, UTM_north, UTM_east);

        //Almaceno las claves de los monumentos a insertar en el arbol
        Lista<string> lista_aux_monumentos_insertar;

        //Claves monumento
        string titulo = (*it_monumentos).at("title").get<string>();//nombre del monumento
        formatear_y_separar(titulo, lista_aux_monumentos_insertar);

        int num_autores = ((*it_monumentos).at("autor_smultiple")).size();

        for (int j = 0; j < num_autores; j++) {//Obtengo los autores
            string autor_aux = ((*it_monumentos).at("autor_smultiple")[j]).get<string>();
            formatear_y_separar(autor_aux, lista_aux_monumentos_insertar);
        }

        int num_fields_texto = ((*it_monumentos).at("text")).size();
        for (int j = 0; j < num_fields_texto; j++) {//Obtengo toda la informacion de dentro de la url(texto)
            string info_url = ((*it_monumentos).at("text")[j]).get<string>();
            formatear_y_separar(info_url, lista_aux_monumentos_insertar);
        }
        lista_aux_monumentos_insertar.begin();
        string insertar_ahora;

        while (lista_aux_monumentos_insertar.next(insertar_ahora)) {
            this->dbMonumentos.add(insertar_ahora, monum_actual);
        }
        ++it_monumentos;
    }

    char URL_RESTAURANTES[500] = "http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";
    char path_RESTAURANTES[100] = "restaurantes.json";

    downloader.downloadJSON(URL_RESTAURANTES, path_RESTAURANTES);//descargo los restaurantes


    std::ifstream fichero_restaurantes(path_RESTAURANTES);
    json json_restaurantes = json::parse(fichero_restaurantes);

    json::iterator it_restaurantes = json_restaurantes["features"].begin();//coloco el iterador en el primer restaurante

    for (int i = 0; i <json_restaurantes["features"].size(); ++i) {//itero hasta haber insertado todos los restaurantes a la estructura

    //Datos utilizados como clave y valor
        double UTM_north = (*it_restaurantes).at("geometry").at(
            "coordinates")[1].get<double>();//coordenadas del restaurante
        double UTM_east = (*it_restaurantes).at("geometry").at(
            "coordinates")[0].get<double>();//coordenadas del restaurante

        Restaurante *restaurante_a_anyadir = new Restaurante(UTM_north, UTM_east);
        dbRestaurantes.add(restaurante_a_anyadir);
        ++it_restaurantes;
    }

}


/*
* Pre: Se han cargado los monumentos
* Post: Busca coincidencias de dato_a_buscar entre los monumentos almacenados y
*       devuelve un máximo de MAX_NUM_MONUMENTOS_DEVOLVER punteros a monumentos en los que
*       existan coincidencias con dato_a_buscar
*/
void DbMonumentosRestaurantes::buscarMonumento(string dato_a_buscar, Lista<Monumento *> &resultados) {
    resultados.clear();
    Lista<string> terminos_a_buscar;
    formatear_y_separar(dato_a_buscar, terminos_a_buscar);//Obtengo las palabras independientes

    if (terminos_a_buscar.size() > 0) {//Hay al menos un termino para buscar
        ListaOrdenada<Monumento *> *resultados_parciales = new ListaOrdenada<Monumento *>[terminos_a_buscar.size()];

        string termino_actual_buscar;
        terminos_a_buscar.begin();
        int k = 0;
        while (terminos_a_buscar.next(termino_actual_buscar)) {
            this->dbMonumentos.search(termino_actual_buscar, resultados_parciales[k]);
            ListaOrdenada<Monumento *> prueba_debug;
            k++;
        }//En este momento esta en resultados parciales almacenados todos los matches
        //con alguno de los terminos

        //Busco los monumentos en los cuales se encuentran todas las palabras
        if (terminos_a_buscar.size() > 1) {//ha habido al menos dos terminos a buscar
            Monumento **actuales_analizar = new Monumento*[terminos_a_buscar.size()];
            for (int i = 0; i < terminos_a_buscar.size(); ++i) {
                resultados_parciales[i].begin();
                if (!resultados_parciales[i].next(actuales_analizar[i])) {//Todas tienen al menos un dato
                    break;
                }
            }

            while (true) {//Busco los terminos que coincidan en las listas
                Monumento *analizar_igual = actuales_analizar[0];
                bool iguales = true;

                for (int i = 1; i < terminos_a_buscar.size(); ++i) {//compruebo si las direcciones de memoria coinciden
                    if (analizar_igual != actuales_analizar[i]) {
                        iguales = false;
                        break;
                    }
                }

                if (iguales) {//las direcciones de memoria son las mismas
                    resultados.add(analizar_igual);
                    if (resultados.size() == 5) {
                        break;//Hay ya 5 elementos a devolver
                    }
                }
                int menor = 0;
                for (int i = 1; i < terminos_a_buscar.size(); ++i) {//Busco el menor para avanzar alguno de las listas
                    if (actuales_analizar[i] < actuales_analizar[menor]) {
                        menor = i;
                    }
                }
                if (!resultados_parciales[menor].next(actuales_analizar[menor])) {
                    break;//No hay siguiente
                }
            }
            delete[] resultados_parciales;//vacio memoria
            delete[] actuales_analizar;//vacio memoria
        }
        else {//Solo se ha insertado una palabra
            Monumento *a_pasar;
            resultados_parciales[0].begin();
            while (resultados.size() < this->MAX_NUM_MONUMENTOS_DEVOLVER && resultados_parciales[0].next(a_pasar)) {
                //Copio de una tabla a la Otra
                resultados.add(a_pasar);
            }
        }
    }
}

/*
* Función auxiliar utilizada para calcular la distancia entre un restaurante y un
* monumento
* Devuelve el cuadrado de la distancia de "a" a "b"
*/
double DbMonumentosRestaurantes::calcular_distancia(double a_UTMNorthing, double a_UTMEasting, double b_UTMNorthing,
    double b_UTMEasting) {
    return (a_UTMNorthing - b_UTMNorthing) * (a_UTMNorthing - b_UTMNorthing)
        + (a_UTMEasting - b_UTMEasting) * (a_UTMEasting - b_UTMEasting);
}

/*
* Pre: Se han cargado los monumentos
* Post: Almacena en restaurante_UTMNorthing y restaurante_UTMEasting las coordenadas en formato UTM
*       del restaurante mas cercano al punto denotado por las coordenadas en el sistema UTM
*       "monumento_UTMNorthing" metros norte, "monumento_UTMEasting" metros este
*/
bool DbMonumentosRestaurantes::buscarRestaurante(double monumento_UTMNorthing, double monumento_UTMEasting,
    double &restaurante_UTMNorthing, double &restaurante_UTMEasting) {

    if (dbRestaurantes.size() > 0) {
        mtx_buscarRestaurante.lock();//cojo el mutex

        //creo un restaurante con un valor aleatorio
        Restaurante *rest_actual;

        //Pongo el primer elemento como seleccionado
        dbRestaurantes.next(rest_actual);
        double aux_UTMNorthing, aux_UTMEasting;
        rest_actual->getcoordenadas(aux_UTMNorthing, aux_UTMEasting);

        //Distancia del seleccionado al monumento
        double distancia_del_seleccionado = this->calcular_distancia(aux_UTMNorthing, aux_UTMEasting,
            monumento_UTMNorthing, monumento_UTMEasting);

        dbRestaurantes.begin();

        while (dbRestaurantes.next(rest_actual)) {

            rest_actual->getcoordenadas(aux_UTMNorthing, aux_UTMEasting);

            if (distancia_del_seleccionado >
                this->calcular_distancia(aux_UTMNorthing, aux_UTMEasting, monumento_UTMNorthing,
                    monumento_UTMEasting)) {
                distancia_del_seleccionado = this->calcular_distancia(aux_UTMNorthing, aux_UTMEasting,
                    monumento_UTMNorthing, monumento_UTMEasting);
                restaurante_UTMNorthing = aux_UTMNorthing;
                restaurante_UTMEasting = aux_UTMEasting;
            }
        }
        mtx_buscarRestaurante.unlock();//suelto el mutex
        return true;
    }
    else return false;
}
