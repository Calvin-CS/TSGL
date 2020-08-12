#include "Writer.h"
Text * Writer::dataLabel = NULL;

/**
 * \brief Default-constructor for the Writer class.
 * \return: The constructed Writer object.
 */
Writer::Writer() : RWThread() { dataLabel = NULL; }

/**
 * \brief Explicit-constructor for the Writer class.
 * \param sharedDatabase, a reference to the RWDatabase object that is shared between the Reader and Writer.
 * \param id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param can, a handle to the Canvas that will be drawn on.
 * \return: The constructed Writer object.
 */
Writer::Writer(RWDatabase<Rectangle*> & sharedDatabase, Lock& lock, unsigned long id, Canvas & can) : RWThread(sharedDatabase, lock, id, can) {
	myX = -can.getWindowWidth()/2 + 50; //Set the x-coordinate to 50
	myCircle->setCenter(myX, myY, 0);
	myCountLabel->setCenter(myX, myY, 1);
	if( !dataLabel ) {
		dataLabel = new Text(RWThread::dataX - 20, RWThread::dataY+RWThread::dataHeight+15, 1, L"0/300", "./assets/freefont/FreeSans.ttf", 16, 0,0,0, BLACK);
		myCan->add( dataLabel );
	}
}

/**
 * \brief Draws and removes Arrow from Canvas
 * \details Helps the write() method
 * \details Includes a half second pause
 */
void Writer::drawArrow(int x, int y) {
	Arrow arrow(x, y, 2, myCircle->getCenterX()+20, myY, 2, 8, 0,0,0, BLACK);
	myCan->add(&arrow);
	myCan->sleepFor(0.5);
	while( paused ) {}
	myCan->remove(&arrow);
}

/**
 * \brief newColor() generates a new random color for writing.
 */
ColorInt Writer::randColor() {
	int red = saferand(0,255);
	int green = saferand(0,255);
	int blue = saferand(0,255);
	return ColorInt(red, green, blue);
}

/**
 * \brief randIndex() generates an index number to add a new item
 */
int Writer::randIndex() {
	int i = saferand(0,data->getMaxCapacity()); //Random index between 0 and the max number of items in data
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

	return new Rectangle(x, y, 0, RWThread::width, RWThread::width, 0,0,0, randColor());
}

//TODO: comment
void Writer::lock() {
	myCircle->setCenter(myX+75, myY, 0);  //Move in toward data
	myCountLabel->setCenter(myX+75, myY, 1);
	monitor->writeLock(); //Lock data for writing
	myCan->sleepFor(RWThread::access_wait);
}

//TODO: comment
void Writer::act() {
	while( paused ) {}
	int id = randIndex();
	myCircle->setCenter(myX+127, myY, 0); //Move inside data
	myCountLabel->setCenter(myX+127, myY, 1);
	Rectangle * rec;
	if( id < data->getItemCount() ) { //Change the color of an item
		rec = data->read(id);
		rec->setColor(randColor());
	} else { //Create a new item
		rec = makeRec(id); //Make random color at random index
		data->write(rec, id);  // Write the item to the data
		myCan->add(rec);
		dataLabel->setText( to_wstring( data->getItemCount() ) + L"/" + to_wstring( data->getMaxCapacity() ) );
	}
	ColorFloat c = rec->getColor();
	myCircle->setColor( c );
	myCountLabel->setColor(c.getContrast());

	//Draw an arrow down to the item
	drawArrow(rec->getCenterX(), rec->getCenterY());
}

//TODO: comment
void Writer::unlock() {
	//Release lock
	count++; myCountLabel->setText( to_wstring(count) ); //Finished another write
	if( count == 100 ) {
		myCountLabel->setFontSize(22);
	}
	while( paused ) {}
	myCircle->setCenter(myX, myY, 0); 	//Return to home location
	myCountLabel->setCenter(myX, myY, 1);
	monitor->writeUnlock(); 	//Unlock the data for writing
}
