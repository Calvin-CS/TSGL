#include "Producer.h"

/**
 * Default-constructor for the Producer class.
 * @return: The constructed Producer object.
 */
Producer::Producer() : PCThread() {
	myFirst = mySecond = myThird = 0;
}

/**
 * Explicit-constructor for the Producer class.
 * @param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue producing object into the Queue.
 * @return: The constructed Producer object.
 */
Producer::Producer(Queue<ColorInt> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myFirst = mySecond = myThird  = 0;
	myX = 50; //Set the x-coordinate to 50
	draw();
}

/**
 * produce() method generates a random color and attempts to append it to the shared buffer.
 */
void Producer::produce() {
	while (myCan->isOpen()) {  //While the Canvas is still open...
		sleep( (rand()%10+3)/10 );
		myFirst = rand() % 255;  //Generate your data and try to stick it in the Queue
		mySecond = rand() % 255;
		myThird = rand() % 255;
		myColor = ColorInt(myFirst, mySecond, myThird);
		buffer->appendLock();
		int i = buffer->getLastIndex();
		buffer->append(myColor, getId());  //Append something and pass your id along too
		count++;
		draw();
		float itAngle = (i*2*PI + PI)/8; // angle of item
		myCan->sleep();
		Circle item(100*cos(itAngle)+(myCan->getWindowWidth()/2), -100*sin(itAngle)+(myCan->getWindowHeight()/2), 20, 50, myColor, true); // draw the item as a circle
		myCan.add(&item);
		buffer->appendUnlock();
	}
}

/**
 * run() method is the implemented abstract method inherited from the Thread class.
 * Calls the produce() method.
 */
void Producer::run() {
	produce();
}
