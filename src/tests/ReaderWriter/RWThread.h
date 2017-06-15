/**
 * RWThread.h contains the subclass of Thread and superclass of Reader and Writer for the Reader-Writer visualization.
 * Includes details for drawing Threads to the Canvas
 */
#ifndef RWTHREAD_H_
#define RWTHREAD_H_

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
 * RWThread class inherits from the Thread class in order to create a RWThread object. 
 * Inheritance: Thread class.
 */
class RWThread : public Thread {
public:
	RWThread(); //Default constructor
	RWThread(RWMonitor<ColorItem> & sharedMonitor, unsigned long id, Canvas & can);  //Explicit constructor
	virtual void run() = 0;	//Must be implemented by subclass
protected:
	void draw(int x, int y);  //Draw the RWThread onto the Canvas
	void erase(); //Remove RWThread from Canvas
	int myX, myY; //Center coordinates for the RWThread
	int count; //Number of colors processed (read or written)
	int recentX, recentY; //Coordinates of most recent location
	ColorInt recentColor; //Color to return to when erasing
	ColorItem myColor;  //Color data obtained from the shared monitor
	RWMonitor<ColorItem> * data; //Handle to the current monitor
	Canvas * myCan;  //Handle to the Canvas
	static const int WAIT_MIN, WAIT_RANGE;
};

#endif /* RWTHREAD_H_ */
