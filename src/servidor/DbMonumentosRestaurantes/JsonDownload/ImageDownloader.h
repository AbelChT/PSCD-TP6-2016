//*****************************************************************
// File:    ImageDownloader.h
// Author:  PSCD-Unizar
// Date:    11/11/15
// Coms:    Programación de Sistemas Concurrentes y 
//          Distribuidos Curso 2015-2016.
//          Especificación de una librería para descarga de imágenes de Internet
//*****************************************************************
 
class ImageDownloader {

public:

	// CONSTRUCTORES
	// PRE:
	// POST: Inicializa la librería de descarga de imágenes
	ImageDownloader();
	
	// DESTRUCTOR
	// PRE: 
	// POST: Libera el espacio utilizado para la descarga de imágenes. 
	//       Debe llamarse antes de acabar el programa	
	~ImageDownloader();
	
	// MÉTODOS

	// PRE:  URL: URL correspondiente a una imagen
	//       path: Ruta local donde almacenar la imagen
	//		 El método 'cleanup' no ha sido llamado
	// POST: Descarga una imagen de la URL y la almacena en el path local indicado
	// 		 Devuelve 0 en caso de éxito y -1 en caso de error
	int downloadImage(char* URL, char* path);
};
