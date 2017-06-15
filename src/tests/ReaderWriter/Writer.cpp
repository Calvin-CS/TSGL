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
Writer::Writer(RWMonitor<ColorItem> & sharedMonitor, unsigned long id, Canvas & can) : RWThread(sharedMonitor, id, can) {
	myColor = ColorItem(*myCan, Colors::highContrastColor(id), 0);
	myX = 50; //Set the x-coordinate to 50
	draw(myX, myY);
}

/**
 * \brief write() method generates random colors and adds them in the shared data while the Canvas is open.
 */
void Writer::write() {
	while (myCan->isOpen()) {  //While the Canvas is still open...

		myCan->sleepFor( (rand()%RWThread::WAIT_RANGE+RWThread::WAIT_MIN)/10.0 ); //Wait for a random time

		//Request data access
		draw(myX+75, myY);  //Move in toward data
		data->startWrite(); //Lock data for writing

		//Create and write
		ColorItem color(*myCan, randColor(), randIndex()); //Make random color at random index
		myColor = color;
		draw(myX+130, myY); //Move inside data
		data->write(color, color.getId());  // Write the item to the data
		color.draw(); //Display the color just written

		//Draw an arrow down to the item
		int endX = color.getX()+(ColorItem::width/2), endY = color.getY()+ColorItem::width/2, beginX = myX+150, beginY = myY;
		myCan->drawLine(beginX, beginY, endX, endY, BLACK, false, true); // draw arrow to item drawn
		myCan->sleep();
		myCan->drawLine(beginX, beginY, endX, endY, recentColor, false, true); // erase the arrow

		//Redraw all ColorItems
		for( int i = 0; i < data->getItemCount(); i++) {
			data->read(i).draw();
		}

		//Release lock
		count++; 			//Finished another write
		draw(myX, myY); 	//Return to home location
		data->endWrite(); 	//Unlock the data for writing
	}
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
	int i = rand()%(data->getMaxCapacity()); // random index between 0 and the number of items in data
		if( i > data->getItemCount() )
			i = data->getItemCount();
	return i;
}

/**
 * \brief run() method is the implemented abstract method inherited from the RWThread class.
 * \details Calls the write() method.
 */
void Writer::run() {
	write();
}
