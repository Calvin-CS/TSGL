#include "Writer.h"

/**
 * \brief Default-constructor for the Writer class.
 * \return: The constructed Writer object.
 */
Writer::Writer() : RWThread() {
}

/**
 * \brief Explicit-constructor for the Writer class.
 * \param sharedDatabase, a reference to the RWDatabase object that is shared between the Reader and Writer.
 * \param id, an unsigned long that will be passed to the Thread() constructor that will act as the id for the Thread object.
 * \param can, a handle to the Canvas that will be drawn on.
 * \return: The constructed Writer object.
 */
Writer::Writer(RWDatabase<Rectangle*> & sharedDatabase, unsigned long id, Canvas & can) : RWThread(sharedDatabase, id, can) {
	myX = 50; //Set the x-coordinate to 50
	myCircle->setCenter(myX, myY);
	myCan->add( myCircle );
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

//TODO: comment
void Writer::lock() {
	myCircle->setCenter(myX+75, myY);  //Move in toward data
	data->startWrite(); //Lock data for writing
	myCan->sleepFor(RWThread::access_wait);
}

//TODO: comment
void Writer::act() {
	while( paused ) {}
	int id = randIndex();
	myCircle->setCenter(myX+130, myY); //Move inside data
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
	drawArrow(rec->getX(), rec->getY());
}

//TODO: comment
void Writer::unlock() {
	//Release lock
	count++; 			//Finished another write
	while( paused ) {}
	myCircle->setCenter(myX, myY); 	//Return to home location
	data->endWrite(); 	//Unlock the data for writing
}
