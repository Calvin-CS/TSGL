#include "Producer.h"

/**
 * Default-constructor for the Producer class.
 * @return: The constructed Producer object.
 */
Producer::Producer() : Thread() {
	myFirst = mySecond = myThird = myX = myY = 0;
	buffer = NULL;
	myCan = NULL;
}	

/**
 * Explicit-constructor for the Producer class.
 * @param: sharedBuffer, a reference to the Queue object that is shared between the Consumer and Producer.
 * @param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * @param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue producing object into the Queue. 
 * @return: The constructed Producer object.
 */
Producer::Producer(Queue<ColorInt> & sharedBuffer, unsigned long id, Canvas & can) : Thread(id) {
	myFirst = mySecond = myThird = myY = 0;
	myX = 50; //Set the x-coordinate to 50
	buffer = &sharedBuffer;  //Get the handle to the buffer and have it point to the buffer
	myCan = &can;  //Get the handle to the Canvas
}

/**
 * produce() method generates a random color and attempts to append it to the shared buffer.
 */
void Producer::produce() {
	while (myCan->isOpen()) {  //While the Canvas is still open...
		myFirst = rand() % 255;  //Generate your data and try to stick it in the Queue
		mySecond = rand() % 255;
		myThird = rand() % 255;
		myColorData = ColorInt(myFirst, mySecond, myThird);
		buffer->append(myColorData, getId());  //Append something and pass your id along too
	} 
}

/**
 * draw() method draws the Producer onto the Canvas.
 * @param: can, a reference to the Canvas object to draw on.
 */
void Producer::draw(Canvas & can) {
	unsigned long id = getId();	//Get the Id of the pthread (the id will act as an offset)
	ColorFloat color = Colors::highContrastColor(id);  //Get the color based off of the id
	myY = myX * (id + 1); //Multiply the center y-coordinate by the id of the pthread and add 1	
	can.drawCircle(myX, myY, 20, 32, color);
}

int Producer::getX() const {
	return myX;
}

int Producer::getY() const {
	return myY;
}

/**
 * run() method is the implemented abstract method inherited from the Thread class. 
 * Calls the produce() method. 
 */
void Producer::run() {
	produce();
}
