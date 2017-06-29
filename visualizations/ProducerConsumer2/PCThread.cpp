#include "PCThread.h"

std::atomic<bool> PCThread::paused(false);

/**
 * \brief Default-constructor for the PCThread class.
 * \return: The constructed PCThread object.
 */
PCThread::PCThread() : Thread() {
	myColor = ColorInt(0,0,0);
	buffer = NULL;
	myCan = NULL;
	count = 0;
	myX = myY = 0;
}

/**
 * \brief Explicit-constructor for the PCThread class.
 * \param: sharedBuffer, a reference to the Queue object that is shared between the Producer and Consumer.
 * \param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param: can, a handle to the Canvas that will be drawn on and will determine whether or not to continue consuming objects from the Queue.
 * \return: The constructed PCThread object.
 */
PCThread::PCThread(Queue<Star*> & sharedBuffer, unsigned long id, Canvas & can) : Thread(id) {
	count = 0;
	myColor = ColorInt(0,0,0);
	buffer = &sharedBuffer;	//Get the handle to the Queue
	myCan = &can;			//Get the handle to the Canvas
	myY = 50 * (id + 1);
	myX = 0; //Set in subclass constructor
}

/**
 * \brief draw() method draws the PCThread onto the Canvas.
 */
void PCThread::draw() {
	myCan->sleep();
	//myCircle = new Circle(myX, myY, 20, 32, myColor);
	//myShape->setColor(myColor);
	myCan->add(myShape);
	// int textSize = 20;
	// if( count > 99 ) textSize = 15;
	// if( count > 999) textSize = 10;
	// myCan->drawText( to_string(count), myX-15, myY+5, textSize, BLACK);
	//myCan->add(&myText); TODO: fix
}
