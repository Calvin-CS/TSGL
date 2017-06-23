#include "RWThread.h"

int RWThread::WAIT_RANGE = 40, RWThread::WAIT_MIN = 15;
	// used 32 and 160 for 8 threads and it took forever but worked
	// 40 and 15 worked very well with the new version of ReaderWriter
	// Still need to have these change based on number of threads.
	const int RWThread::width = 20; //Width of each object
	const int RWThread::dataX = 200, RWThread::dataY = 670; //Bottom left coordinates of the data area
	const int RWThread::dataHeight = 600, RWThread::dataWidth = 200; //Dimensions of the data area
	int RWThread::threadCount = 0;
	float RWThread::access_wait = 1.0;
	atomic<bool> RWThread::paused(false);

/**
 * \brief Default-constructor for the RWThread class.
 * \return: The constructed RWThread object.
 */
RWThread::RWThread() : Thread() {
	myX = myY = count = 0;
	data = NULL;
	myCan = NULL;
	myCircle = NULL;
}

/**
 * \brief Explicit-constructor for the RWThread class.
 * \param: sharedMonitor, a reference to the RWMonitor object that is shared between the Reader and Writer.
 * \param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue.
 * \return: The constructed RWThread object.
 */
RWThread::RWThread(RWMonitor<Rectangle*> & sharedMonitor, unsigned long id, Canvas & can) : Thread(id) {
	//Update static variables
	threadCount++;
	access_wait = 1.0/threadCount;

	count = 0;
	data = &sharedMonitor;	//Get the handle to the Monitor
	myCan = &can;						//Get the handle to the Canvas
	myY = 50 * (id + 1) + 40;
	myX = 0; //Set in subclass constructor
	myCircle = new Circle(myX, myY, 20, 32, BLACK); //Move based on new x in subclass
	myCircle->setLayer(2);
	//myText = new Text( count, ...);
	//Text should go on layer 3, between Circle (2) and Arrow (5)
}
