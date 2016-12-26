#include <iostream>
#include <string>
#include "ImageDownloader.h"
using namespace std;
int main(){
  // Creamos el objeto para descargar imágenes
  ImageDownloader downloader;
  // Descargamos una imagen de Internet
  char URL[500] = "http://www.zaragoza.es/georref/json/hilo/ver_Restaurante";
  char path[100] = "restaurantes.json";
  downloader.downloadImage(URL, path);
  return 0;
}
