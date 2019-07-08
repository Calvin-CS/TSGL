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
	myX = can.getWindowWidth() - 50;
	myShape = new Rectangle(myX, myY, 40, 40, ColorInt(0, 0, 0), BLACK);
	myShape->setCenter(myX, myY);
	myShape->setLayer(1);
	myCountLabel->setBottomLeftCorner(myX - 5, myY + 10);
	myCountLabel->setLayer(2);
	myCan->add(myShape);
}

/**
 * locks the Queue for consumption
 */
void Consumer::lock() {
	//Show waiting status
	myShape->setColor( BLACK, WHITE );
	myCountLabel->setColor(WHITE);
	if( myItem ) {
		myCan->remove( myItem );
		delete myItem;
		myItem = NULL;
	}

	buffer->consumerLock(); //Request lock
	myShape->setColor( WHITE, BLACK );
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
	ColorFloat* fillColor = myItem->getFillColor();
	myShape->setColor( fillColor, BLACK ); //Change Consumer color to Item color
	delete[] fillColor;
	myCountLabel->setColor(BLACK);
	count++; myCountLabel->setText( std::to_wstring(count) );
	if(count == 10) myCountLabel->setBottomLeftCorner(myX - 10, myY + 10);
	if(count == 100) {
		// myCountLabel->setFontSize(20);
		myCountLabel->setBottomLeftCorner(myX - 18, myY + 10);
	}
}

/**
 * unlocks the Queue after consuming an item
 */
void Consumer::unlock() {
	// myCan->remove(myItem);
	// delete myItem;
	// myItem = NULL;
	buffer->consumerUnlock();
	while( paused ) {}
}

Consumer::~Consumer() {
	if(myItem) {
		delete myItem;
		myItem = NULL;
	}
}
