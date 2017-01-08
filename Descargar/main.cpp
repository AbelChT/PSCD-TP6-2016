#include "JsonDownload.h"

int main(){
  JsonDownload downloader;
  //Funciona
  char URL_MONUMENTOS[500] = "http://www.zaragoza.es/georref/json/hilo/verconsulta_Piezas?georss_tag_1=-&georss_materiales=-&georss_tematica=-&georss_barrio=-&georss_epoca=-";
  /*
  * No funciona
  * www.zaragoza.es/buscador/select?wt=json&q=*:* AND -tipocontenido_s:estatico AND category:(\"Arte Pru00fablico\")&fl=uri,title,emplazamiento_smultiple,barrio_smultiple,epoca_smultiple,autor_smultiple,x_coordinate,y_coordinate&rows=400
  * http://www.zaragoza.es/api/recurso/turismo/restaurante?fl=geometry&rf=html&results_only=true&srsname=utm30n&rows=0
  */

  char path_MONUMENTOS[100] = "monumentos.json";

  downloader.downloadJSON(URL_MONUMENTOS, path_MONUMENTOS);//descargo los monumentos
  return 0;
}
