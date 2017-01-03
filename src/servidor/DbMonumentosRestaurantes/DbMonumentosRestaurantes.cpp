#include "DbMonumentosRestaurantes.h"

using json = nlohmann::json;
using namespace std;

/*
* Descarga los monumentos y restaurantes proporcionados por el ayuntamiento de
* zaragoza y los carga en las estructuras correspondientes
*/
void cargarMonumentosRestaurantes() {
    JsonDownload downloader;

    // Tratamiento de los monumentos
    char URL_MONUMENTOS[500] = "http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-";
    char path_MONUMENTOS[100] = "monumentos.json";

    downloader.downloadJSON(URL_MONUMENTOS, path_MONUMENTOS);//descargo los monumentos

    std::ifstream fichero_monumentos(path_MONUMENTOS);
    json json_monumentos(fichero_monumentos);//preparo el archivo json de monumentos descargado

    json::iterator it_monumentos = json_monumentos["features"].begin();//coloco el iterador en el primer monumento

    for (int i = 0; i < json_monumentos["features"].size(); ++i) {//itero hasta haber insertado todos los monumentos a la estructura

        //Debido a que la descripción, la categoria y la fecha es vacia en todos los monumentos, no la he cargado en la
        // estructura de datos
        Monumento* monum_actual=new Monumento;
        monum_actual->title = (*it_monumentos).at("properties").at("title").get<string>();//nombre del monumento
        monum_actual->link = (*it_monumentos).at("properties").at("link").get<string>();//url del monumento
        monum_actual->coordenadas[0] = (*it_monumentos).at("geometry").at("coordinates")[0].get<double>();//coordenadas del monumento
        monum_actual->coordenadas[1] = (*it_monumentos).at("geometry").at("coordinates")[1].get<double>();//coordenadas del monumento
        dbMonumentos.add(monum_actual->title,monum_actual);

        ++it_monumentos;
    }

    // Tratamiento de los restaurantes
    char URL_RESTAURANTES[500] = "http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";
    char path_RESTAURANTES[100] = "restaurantes.json";

    downloader.downloadJSON(URL_RESTAURANTES, path_RESTAURANTES);//descargo los restaurantes


    std::ifstream fichero_restaurantes(path_MONUMENTOS);
    json json_restaurantes(fichero_restaurantes);//preparo el archivo json de restaurantes descargado

    json::iterator it_restaurantes = json_restaurantes["features"].begin();//coloco el iterador en el primer restaurante

    for (int i = 0; i < json_restaurantes["features"].size(); ++i) {//itero hasta haber insertado todos los restaurantes a la estructura
        Restaurante nuevo_restaurante;
        nuevo_restaurante.nombre = (*it_restaurantes).at("properties").at("title").get<string>();//Nombre del restaurante
        nuevo_restaurante.link = (*it_restaurantes).at("properties").at("link").get<string>();//link al restaurante
        nuevo_restaurante.direccion = (*it_restaurantes).at("properties").at("description").get<string>();//Calle del restaurante
        nuevo_restaurante.categoria = (*it_restaurantes).at("properties").at("category").get<string>();// Numero de tenedores del restaurante aunque tambien hay otras categorias
        nuevo_restaurante.coordenadas[0] = (*it_restaurantes).at("geometry").at("coordinates")[0].get<double>();//coordenadas del restaurante
        nuevo_restaurante.coordenadas[1] = (*it_restaurantes).at("geometry").at("coordinates")[1].get<double>();

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
void buscarMonumento(string dato_a_buscar, Pila<Monumento> resultados){
  resultados.clear();
  dbMonumentos.search(substring,resultados);
}

/*
* Función auxiliar utilizada para calcular la distancia entre un restaurante y un
* monumento
* Devuelve el cuadrado de la distancia de "a" a "b"
*/
double calcular_distancia(Restaurante& a , Monumento& b){
  return (a.coordenadas[0]-b.coordenadas[0])*(a.coordenadas[0]-b.coordenadas[0])
  + (a.coordenadas[1]-b.coordenadas[1])*(a.coordenadas[1]-b.coordenadas[1]);
}

/*
* Pre: Se han cargado los monumentos
* Post: Almacena en resultados los MAX_NUM_RESTAURANTES_DEVOLVER más cercanos
*       a monumento_seleccionado en el caso de que existan al menos MAX_NUM_RESTAURANTES_DEVOLVER
*       en dbRestaurantes
*       En caso contrario devuelve todos los restaurantes almacenados en dbRestaurantes
*/
void buscarRestaurante(Monumento monum_seleccionado, Pila<Restautante> resultados){
  resultados.clear();

  Restaurantes seleccionados[MAX_NUM_RESTAURANTES_DEVOLVER];
  int almacenados=0;
  double distancia_seleccionados[MAX_NUM_RESTAURANTES_DEVOLVER];

  if(dbRestaurantes.size>0){
    mtx_buscarRestaurante.lock();//cojo el mutex

    dbRestaurantes.begin();
    Restaurante rest_actual;

    while (dbRestaurantes.next(rest_actual)) {
      if (almacenados<MAX_NUM_RESTAURANTES_DEVOLVER) {//hay menos de MAX_NUM_RESTAURANTES_DEVOLVER seleccionados
        seleccionados[almacenados]=rest_actual;
        distancia_seleccionados[almacenados]=calcular_distancia(rest_actual,monum_seleccionado);
        ++almacenados;
      }else{
        int indice_mayor=0;//almacena el indice del restaurante mas lejano de los seleccionados
        for (int i = 1; i < MAX_NUM_RESTAURANTES_DEVOLVER; i++) {//busco el mayor de los almacenados
          if(distancia_seleccionados[indice_mayor]<distancia_seleccionados[i]){
            indice_mayor=i;
          }
        }
        double distancia_actual=calcular_distancia(rest_actual,monum_seleccionado);
        if(distancia_seleccionados[indice_mayor]>distancia_actual){
          distancia_seleccionados[indice_mayor]=distancia_actual;
          seleccionados[indice_mayor]=rest_actual;
        }
      }
    }
    mtx_buscarRestaurante.unlock();//suelto el mutex

    for (int i = 0; i < almacenados; i++) {//añado los seleccionados a la lista de resultado
      resultados.add(seleccionados[i]);
    }
  }
}
