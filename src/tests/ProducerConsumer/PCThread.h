/**
 * PCThread.h contains the subclass of Thread and superclass of Producer and Consumer for the Producer-Consumer visualization.
 * Includes details for drawing Threads to the Canvas
 */
#ifndef PCTHREAD_H_
#define PCTHREAD_H_

#include <omp.h>
//#include <tsgl.h>
#include "RWMonitor.h"
#include "WMonitor.h"
#include "Thread.h"
#include "ColorItem.h"
#include "../../TSGL/tsgl.h"
#include "../../TSGL/Canvas.h"
#include "../../TSGL/Line.h"
using namespace tsgl;

/**
 * PChread class inherits from the Thread class in order to create a PCThread object.
 * Inheritance: Thread class.
 */
class PCThread : public Thread {
public:
	PCThread(); //Default constructor
	PCThread(RWMonitor<ColorItem> & sharedMonitor, unsigned long id, Canvas & can);  //Explicit constructor
	virtual void run() = 0;	//Must be implemented by subclass
protected:
	void draw(int x, int y);  //Draw the PCThread onto the Canvas
	void erase(); //Remove PCThread from Canvas
	int myX, myY; //Center coordinates for the PCThread
	int count; //Number of colors processed (read or written)
	int recentX, recentY; //Coordinates of most recent location
	ColorInt recentColor; //Color to return to when erasing
	ColorItem myColor;  //Color data obtained from the shared monitor
	RWMonitor<ColorItem> * data; //Handle to the current monitor
	Canvas * myCan;  //Handle to the Canvas
	static const int WAIT_MIN, WAIT_RANGE;
};

#endif /* PCTHREAD_H_ */
