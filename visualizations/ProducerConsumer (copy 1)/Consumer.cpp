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

/**
 * consume() method takes something from the buffer and consumes it.
 */
void Consumer::consume() {
	while(myCan->isOpen()) { //While the Canvas is still open...
		//TODO: make random generator make more sense
		sleep( (rand()%10+3)/10 ); //Wait for random time
		while( paused ) {}

		buffer->removeLock(); //TODO: change the name of this lock

		removeItem();

		count++;
		buffer->removeUnlock();
	}
}

//TODO: comment
void Consumer::removeItem() {
	Star * item = buffer->remove();	//Take out data from the Queue and consume it
	showArrow(item);
	while( paused ) {}
	myCan->remove(item); 							//Remove the same item from the Canvas
	while( paused ) {}
	myCircle->setColor( item->getColor() );
	delete item; 											//Delete the unused pointer to avoid memory leaks
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

/**
 * run() method is the implemented abstract method inherited from the Thread class.
 * Calls the consume() method.
 */
void Consumer::run() {
	consume();
}
