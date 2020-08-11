#include "Consumer.h"

/**
 * Default-constructor for the Consumer class.
 * return: The constructed Consumer object.
 */
Consumer::Consumer() : PCThread() { }

/**
 * Explicit-constructor for the Consumer class.
 * param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming object from the Queue.
 * return: The constructed Consumer object.
 */
Consumer::Consumer(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can) : PCThread(sharedBuffer, id, can) {
	myX = can.getWindowWidth()/2 - 50;
	myShape = new Square(myX, myY, 0, 40, 0,0,0, ColorInt(0, 0, 0));
	myShape->setCenter(myX, myY, 0);
	myCountLabel->setCenter(myX, myY, 1);
	myCan->add(myShape);
}

/**
 * locks the Queue for consumption
 */
void Consumer::lock() {
	//Show waiting status
	myShape->setColor( BLACK );
	myCountLabel->setColor(WHITE);
	myCan->remove( myItem );
	delete myItem;
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
	int endX = myShape->getCenterX()-50, endY = myShape->getCenterY();
	animateItem(endX, endY);
	while( paused ) {}
	ColorFloat c = myItem->getColor();
	myShape->setColor( c ); //Change Consumer color to Item color
	myCountLabel->setColor(c.getContrast());
	count++; myCountLabel->setText( std::to_wstring(count) );
	if(count == 10) myCountLabel->setCenter(myX, myY, 1);
	if(count == 100) {
		myCountLabel->setFontSize(22);
		myCountLabel->setCenter(myX, myY, 1);
	}
}

/**
 * unlocks the Queue after consuming an item
 */
void Consumer::unlock() {
	// myCan->remove(myItem);
	// delete myItem;
	buffer->consumerUnlock();
	while( paused ) {}
}

Consumer::~Consumer() {
	// buffer->consumerUnlock();
	// if(myItem) {
	// 	delete myItem;
	// 	myItem = NULL;
	// }
}
