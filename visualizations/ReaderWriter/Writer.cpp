#include "Writer.h"

/**
 * \brief Default-constructor for the Writer class.
 * \return: The constructed Writer object.
 */
Writer::Writer() : RWThread() {
}

/**
 * \brief Explicit-constructor for the Writer class.
 * \param sharedMonitor, a reference to the RWMonitor object that is shared between the Reader and Writer.
 * \param id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param can, a handle to the Canvas that will be drawn on.
 * \return: The constructed Writer object.
 */
Writer::Writer(RWMonitor<Rectangle*> & sharedMonitor, unsigned long id, Canvas & can) : RWThread(sharedMonitor, id, can) {
	myX = 50; //Set the x-coordinate to 50
	myCircle->setLocation(myX, myY);
	myCan->add( myCircle );
}

/**
 * \brief write() method generates random colors and adds them in the shared data while the Canvas is open.
 */
void Writer::write() {
	while (myCan->isOpen()) {  //While the Canvas is still open...

		while( paused ) {}

		//Request data access
		myCircle->setLocation(myX+75, myY);  //Move in toward data
		data->startWrite(); //Lock data for writing
		myCan->sleepFor(RWThread::access_wait);

		//Create and write
		int id = randIndex();
		while( paused ) {}
		myCircle->setLocation(myX+130, myY); //Move inside data
		Rectangle * rec;
		if( id < data->getItemCount() ) { //Change the color of an item
			rec = data->read(id);
			rec->setColor(randColor());
		} else { //Create a new item
			rec = makeRec(id); //Make random color at random index
			data->write(rec, id);  // Write the item to the data
			myCan->add(rec);
		}
		myCircle->setColor( rec->getColor() );

		//Draw an arrow down to the item
		int endX = rec->getX()+(RWThread::width/2), endY = rec->getY()+RWThread::width/2;
		drawArrow(endX, endY);

		//Release lock
		count++; 			//Finished another write
		while( paused ) {}
		myCircle->setLocation(myX, myY); 	//Return to home location
		data->endWrite(); 	//Unlock the data for writing

		myCan->sleepFor( (rand()%RWThread::WAIT_RANGE+RWThread::WAIT_MIN)/10.0 ); //Wait for a random time

	}
}

/**
 * \brief Draws and removes Arrow from Canvas
 * \details Helps the write() method
 * \details Includes a half second pause
 */
void Writer::drawArrow(int x, int y) {
	Arrow arrow(myX+150, myY, x, y);
	arrow.setLayer(5);
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

/**
 * \brief newColor() generates a new random color for writing.
 */
ColorInt Writer::randColor() {
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;
	return ColorInt(red, green, blue);
}

/**
 * \brief randIndex() generates an index number to add a new item
 */
int Writer::randIndex() {
	int i = rand()%(data->getMaxCapacity()); //Random index between 0 and the max number of items in data
		if( i > data->getItemCount() ) //Max index is next empty index
			i = data->getItemCount();
	return i;
}

/**
 * \brief makeRec() creates a new Rectangle representing a random color
 */
Rectangle * Writer::makeRec(int index) {
	int x = dataX + index%(200/RWThread::width) * RWThread::width; // start of data + column
	int y = dataY - (index/(200/RWThread::width) + 1) * RWThread::width; // start of data + row

	return new Rectangle(x, y, RWThread::width, RWThread::width, randColor());
}

/**
 * \brief run() method is the implemented abstract method inherited from the RWThread class.
 * \details Calls the write() method.
 */
void Writer::run() {
	write();
}
