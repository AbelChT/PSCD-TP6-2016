#include "DbMonumentosRestaurantes.h"

using json = nlohmann::json;
using namespace std;

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
ojo // aqui falta añadirlas a la estructura
        string ss1 = (*it_monumentos).at("properties").at("title").get<string>();//nombre del monumento
        string ss2 = (*it_monumentos).at("properties").at("link").get<string>();//url del monumento
        double cordx = (*it_monumentos).at("geometry").at("coordinates")[0].get<double>();//coordenadas del monumento
        double cordy = (*it_monumentos).at("geometry").at("coordinates")[1].get<double>();//coordenadas del monumento

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
      
ojo // aqui falta añadirlas a la estructura
        string ss1 = (*it_restaurantes).at("properties").at("title").get<string>();//Nombre del restaurante
        string ss2 = (*it_restaurantes).at("properties").at("link").get<string>();//link al restaurante
        string ss2 = (*it_restaurantes).at("properties").at("description").get<string>();//Calle del restaurante
        string ss2 = (*it_restaurantes).at("properties").at("category").get<string>();// Numero de tenedores del restaurante aunque tambien hay otras categorias
        double cordx = (*it_restaurantes).at("geometry").at("coordinates")[0].get<double>();//coordenadas del restaurante
        double cordy = (*it_restaurantes).at("geometry").at("coordinates")[1].get<double>();

        ++it_restaurantes;
    }

}
