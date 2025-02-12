#include "MutexExample.h"

int contatore = 0;
std::mutex _m;

void incrementa() {
	_m.lock();
	for (int i = 0; i < 1000000; i++) {
		contatore++;
	}
}	

void decrementa() {
	_m.lock();
	for (int i = 0; i < 1000000; i++) {
		contatore--;
	}
}

void MutexExample() {
	std::thread t1(incrementa);
	std::thread t2(decrementa);
	t1.join();
	t2.join();
	std::cout << contatore << std::endl;
}

