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
Producer::Producer(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myFirst = mySecond = myThird  = 0;
	myX = 50; //Set the x-coordinate to 50
	myShape = new Circle(myX, myY, 20, 32, myColor);
	draw();
}

/**
 * produce() method generates a random color and attempts to append it to the shared buffer.
 */
void Producer::produce() {
	while (myCan->isOpen()) {  //While the Canvas is still open...
		sleep( (rand()%10+3)/10 );
		while( paused ) { }
		myColor = randColor();
		buffer->appendLock();
		int i = buffer->getLastIndex();
		count++;
		draw();
		while( paused ) { }
		float itAngle = (i*2*PI + PI)/8; // angle of item
		myCan->sleep();
		Star * item = new Star(100*cos(itAngle)+(myCan->getWindowWidth()/2), 100*sin(itAngle)+(myCan->getWindowHeight()/2), 20, 5, myColor, false); // draw the item as a star
		myCan->add(item);
		while( paused ) { }

		//update our Circle's color TODO: fix when the text is back or we make this better, needs to be cleaner
		//TODO: mainly waiting for all Shapes to have change color methods
		myCan->remove(myShape);
		while( paused ) { }
		delete myShape;
		myShape = new Circle(myX, myY, 20, 32, myColor);
		myCan->add(myShape);

		buffer->append(item, getId());  //Append something and pass your id along too
		showArrow(item);
		buffer->appendUnlock();
	}
}

//TODO: comment
void Producer::showArrow(Star * c) {
	//arrow going from the star to us
	Arrow arrow(myX+30, myY, c->getX(), c->getY());
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) { }
	myCan->remove(&arrow);
}

//TODO: comment
ColorInt Producer::randColor() {
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;
	return ColorInt(red, green, blue);
}

/**
 * run() method is the implemented abstract method inherited from the Thread class.
 * Calls the produce() method.
 */
void Producer::run() {
	produce();
}
