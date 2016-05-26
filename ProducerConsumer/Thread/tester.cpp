
#include "Thread.h"
#include <iostream>

int main() {
	const int SIZE = 8;	
	Thread t;
	t.start();
	std::cout << "Thread created!\n";
	t.join();
	std::cout << "Thread joined!\n";
	Thread t1(0);
	t1.start();	
	std::cout << "Thread 2 created!\n";
	t1.join();
	std::cout << "Thread 2 joined!\n";
	Thread * children = new Thread[SIZE];	
	for(int i = 0; i < SIZE; i++) {
		children[i] = Thread(i);
	}
	
	std::cout << "Threads spawned!\n";
	
	for(int j = 0; j < SIZE; j++) {
		children[j].start();	
	}
	
	std::cout << "Threads created!\n";
	
	for(int l = 0; l < SIZE; l++) {
		children[l].join();	
	}
	
	std::cout << "Threads joined!\n";
}
