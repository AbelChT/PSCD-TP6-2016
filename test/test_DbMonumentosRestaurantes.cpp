/******************************************************************
* Author: Abel Chils Trabanco
*         Jorge Aznar López
*         Daniel Fraile Belmonte
* Date: 2 de enero de 2017
******************************************************************/

#include <string>
#include <iostream>
#include <time.h>
#include "../src/servidor/DbMonumentosRestaurantes/DbMonumentosRestaurantes.h"

using namespace std;

int main(){
    clock_t t;
    cout << "Programa iniciado. Cargando base de datos....." << endl;
    t = clock();
    DbMonumentosRestaurantes prueba(5);
    t = clock() - t;
    cout << "--Base de datos cargada, programa listo para utilizarse" << endl;

    cout<<"--Para cargar el procesador ha usado "<<t<<" clicks ("<< ((float)t)/CLOCKS_PER_SEC<<" segundos)"<<endl<< endl;

    string monumento;
    Lista<Monumento*> list;
    Monumento * aux;
    while(true){
      cout<<"Escribe un monumento a buscar: "<<flush;
      getline(cin,monumento);
      //cin>>monumento;
      t = clock();
      prueba.buscarMonumento(monumento,list);
      t = clock() - t;
      cout <<"Lista de monumentos con coincidencia: "<<endl;
      list.begin();
      while (list.next(aux)) {
        cout<<aux->getlink()<<endl;
      }
      cout << "TOTAL: "<<list.size()<<endl;
      cout<<"--Para buscar el procesador ha usado "<<t<<" clicks ("<< ((float)t)/CLOCKS_PER_SEC<<" segundos)"<<endl<< endl;
    }
    return 0;
}
