#include "DbMonumentosRestaurantes.h"

using json = nlohmann::json;
using namespace std;

/*
* Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
* zaragoza y los carga en las estructuras correspondientes
*/
DbMonumentosRestaurantes::DbMonumentosRestaurantes(int MAX_NUM_MONUMENTOS_DEVOLVER) {
  this->dbMonumentos = new SuffixTree<Monumento *>(MAX_NUM_MONUMENTOS_DEVOLVER);
  JsonDownload downloader;

// Tratamiento de los monumentos
  char URL_MONUMENTOS[500] = "http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-";
  char path_MONUMENTOS[100] = "monumentos.json";

  downloader.downloadJSON(URL_MONUMENTOS, path_MONUMENTOS);//descargo los monumentos

  std::ifstream fichero_monumentos(path_MONUMENTOS);
  json json_monumentos(fichero_monumentos);//preparo el archivo json de monumentos descargado

  json::iterator it_monumentos = json_monumentos["features"].begin();//coloco el iterador en el primer monumento

  for (int i = 0;i < json_monumentos["features"].size(); ++i) {//itero hasta haber insertado todos los monumentos a la estructura

    //Valores a guardar del monumento(valores)
    string link_monumento= (*it_monumentos).at("properties").at("link").get<string>();//url del monumento
    double UTM_north=(*it_monumentos).at("geometry").at("coordinates")[0].get<double>();//coordenadas del monumento
    double UTM_east=(*it_monumentos).at("geometry").at("coordinates")[1].get<double>();//coordenadas del monumento
    Monumento *monum_actual = new Monumento(link_monumento,UTM_north,UTM_east);

    //Valores utilizados como clave en el monumento
    string titulo = (*it_monumentos).at("properties").at("title").get<string>();//nombre del monumento
    string autor = (*it_monumentos).at("properties").at("authot").get<string>();//nombre del monumento

    dbMonumentos->add(titulo, monum_actual);
    dbMonumentos->add(autor, monum_actual);

    ++it_monumentos;
  }

// Tratamiento de los restaurantes
  char URL_RESTAURANTES[500] = "http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";
  char path_RESTAURANTES[100] = "restaurantes.json";

  downloader.downloadJSON(URL_RESTAURANTES, path_RESTAURANTES);//descargo los restaurantes


  std::ifstream fichero_restaurantes(path_RESTAURANTES);
  json json_restaurantes(fichero_restaurantes);//preparo el archivo json de restaurantes descargado

  json::iterator it_restaurantes = json_restaurantes["features"].begin();//coloco el iterador en el primer restaurante

  for (int i = 0; i < json_restaurantes["features"].size(); ++i) {//itero hasta haber insertado todos los restaurantes a la estructura

    //Datos utilizados como clave y valor
    double UTM_north=(*it_restaurantes).at("geometry").at("coordinates")[0].get<double>();//coordenadas del monumento
    double UTM_east=(*it_restaurantes).at("geometry").at("coordinates")[1].get<double>();//coordenadas del monumento

    Restaurante nuevo_restaurante(UTM_north,UTM_east);
    dbRestaurantes.add(nuevo_restaurante);

    ++it_restaurantes;
  }

}


/*
* Pre: Se han cargado los monumentos
* Post: Busca coincidencias de dato_a_buscar entre los monumentos almacenados y
*       devuelve un máximo de MAX_NUM_MONUMENTOS_DEVOLVER monumentos en los que
*       existan coincidencias con dato_a_buscar
*/
void DbMonumentosRestaurantes::buscarMonumento(string dato_a_buscar, Lista<Monumento> &resultados) {
  resultados.clear();
  Lista<Monumento *> punt_resultados;
  dbMonumentos->search(dato_a_buscar, punt_resultados);
  Monumento *actual_leido;
  punt_resultados.begin();
  while (punt_resultados.next(actual_leido)) {
    resultados.add(*actual_leido);
  }
}

/*
* Función auxiliar utilizada para calcular la distancia entre un restaurante y un
* monumento
* Devuelve el cuadrado de la distancia de "a" a "b"
*/
double DbMonumentosRestaurantes::calcular_distancia(double a_UTMNorthing , double a_UTMEasting , double b_UTMNorthing , double b_UTMEasting) {
  return (a_UTMNorthing - b_UTMNorthing) * (a_UTMNorthing - b_UTMNorthing)
         + (a_UTMEasting - b_UTMEasting) * (a_UTMEasting -b_UTMEasting);
}

/*
* Pre: Se han cargado los monumentos
* Post: Almacena en restaurante_UTMNorthing y restaurante_UTMEasting las coordenadas en formato UTM
*       del restaurante mas cercano al punto denotado por las coordenadas en el sistema UTM
*       "monumento_UTMNorthing" metros norte, "monumento_UTMEasting" metros este
*/
bool DbMonumentosRestaurantes::buscarRestaurante(double monumento_UTMNorthing, double monumento_UTMEasting,
                                                 double& restaurante_UTMNorthing, double& restaurante_UTMEasting) {

  if (dbRestaurantes.size() > 0) {
    mtx_buscarRestaurante.lock();//cojo el mutex

    //creo un restaurante con un valor aleatorio
    Restaurante rest_actual(0,0);

    //Pongo el primer elemento como seleccionado
    dbRestaurantes.next(rest_actual);
    double aux_UTMNorthing,aux_UTMEasting;
    rest_actual.getcoordenadas(aux_UTMNorthing,aux_UTMEasting);

    //Distancia del seleccionado al monumento
    double distancia_del_seleccionado=this->calcular_distancia(aux_UTMNorthing,aux_UTMEasting,monumento_UTMNorthing,monumento_UTMEasting);

    dbRestaurantes.begin();

    while (dbRestaurantes.next(rest_actual)) {

      rest_actual.getcoordenadas(aux_UTMNorthing,aux_UTMEasting);

      if(distancia_del_seleccionado>this->calcular_distancia(aux_UTMNorthing,aux_UTMEasting,monumento_UTMNorthing,monumento_UTMEasting)){
        distancia_del_seleccionado=this->calcular_distancia(aux_UTMNorthing,aux_UTMEasting,monumento_UTMNorthing,monumento_UTMEasting);
        restaurante_UTMNorthing=aux_UTMNorthing;
        restaurante_UTMEasting=aux_UTMEasting;
      }
    }
    mtx_buscarRestaurante.unlock();//suelto el mutex
    return true;
  }
  else return false;
}
