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
	myShape->setCenter(myX, myY);
	myCan->add(myShape);
}

//TODO: comment
void Producer::showArrow(Star * c) {
	//arrow going from the star to this
	Arrow arrow(myX+30, myY, c->getX(), c->getY());
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

//TODO: comment
ColorInt Producer::randColor() {
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;
	return ColorInt(red, green, blue);
}

//TODO: comment
void Producer::lock() {
	buffer->appendLock();
	while( paused ) {}
}

//TODO: comment
void Producer::act() {
	int i = buffer->getLastIndex();
	count++;
	while( paused ) {}
	ColorInt newColor = randColor();
	myShape->setColor( newColor );
	float itAngle = (i*2*PI + PI)/8; // angle of item
	myCan->sleep();
	Star * item = new Star(100*cos(itAngle)+(myCan->getWindowWidth()/2), 100*sin(itAngle)+(myCan->getWindowHeight()/2), 20, 5, myShape->getColor(), false); // draw the item as a star
	while( paused ) {}
	myCan->add(item);
	buffer->append(item, getId());  //Append something and pass your id along too
	showArrow(item);
}

//TODO: comment
void Producer::unlock() {
	buffer->appendUnlock(); //TODO: change name of method
	while( paused ) {}
}
