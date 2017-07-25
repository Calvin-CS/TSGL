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
	myShape = new Rectangle(myX, myY, 40, 40, ColorInt(0, 0, 0));
	myShape->setCenter(myX, myY);
	myCountLabel->setCenter(myX, myY);
	myCan->add(myShape);
}

/**
 * locks the Queue for consumption
 */
void Consumer::lock() {
	//Show waiting status
	myShape->setColor( BLACK );
	myCountLabel->setColor(WHITE);
	if( myItem ) {
		myCan->remove( myItem );
		delete myItem;
	}

	buffer->consumerLock(); //Request lock
	myShape->setColor( WHITE );
	myCountLabel->setColor(BLACK);
	while( paused ) {}
}

/**
 * act goes through the process of consuming an item from Queue
 */
void Consumer::act() {
	myItem = buffer->remove();	//Take out data from the Queue and consume it
	int endX = myShape->getX()-50, endY = myShape->getY();
	animateItem(endX, endY);
	while( paused ) {}
	myShape->setColor( myItem->getColor() ); //Change Consumer color to Item color
	myCountLabel->setColor(BLACK);
	count++; myCountLabel->setString( to_string(count) );
	if(count == 10) myCountLabel->setCenter(myX, myY);
	if(count == 100) {
		myCountLabel->setFontSize(20);
		myCountLabel->setCenter(myX, myY);
	}
}

/**
 * unlocks the Queue after consuming an item
 */
void Consumer::unlock() {
	buffer->consumerUnlock();
	while( paused ) {}
}
