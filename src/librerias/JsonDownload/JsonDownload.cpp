//*****************************************************************
// File:    JsonDownload.cpp
//*****************************************************************

#include "JsonDownload.h"
#include <curl/curl.h>

// PRE:
// POST: Inicializa la librer�a de descarga de im�genes
JsonDownload::JsonDownload() {
    // Inicializamos la libreria de curl
    curl_global_init(CURL_GLOBAL_ALL);
}

// PRE:
// POST: Libera el espacio utilizado para la descarga de im�genes.
//       Debe llamarse antes de acabar el programa
JsonDownload::~JsonDownload() {
    // Limpiamos el espacio usado al finalizar el programa
    curl_global_cleanup();
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {

    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// PRE:  URL: URL correspondiente a un archivo JSON
//       path: Ruta local donde almacenar el archivo JSON
// POST: Descarga un archivo JSON de la URL y la almacena en el path local indicado
// 		 Devuelve 0 en caso de �xito y -1 en caso de error
int JsonDownload::downloadJSON(char *URL, char *path) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(path, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);

        return 0;
    } else
        return -1;
}