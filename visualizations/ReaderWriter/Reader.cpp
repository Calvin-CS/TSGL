#include "Reader.h"

/**
 * \brief Default-constructor for the Reader class.
 * \return: The constructed Reader object.
 */
Reader::Reader() : RWThread() { }

/**
 * \brief Explicit-constructor for the Reader class.
 * \param: sharedData, a reference to the RWMonitor object that is shared between the Reader and Writer.
 * \param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param: can, a handle to the Canvas that will be drawn on.
 * \return: The constructed Reader object.
 */
Reader::Reader(RWMonitor<Rectangle*> & sharedMonitor, unsigned long id, Canvas & can) : RWThread(sharedMonitor, id, can) {
	myX = can.getWindowWidth()-50;
	myCircle->setLocation(myX, myY);
	myCan->add( myCircle );
}

/**
 * \brief read() method takes items from the vector and reads them while the Canvas is open.
 */
void Reader::read() {
	while(myCan->isOpen()) { //While the Canvas is still open...

		while( paused ) {}

		//Request data access
		myCircle->setLocation(myX-75, myY); //Move towards data
		data->startRead();  //Lock data for reading
		myCan->sleepFor(RWThread::access_wait);
		while( paused ) {}
		myCircle->setLocation(myX-130, myY); //Move inside data

		//Read
		Rectangle * rec = data->read(rand()%data->getItemCount()); //Get the color
		myCircle->setColor( rec->getColor() );

		//Draw and erase the arrow
		int endX = rec->getX()+(RWThread::width/2), endY = rec->getY()+RWThread::width/2; //Calculate center coordinates of the item
		drawArrow(endX, endY);

		//Release lock
		count++; 			//Finished another read
		while( paused ) {}
		myCircle->setLocation(myX, myY); 	//Return to home location
		data->endRead(); 	//Unlock the data

		myCan->sleepFor( (rand()%RWThread::WAIT_RANGE+RWThread::WAIT_MIN)/10.0 ); //Wait for a random time

	}
}

/**
 * \brief Draws and removes Arrow from Canvas
 * \details Helps the read() method
 * \details Includes a half second pause
 */
void Reader::drawArrow(int x, int y) {
	Arrow arrow(myX-150, myY, x, y);
	arrow.setLayer(5);
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

/**
 * \brief run() method implements the abstract method inherited from the RWThread class.
 * \details Calls the read() method.
 */
void Reader::run() {
	read();
}
