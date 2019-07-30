/**
 * RWThread.h contains the subclass of Thread and superclass of Reader and Writer for the Reader-Writer visualization.
 * Includes details for drawing Threads to the Canvas
 */
#ifndef RWTHREAD_H_
#define RWTHREAD_H_

#include <omp.h>
#include <tsgl.h>
#include <atomic> //atomic<bool> paused
#include "RWDatabase.h"
#include "Thread.h"
#include "Lock.h"
using namespace tsgl;

/**
 * RWThread class inherits from the Thread class in order to create a RWThread object.
 * Inheritance: Thread class.
 */
class RWThread : public Thread {
public:
	RWThread(); //Default constructor
	RWThread(RWDatabase<Rectangle*> & sharedDatabase, Lock& lock, unsigned long id, Canvas & can);  //Explicit constructor
	void run();
	void wait();
	virtual void lock() = 0; //Must be implemented by subclass
	virtual void act() = 0; //Must be implemented by subclass
	virtual void unlock() = 0; //Must be implemented by subclass
	static const int width, dataX, dataY, dataHeight, dataWidth; //constants for display
	static int WAIT_MIN, WAIT_RANGE;
	static float access_wait;
	static atomic<bool> paused;
protected:
	int myX, myY; //Center coordinates for the RWThread
	int count; //Number of colors processed (read or written)
	RWDatabase<Rectangle*> * data; //Handle to the current database
	Lock* monitor; //Handle to the current monitor
	Canvas * myCan;  //Handle to the Canvas
	Circle * myCircle; //Circle representing the Thread
	Text * myCountLabel; //Text label for number processed

	//Static values
	static int threadCount;
};

#endif /* RWTHREAD_H_ */
