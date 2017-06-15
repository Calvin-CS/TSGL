#include "Consumer.h"

/**
 * Default-constructor for the Consumer class.
 * @return: The constructed Consumer object.
 */
Consumer::Consumer() : Thread() {
	myColor = ColorInt(0,0,0);
	buffer = NULL;
	myCan = NULL;
	numConsumed = 0;
}

/**
 * Explicit-constructor for the Consumer class.
 * @param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue. 
 * @return: The constructed Consumer object.
 */
Consumer::Consumer(Queue<ColorInt> & sharedBuffer, unsigned long id, Canvas & can) : Thread(id) {
	myColor = ColorInt(0, 0, 0);
	buffer = &sharedBuffer;   //Get the address of that buffer and have the handle point to it
	myCan = &can;	//Get the handle to the Canvas
	draw();
	numConsumed = 0;
}

/**
 * consume() method takes something from the buffer and consumes it.
 */
void Consumer::consume() {
	while(myCan->isOpen()) { //While the Canvas is still open...
		sleep( (rand()%10+3)/10 );
		buffer->removeLock();
		int i = buffer->getFirstIndex();
		myColor = buffer->remove();  //Take out data from the Queue and consume it
		myCan->sleep();
		numConsumed++;
		draw(); // draw the color just found 
		// white out the location in drawn buffer
		float itAngle = (i*2*PI + PI)/8; // angle of item
		myCan->drawCircle(100*cos(itAngle)+(myCan->getWindowWidth()/2), -100*sin(itAngle)+(myCan->getWindowHeight()/2), 20, 50, ColorFloat(255, 255, 255), true); // draw the item as a circle
		buffer->removeUnlock();
	}
}

/**
 * draw() method draws the Consumer onto the Canvas.
 * @param: can, a reference to the Canvas object to draw on.
 * @param: windowWidth, an int representing the window width of the Canvas.
 * (in order to draw the Consumer on the right side of the Canvas, I had to take the max width of the Canvas and subtract it by an offset.)
 */
void Consumer::draw() {
	int windowWidth = myCan->getWindowWidth();
	unsigned long offset = getId();   //Get the id of the pthread (the id will act as an offset)
	int x = windowWidth - 50;
	int y = 50 * (offset+1);
	myCan->drawCircle(x, y, 20, 32, myColor);
	myCan->drawText( to_string(numConsumed), x-15, y+5, 20, BLACK);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the consume() method. 
 */
void Consumer::run() {
	consume();
}
