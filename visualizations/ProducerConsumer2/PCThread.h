/**
 * PCThread.h contains the subclass of Thread and superclass of Producer and Consumer for the Producer-Consumer visualization.
 * Includes details for drawing Threads to the Canvas
 */
#ifndef PCTHREAD_H_
#define PCTHREAD_H_

#include <omp.h>
//#include <tsgl.h>
#include "Thread.h"
#include "Queue.h"
#include <tsgl.h>
#include <atomic>
using namespace tsgl;

/**
 * PChread class inherits from the Thread class in order to create a PCThread object.
 * Inheritance: Thread class.
 */
class PCThread : public Thread {
public:
	PCThread(); //Default constructor
	PCThread(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can);  //Explicit constructor
	void draw(); //Draw the PCThread and its count
	virtual void run() = 0;	//Must be implemented by subclass
	static std::atomic<bool> paused;
protected:
	int myX, myY; //Center coordinates for the PCThread
	int count; //Number of colors processed (produced or consumed)
	ColorInt myColor;  //Color data obtained from or added to the shared buffer
	Queue<Star*> * buffer; //Handle to the current buffer
	Canvas * myCan;  //Handle to the Canvas
	Shape * myShape;
};

#endif /* PCTHREAD_H_ */
