#include "Consumer.h"

/**
 * Default-constructor for the Consumer class.
 * @return: The constructed Consumer object.
 */
Consumer::Consumer() : PCThread() { }

/**
 * Explicit-constructor for the Consumer class.
 * @param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue.
 * @return: The constructed Consumer object.
 */
Consumer::Consumer(Queue<ColorInt> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myX = can.getWindowWidth() - 50;
	draw();
}

/**
 * consume() method takes something from the buffer and consumes it.
 */
void Consumer::consume() {
	while(myCan->isOpen()) { //While the Canvas is still open...
		//TODO: make random generator make more sense
		sleep( (rand()%10+3)/10 ); //Wait for random time

		buffer->removeLock();

		int i = buffer->getFirstIndex();
		myColor = buffer->remove();  //Take out data from the Queue and consume it
		myCan->sleep();
		count++;
		draw(); // draw the color just found
		// white out the location in drawn buffer
		float itAngle = (i*2*PI + PI)/8; // angle of item
		Circle item(100*cos(itAngle)+(myCan->getWindowWidth()/2), -100*sin(itAngle)+(myCan->getWindowHeight()/2), 20, 50, ColorFloat(255, 255, 255), true); // draw the item as a circle
		myCan.add(&item);
		buffer->removeUnlock();
	}
}

/**
 * run() method is the implemented abstract method inherited from the Thread class.
 * Calls the consume() method.
 */
void Consumer::run() {
	consume();
}
