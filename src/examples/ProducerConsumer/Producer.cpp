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
	myShape = new Circle(myX, myY, 20, ColorInt(0, 0, 0), BLACK);
	myShape->setLayer(1);
	myCountLabel->setBottomLeftCorner(myX - 5, myY + 10);
	myCountLabel->setLayer(2);
	myCan->add(myShape);
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

/**
 * wait takes some time to find the next color
 */
void Producer::wait() {
	myItem = nextItem();
	ColorFloat * fillColor = myItem->getFillColor();
	myShape->setColor( fillColor, BLACK );
	myCountLabel->setColor(fillColor->getContrast());
	delete[] fillColor;
	PCThread::wait();
}

/**
 * locks the Queue for production
 */
void Producer::lock() {
	myCan->add( myItem );
	myShape->setColor( BLACK, WHITE );
	myCountLabel->setColor(WHITE);
	buffer->producerLock();
	myShape->setColor( WHITE, BLACK );
	myCountLabel->setColor(BLACK);
	while( paused ) {}
}

/**
 * act goes through the process of adding a produced item to Queue
 */
void Producer::act() {
	while( paused ) {}
	myCan->sleep();
	int i = buffer->getLastIndex();
	buffer->append(myItem, getId());  //Append something and pass your id along too
	//Show Item added to Queue
	float itAngle = (i*2*PI + PI)/8; // angle of item
	int endX = 100*cos(itAngle)+300, endY = 100*sin(itAngle)+175;
	animateItem(endX, endY);

	count++; myCountLabel->setText( std::to_wstring(count) );
	if(count == 10) myCountLabel->setCenter(myX, myY);
	if(count == 100) {
		myCountLabel->setFontSize(22);
		myCountLabel->setCenter(myX, myY);
	}
	myItem = NULL;
}

/**
 * unlocks the Queue after production
 */
void Producer::unlock() {
	buffer->producerUnlock(); //TODO: change name of method
	while( paused ) {}
}

Producer::~Producer() {
	if(myItem) {
		delete myItem;
		myItem = NULL;
	}
}