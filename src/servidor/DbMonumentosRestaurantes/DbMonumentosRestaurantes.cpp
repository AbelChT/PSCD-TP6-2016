#include "DbMonumentosRestaurantes.h"

using json = nlohmann::json;
using namespace std;

/*
 * Divide el string input en palabras y las almacena en output
 */
void formatear_y_separar(string input,Lista<string>& output){
  int j=0;
  int length=input.length();
  for (int i = 0; i < length; i++) {
      if (input[j] == ' ') {
        if(j!=0){
          output.add(input.substr(0,j));
        }
        input=input.substr(j+1);
        j=0;
      }else{
        ++j;
      }
}
if(input.length()>0){
  output.add(input);
}
}


/*
 * Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
 * zaragoza y los carga en las estructuras correspondientes
 */
DbMonumentosRestaurantes::DbMonumentosRestaurantes(int MAX_NUM_MONUMENTOS_DEVOLVER) {
    this->dbMonumentos = new SuffixTree<Monumento *>(MAX_NUM_MONUMENTOS_DEVOLVER);
    JsonDownload downloader;
    /*
    * Tratamiento de los monumentos
    *
    * Los datos los descargo desde www.zaragoza.es/buscador/select?wt=json&q=*:* AND -tipocontenido_s:estatico AND category:("Arte P\u00fablico")&fl=uri,title,emplazamiento_smultiple,barrio_smultiple,epoca_smultiple,autor_smultiple,x_coordinate,y_coordinate&rows=400
    * Esta url la he obtenido realizando una consulta al buscador Solr
    * En esta consulta he hecho que busque en el conjunto de datos Arte P\u00fablico
    * y que de ahí me devuelva los 400 primeros registros, debido a que
    * en total son 385 y la opcion de rows=0 no devuelve todos
    * Ademas le he indicado que me devuelva el autor, el titulo, la url, la epoca,
    * la ubicacion y las coordenadas.
    * Por decisiones de diseño hemos optado por no incluir la descripcion ni la
    * cronica de los monumentos. Esta informacion la podriamos obtener si en la
    * consulta en el parametro fl añadiesemos la opcion text, quedando fl asi:
    * fl=uri,title,emplazamiento_smultiple,barrio_smultiple,epoca_smultiple,autor_smultiple,x_coordinate,y_coordinate,text
    * Luego en la respuesta, en la posicion numero 14 del campo text encontrariamos estos datos.
    * La decision de no añadirlo la hemos tomado debido a que la estructura de datos que
    * hemos utilizado para tener una gran eficiencia en tiempo de consulta, con datos de gran longitud
    * es muy ineficiente en memoria y en tiempo de carga.
    * Por lo que para seguir manteniendo los beneficios de esta estructura hemos decidido
    * restringir los campos en los que se realiza la busqueda.
    *
    * Los datos los hemos decidido obtener a partir de consultas a Solr, debido a que
    * de esta forma tenemos control sobre lo que nos devuelve y tambien debido a
    * que algunos de los registros almacenados en http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-
    * (que es el enlace que proporciona la web para el formato json) estan repetidos.
    */

    // Tratamiento de los monumentos\n

    //Hay que conseguir descargar la url de abajo
    char URL_MONUMENTOS[500] = "www.zaragoza.es/buscador/select?wt=json&q=*:* AND -tipocontenido_s:estatico AND category:(\"Arte Pru00fablico\")&fl=uri,title,emplazamiento_smultiple,barrio_smultiple,epoca_smultiple,autor_smultiple,x_coordinate,y_coordinate&rows=400";
    URL_MONUMENTOS[97]='\\';//si lo añado sobre el texto el directamente transforma u en tilde
  //  cout<<URL_MONUMENTOS<<endl;
    char path_MONUMENTOS[100] = "monumentos.json";

    downloader.downloadJSON(URL_MONUMENTOS, path_MONUMENTOS);//descargo los monumentos

    std::ifstream fichero_monumentos(path_MONUMENTOS);
    json json_monumentos(fichero_monumentos);//preparo el archivo json de monumentos descargado

    json::iterator it_monumentos = json_monumentos["response"].at("docs").begin();//coloco el iterador en el primer monumento

    for (int i = 0; i < json_monumentos["response"].at("docs").size(); ++i) {//itero hasta haber insertado todos los monumentos a la estructura
        //Valores monumento
        string link_monumento = (*it_monumentos).at("uri").get<string>();//url del monumento
        double UTM_north = (*it_monumentos).at("y_coordinate").get<double>();//coordenadas del monumento
        double UTM_east = (*it_monumentos).at("x_coordinate").get<double>();//coordenadas del monumento
        Monumento *monum_actual = new Monumento(link_monumento, UTM_north, UTM_east);

        //Almaceno las claves de los monumentos a insertar en el arbol
        Lista<string> lista_aux_monumentos_insertar;

        //Claves monumento
        string titulo = (*it_monumentos).at("title").get<string>();//nombre del monumento
        formatear_y_separar(titulo,lista_aux_monumentos_insertar);

        int num_autores=((*it_monumentos).at("autor_smultiple")).size();

        for (int j = 0; j < num_autores; j++) {//Obtengo los autores
          string autor_aux=((*it_monumentos).at("autor_smultiple")[j]).get<string>();
          formatear_y_separar(autor_aux,lista_aux_monumentos_insertar);
        }

        int num_fields_texto=((*it_monumentos).at("text")).size();
        for (int j = 0; j < num_fields_texto; j++) {//Obtengo toda la informacion de dentro de la url
          string info_url=((*it_monumentos).at("text")[j]).get<string>();
          formatear_y_separar(info_url,lista_aux_monumentos_insertar);
        }
        lista_aux_monumentos_insertar.begin();
        string insertar_ahora;
        while (lista_aux_monumentos_insertar.next(insertar_ahora)) {
          dbMonumentos->add(insertar_ahora, monum_actual);
        }
        ++it_monumentos;
    }
/*
* Tratamiento de los restaurantes
*
* Los datos los descargo desde http://www.zaragoza.es/api/recurso/turismo/restaurante?fl=geometry&rf=html&results_only=true&srsname=utm30n&rows=0
* Esta url la he conseguido utilizando la api de zaragoza http://www.zaragoza.es/docs-api/#!/Turismo%3A_Restaurantes
* Otra fuente de datos que tambien te ofrece la web es http://www.zaragoza.es/georref/json/hilo/ver_Restaurante
* En esta aparecen mas restaurantes, pero creo que es debido a que ha de estar desactualizada,
* por eso no la he utilizado
*
* Los parametros introducidos en la api han sido:
* fl=geometry para asi obtener solamente su ubicación
* rf=html para obtener los datos con formato de texto html
* results_only=true para eliminar atributos de paginacion
* srsname=utm30n para obtener las coordenadas en formato UTM
* rows=0 para obtener todos los registros
*/


    char URL_RESTAURANTES[500] = "http://www.zaragoza.es/api/recurso/turismo/restaurante?fl=geometry&rf=html&results_only=true&srsname=utm30n&rows=0";
    char path_RESTAURANTES[100] = "restaurantes.json";

   downloader.downloadJSON(URL_RESTAURANTES, path_RESTAURANTES);//descargo los restaurantes


    std::ifstream fichero_restaurantes(path_RESTAURANTES);
    json json_restaurantes(fichero_restaurantes);//preparo el archivo json de restaurantes descargado

    json::iterator it_restaurantes = json_restaurantes["features"].begin();//coloco el iterador en el primer restaurante

    for (int i = 0; i <
                    json_restaurantes["features"].size(); ++i) {//itero hasta haber insertado todos los restaurantes a la estructura

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
*       devuelve un máximo de MAX_NUM_MONUMENTOS_DEVOLVER monumentos en los que
*       existan coincidencias con dato_a_buscar
*/
void DbMonumentosRestaurantes::buscarMonumento(string dato_a_buscar, Lista<Monumento *> &resultados) {
    resultados.clear();
    Lista<Monumento *> punt_resultados;
    dbMonumentos->search(dato_a_buscar, punt_resultados);
    Monumento *actual_leido;
    punt_resultados.begin();
    while (punt_resultados.next(actual_leido)) {
        resultados.add(actual_leido);
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
    } else return false;
}
