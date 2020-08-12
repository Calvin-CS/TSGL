#include "Reader.h"

/**
 * \brief Default-constructor for the Reader class.
 * \return: The constructed Reader object.
 */
Reader::Reader() : RWThread() { }

/**
 * \brief Explicit-constructor for the Reader class.
 * \param: sharedData, a reference to the RWDatabase object that is shared between the Reader and Writer.
 * \param: id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param: can, a handle to the Canvas that will be drawn on.
 * \return: The constructed Reader object.
 */
Reader::Reader(RWDatabase<Rectangle*> & sharedDatabase, Lock& lock, unsigned long id, Canvas & can) : RWThread(sharedDatabase, lock, id, can) {
	myX = can.getWindowWidth()/2-50;
	myCircle->setCenter(myX, myY, 0);
	myCountLabel->setCenter(myX, myY, 1);
}

/**
 * \brief Draws and removes Arrow from Canvas
 * \details Helps the read() method
 * \details Includes a half second pause
 */
void Reader::drawArrow(int x, int y) {
	Arrow arrow(x, y, 2, myCircle->getCenterX()-20, myY, 2, 8, 0,0,0, BLACK, false);
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

//TODO: comment
void Reader::lock() {
	myCircle->setCenter(myX-75, myY, 0); //Move towards data
	myCountLabel->setCenter(myX-75, myY, 1);
	monitor->readLock();  //Lock data for reading
	myCan->sleepFor(RWThread::access_wait);
	while( paused ) {}
	myCircle->setCenter(myX-127, myY, 0); //Move inside data
	myCountLabel->setCenter(myX-127, myY, 1);
}

//TODO: comment
void Reader::act() {
	//Read
	Rectangle * rec = data->read(saferand(0, data->getItemCount()-1)); //Get the color
	ColorFloat c = rec->getColor();
	myCircle->setColor( c );
	myCountLabel->setColor( c.getContrast() );

	//Draw and erase the arrow
	drawArrow(rec->getCenterX(), rec->getCenterY());
}

//TODO: comment
void Reader::unlock() {
	//Release lock
	count++; myCountLabel->setText( to_wstring(count) ); //Finished another read
	if( count == 100 ) {
		myCountLabel->setFontSize(22);
	}
	while( paused ) {}
	myCircle->setCenter(myX, myY, 0); 	//Return to home location
	myCountLabel->setCenter(myX, myY, 1);
	monitor->readUnlock(); 	//Unlock the data
}
