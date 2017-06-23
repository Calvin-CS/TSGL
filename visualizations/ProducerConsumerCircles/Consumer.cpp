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
Consumer::Consumer(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myX = can.getWindowWidth() - 50;
	myCircle->setLocation(myX, myY);
	myCan->add(myCircle);
}

//TODO: comment
void Consumer::showArrow(Star * c) {
	//arrow going from the star to this
	Arrow arrow(c->getX(), c->getY(), myX-30, myY);
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

//TODO: comment
void Consumer::lock() {
	buffer->removeLock();
	while( paused ) {}
}

//TODO: comment
void Consumer::act() {
	Star * item = buffer->remove();	//Take out data from the Queue and consume it
	showArrow(item);
	while( paused ) {}
	myCan->remove(item); 							//Remove the same item from the Canvas
	while( paused ) {}
	myCircle->setColor( item->getColor() );
	delete item; 											//Delete the unused pointer to avoid memory leaks
	count++;
}

//TODO: comment
void Consumer::unlock() {
	buffer->removeUnlock();
	while( paused ) {}
}
