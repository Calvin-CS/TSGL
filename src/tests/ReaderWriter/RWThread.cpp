#include "RWThread.h"

const int RWThread::WAIT_RANGE = 2, RWThread::WAIT_MIN = 15;
	// used 32 and 160 for 8 threads and it took forever but worked

/**
 * \brief Default-constructor for the RWThread class.
 * \return: The constructed RWThread object.
 */
RWThread::RWThread() : Thread() {
	myColor = ColorItem();
	data = NULL;
	myCan = NULL;
	count = 0;
	recentX = recentY = myX = myY = 0;
}

/**
 * \brief Explicit-constructor for the RWThread class.
 * \param: sharedMonitor, a reference to the RWMonitor object that is shared between the Reader and Writer.
 * \param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue.
 * \return: The constructed RWThread object.
 */
RWThread::RWThread(RWMonitor<ColorItem> & sharedMonitor, unsigned long id, Canvas & can) : Thread(id) {
	count = recentX = recentY = 0;
	myColor = ColorItem();
	recentColor = can.getBackgroundColor();
	data = &sharedMonitor;	//Get the handle to the Monitor
	myCan = &can;			//Get the handle to the Canvas
	myY = 50 * (id + 1) + 60;
	myX = 0; //Set in subclass constructor
}

/**
 * \brief draw() method draws the RWThread onto the Canvas.
 * \param: x, the x coordinate of the drawing
 * \param: y, the y coordinate of the drawing
 */
void RWThread::draw(int x, int y) {
	myCan->sleep();
	erase();
	recentColor = myCan->getPoint(x, y);
	myCan->drawCircle(x, y, 20, 32, myColor.getColor());
	int textSize = 20;
	if( count > 99 ) textSize = 15;
	if( count > 999) textSize = 10;
	myCan->drawText( to_string(count), x-15, y+5, textSize, BLACK);
	recentX = x;
	recentY = y;
}

/**
 * \brief erase() method erases the RWTHread from the Canvas
 */
void RWThread::erase() {
	myCan->drawCircle(recentX, recentY, 20, 32, recentColor);
}
