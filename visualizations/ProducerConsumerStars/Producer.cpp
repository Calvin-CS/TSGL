#include "Producer.h"

/**
 * Default-constructor for the Producer class.
 * @return: The constructed Producer object.
 */
Producer::Producer() : PCThread() { }

/**
 * Explicit-constructor for the Producer class.
 * @param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue producing object into the Queue.
 * @return: The constructed Producer object.
 */
Producer::Producer(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myX = 50; //Set the x-coordinate to 50
	myShape = new Circle(myX, myY, 20, 32, ColorInt(0, 0, 0));
	myCan->add(myShape);
}

/**
 * showArrow draws an arrow from the Producer to a Star
 * @param: c, a Star pointer for the location of the Arrow
 */
void Producer::showArrow(Star * c) {
	//arrow going from the star to this
	Arrow arrow(myX+30, myY, c->getX(), c->getY());
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

/**
 * randColor generates a new random ColorInt
 */
ColorInt Producer::randColor() {
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;
	return ColorInt(red, green, blue);
}

/**
 * nextItem generates a new Star with a random color
 */
Star* Producer::nextItem() {
	return new Star(myX+50, myY, 20, 5, randColor() );
}

void Producer::wait() {
	myItem = nextItem();
	myShape->setColor( myItem->getColor() );
	PCThread::wait();
}

/**
 * locks the Queue for production
 */
void Producer::lock() {
	myCan->add( myItem );
	myShape->setColor( ColorInt(0, 0, 0) );
	buffer->producerLock();
	while( paused ) {}
}

/**
 * act goes through the process of adding a produced item to Queue
 */
void Producer::act() {
	while( paused ) {}
	myCan->sleep();
	int i = buffer->getLastIndex();
	float itAngle = (i*2*PI + PI)/8; // angle of item
	buffer->append(myItem, getId());  //Append something and pass your id along too

	//Show Item added to Queue
	myItem->setCenter(100*cos(itAngle)+(myCan->getWindowWidth()/2), 100*sin(itAngle)+(myCan->getWindowHeight()/2));
	showArrow(myItem);

	count++;
}

/**
 * unlocks the Queue after production
 */
void Producer::unlock() {
	buffer->producerUnlock(); //TODO: change name of method
	while( paused ) {}
}
