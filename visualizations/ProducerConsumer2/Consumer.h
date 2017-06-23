/**
 * Consumer.h contains the class necessary in order to create a Consumer object for the Producer-Consumer visualization.
 */
#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <random>
#include <tsgl.h>
#include "Queue.h"
#include "PCThread.h"
using namespace tsgl;

/**
 * Consumer class inherits from the Thread class in order to create a Consumer object.
 * Inheritance: Thread class.
 * Implements the run() method, which calls the consume() method.
 */
class Consumer : public PCThread {
public:
	Consumer(); //Default constructor
	Consumer(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can);  //Explicit constructor
	void consume(); //Consume method
	void run();	//Inheirted from Thread class; function that the pthread should run.
private:
	void removeItem(); //Helps consume() by removing an item from all its locations
	void showArrow(Star * c); //Draws the arrow to show item being read
};

#endif /* CONSUMER_H_ */
