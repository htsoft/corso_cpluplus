// Fibonacci.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "MathLibrary.h"

int main()
{
    // Initialize a Fibonacci relation sequence.
    fibonacci_init(1, 1);
    // Write out the sequence values until overflow.
    do {
        std::cout << fibonacci_index() << ": "
            << fibonacci_current() << std::endl;
    } while (fibonacci_next());
    // Report count of values written before overflow.
    std::cout << fibonacci_index() + 1 <<
        " Fibonacci sequence values fit in an " <<
        "unsigned 64-bit integer." << std::endl;
}

/*
* 1 - Creare un progetto DLL per contenere la libreria
* 2 - Scrivere i file header contenenti nelle dichiarazioni delle funzioni esportate EXTERN 'C' __declspec(dllexport)
* 3 - Implementare le funzioni nel corrispondente file cpp
* 4 - Nel progetto che andrà ad utilizzare la DLL aprire le proprietà di progetto e aggiungere il percorso della cartella contenente i file header nella sezione C++ Generale
* 5 - Nelle proprietà di progetto linker sezione input aggiungere il nome della libreria.lib che referenzia la nostra DLL
* 6 - Nelle proprietà di progetto linker sezione general aggiungere il percorso della cartella contenente la libreria.lib
* 7 - Scrivere il codice che utilizza le funzioni della DLL
* 
* P.S. il file header che definisce le funzioni utilizzabili la DLL deve prevede la seguente dicitura EXTERN 'C' __declspec(dllimport)
*      è possibile guardando l'esempio adoperare il preprocessore per definire una macro che in base al valore di una variabile definisca la dicitura corretta
*/