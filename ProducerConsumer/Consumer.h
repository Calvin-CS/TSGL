/**
 * Consumer.h contains the class necessary in order to create a Consumer object for the Producer-Consumer visualization.
 */
#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <omp.h>
#include <random>
#include <tsgl.h>
#include "Queue.h"
#include "Thread.h"
using namespace tsgl;

/**
 * Consumer class inherits from the Thread class in order to create a Consumer object. 
 * Inheritance: Thread class.
 * Implements the run() method, which calls the consume() method.
 */
class Consumer : public Thread {
public:
	Consumer(); //Default constructor
	Consumer(Queue<ColorInt> & sharedBuffer, unsigned long id, Canvas & can);  //Explicit constructor
	void consume(); //Consume method
	void draw(Canvas & can, int windowWidth);  //Draw the Consumer onto the Canvas and id of pthread
	void run();	//Inheirted from Thread class; function that the pthread should run.
private:
	ColorInt myColor;  //Color data obtained from the shared buffer
	Queue<ColorInt> * buffer; //Handle to the current buffer
	Canvas * myCan;  //Handle to the Canvas
};

#endif /* CONSUMER_H_ */
