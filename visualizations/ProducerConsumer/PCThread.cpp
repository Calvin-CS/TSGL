#include "PCThread.h"

std::atomic<bool> PCThread::paused(false);

/**
 * \brief Default-constructor for the PCThread class.
 * \return: The constructed PCThread object.
 */
PCThread::PCThread() : Thread() {
	buffer = NULL;
	myCan = NULL;
	myShape = NULL;
	myItem = NULL;
	myCountLabel = NULL;
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
	count = 99;
	buffer = &sharedBuffer;	//Get the handle to the Queue
	myCan = &can;			//Get the handle to the Canvas
	myY = 50 * (id + 1);
	myX = 0; //Set in subclass constructor
	myItem = NULL;
	myCountLabel = new Text( to_string(count), myX, myY+5, 24, WHITE);
	myCountLabel->setLayer(3);
	myCan->add( myCountLabel );
}

//TODO: comment and improve
void PCThread::wait() {
	myCan->sleepFor( (rand()%10+3.0)/5.0 );
	while( paused ) {}
}

void PCThread::run() {
	while( myCan->isOpen() ) {
		wait();
		lock();
		act();
		unlock();
	}
}

void PCThread::animateItem(int endX, int endY) {
	const int steps = 20;
	const float timeInterval = 0.7;
	int startX = myItem->getX(), startY = myItem->getY();

	Arrow arrow(startX, startY, endX, endY, BLACK, false);
	myCan->add(&arrow);

	float deltaX = (endX - startX) / float(steps); //Change in x each step
	float deltaY = (endY - startY) / float(steps); //Change in y each step

	for(int i = 0; i <= steps; i++) {
		myItem->setCenter( round( startX+ i*deltaX ), round(startY+i*deltaY));
		myCan->sleepFor( timeInterval / steps );
		while( paused ) {}
	}

	myCan->remove(&arrow);
}
