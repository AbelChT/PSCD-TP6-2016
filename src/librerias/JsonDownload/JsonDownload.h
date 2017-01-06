//*****************************************************************
// File:    JsonDownload.h
//*****************************************************************

class JsonDownload {

public:

    // CONSTRUCTORES
    // PRE:
    // POST: Inicializa la librer�a de descarga de im�genes
    JsonDownload();

    // DESTRUCTOR
    // PRE:
    // POST: Libera el espacio utilizado para la descarga de im�genes.
    //       Debe llamarse antes de acabar el programa
    ~JsonDownload();

    // M�TODOS

    // PRE:  URL: URL correspondiente a un archivo JSON
    //       path: Ruta local donde almacenar el archivo JSON
    // POST: Descarga un archivo JSON de la URL y la almacena en el path local indicado
    // 		 Devuelve 0 en caso de �xito y -1 en caso de error
    int downloadJSON(char *URL, char *path);
};