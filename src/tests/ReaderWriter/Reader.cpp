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
Reader::Reader(RWMonitor<ColorItem> & sharedMonitor, unsigned long id, Canvas & can) : RWThread(sharedMonitor, id, can) {
	myX = can.getWindowWidth()-50;
	draw(myX, myY);
}

/**
 * \brief read() method takes items from the vector and reads them while the Canvas is open.
 */
void Reader::read() {
	while(myCan->isOpen()) { //While the Canvas is still open...

		myCan->sleepFor( (rand()%RWThread::WAIT_RANGE+RWThread::WAIT_MIN)/10.0 ); //Wait for a random time

		//Request data access
		draw(myX-75, myY); //Move towards data
		data->startRead();  //Lock data for reading
		draw(myX-130, myY); //Move inside data
		
		//Read
		myColor = data->read(rand()%data->getItemCount()); //Get the color

		//Draw and erase the arrow
		int beginX = myX-150, beginY = myY, endX = myColor.getX()+(ColorItem::width/2), endY = myColor.getY()+ColorItem::width/2;
		myCan->drawLine(beginX, beginY, endX, endY, BLACK, false, true); // draw arrow to item drawn
		myCan->sleep();
		myCan->drawLine(beginX, beginY, endX, endY, recentColor, false, true); // erase the arrow

		//Redraw all ColorItems
		for(int i = 0; i < data->getItemCount(); i++ ) {
			data->read(i).draw();
		}

		//Release lock
		count++; 			//Finished another read
		draw(myX, myY); 	//Return to home location
		data->endRead(); 	//Unlock the data
	}
}

/**
 * \brief run() method implements the abstract method inherited from the RWThread class. 
 * \details Calls the read() method.
 */
void Reader::run() {
	read();
}
