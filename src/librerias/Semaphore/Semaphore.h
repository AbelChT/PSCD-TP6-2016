//*****************************************************************
// File:   Semaphore.h
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Especificación de una clase de semáforos, extendida, con operaciones
//         "wait(nat)" y "signal(nat)"
//         El valor transmitido en la inicialización ha de ser
//         no negativo. Si fuera negativo, da error y aborta la ejecución
//         La semántica que implementa es la que en
//         M. Ben-Ari, "Principles of Concurrent and Distributed Programming", Addison-Wesley, 2006
//         denomina "Busy-wait" (ver pág. 120)
//         Para los casos en que el uso del semáforo va a ser exclusivamente como "mutex", Cpp
//         suministra la clase "std::mutex", cuya especificación se encuentra en 
//         http://en.cppreference.com/w/cpp/thread/mutex
//*****************************************************************

#ifndef SEMAPHORE_H
#define SEMAPHORE_H


#include <mutex>
#include <condition_variable>
#include <assert.h>

using namespace std; //mutex, condition_variable, etc.

class Semaphore{
private:
    mutex mtx;       //los dos primeros atributos se entenderán más adelante
    condition_variable cv;
    int count;       //natural asociado al semáforo  
    bool initialized;//para manejar dos constructores distintos

public:
	//------------------------- constructores
    //Pre:
    //Post: NOT initialized
    Semaphore();

    //Pre: n>=0
    //Post: count=n AND initialized
    Semaphore(int n);

    //Pre: n>=0 AND NOT initialized
    //Post: initialized AND count=n
    void setInitValue(int n);
    //------------------------- operaciones estándar
    //Pre: initialized
    //Post: <count++>
    void signal();

    //Pre: initialized
    //Post: <await count>0 count-- >
    void wait();

    //------------------------- operaciones extendidas
    //Pre: n>0 AND initialized
    //Post: <count=count+n>
    void signal(int n);

    //Pre: n>0 AND initialized
    //Post: <await count>=n count=count-n >
    void wait(int n);
};

#endif 
