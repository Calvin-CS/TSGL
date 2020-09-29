/**
 * main.cpp contains the code that shows the visualization for the Producer-Consumer problem using TSGL and pthreads.
 * It utilizes a custom Queue class to make the shared buffer.
 * Producer and Consumer classes have been made in order to make the Producers and Consumers
 * A Thread class has been made in order to have an encapsulated pthread (which the Producer and Consumer class both inherit from).
 * Usage: ./ProducerConsumer <numberOfProducers> <numberOfConsumers>
 */

#include <stdlib.h>
#include <unistd.h>
#include <tsgl.h>
#include <exception> //for try-catch debugging
#include "Producer.h"
#include "Consumer.h"
using namespace tsgl;


//Global constants
const int INNERRAD = 75;  // radius of the inner circle
const int OUTERRAD = 150; // radius of the outercircle
const int CAPACITY = 8;
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 550, MAX_DATA = 8; //Size of Canvas and limit on amount of data to be stored in Queue
Canvas queueDisplay(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Producer-Consumer", WHITE, nullptr, FRAME );  //Canvas to draw on
Queue<Star*> sharedBuffer(MAX_DATA, queueDisplay);  //Shared buffer (has colored data)


/**
 * displayLegend helps the main method by controlling the legendDisplay
 */
void displayLegend(Circle *waitingCircle, Square *waitingSquare, Canvas *queueDisplay) {
	Star** bufferArray = sharedBuffer.getArray();
	int produceIndex;
	int cap = sharedBuffer.getCapacity();
	int oldFirstIndex = 0;
	ColorFloat prodColor;
	ColorFloat consColor;
	while( queueDisplay->isOpen() ) {
		if(!sharedBuffer.isEmpty()) {
			produceIndex = (sharedBuffer.getLastIndex() + cap - 1) % cap;
			prodColor = bufferArray[produceIndex]->getColor();
			waitingCircle->setColor( prodColor );
		}
		if(oldFirstIndex != sharedBuffer.getFirstIndex()) {
			consColor = bufferArray[oldFirstIndex]->getColor();
			waitingSquare->setColor( consColor );
			oldFirstIndex = sharedBuffer.getFirstIndex();
		}
	}
}

//Main method
int main(int argc, char * argv[]) {
	int numProducers, numConsumers;  //Number of producers, consumers

	if( argc == 1) {
		std::cout << "\nTo run the program with different values, use the format:\n\t./ProducerConsumer [numberOfProducers] [numberOfConsumers]" << std::endl;
	}

	//Check the command line
	numProducers = (argc > 1) ? atoi(argv[1]) : 5; //Producers defaults to 5
	numConsumers = (argc > 2) ? atoi(argv[2]) : 5; //Consumers defaults to 5

	//Set to max number of producers && consumers if negative, zero values
	if(numProducers <= 0 || numConsumers <= 0) {
		std::cout << "Invalid input! Now changing to max default values..." << std::endl;
		numProducers = numConsumers = 8;
	} else if(numProducers > 8 || numConsumers > 8) {
		std::cout << "Too many producers/consumers! Now changing to max default values..." << std::endl;
		numProducers = numConsumers = 8;
	}

	srand(time(NULL)); // seed the random number generator

	Background * queueBackground = queueDisplay.getBackground();
	float centerY = WINDOW_HEIGHT/2 - 175;
	//Fire up the visualization
  	queueDisplay.start();

	queueDisplay.bindToButton(TSGL_SPACE, TSGL_PRESS, []() { // toggle pause when spacebar is pressed
		PCThread::paused = !PCThread::paused;
	});

	queueBackground->drawRegularPolygon(0, centerY, 0, OUTERRAD, CAPACITY, 0,0,0, WHITE, true);
	queueBackground->drawRegularPolygon(0, centerY, 0, INNERRAD, CAPACITY, 0,0,0, WHITE, true);

	//Prepare the display with background items
	for(int i = 0; i < CAPACITY; i++) {
		float langle = (i*2*PI)/CAPACITY; // line angle
		queueBackground->drawLine(-INNERRAD*sin(langle), INNERRAD*cos(langle) + centerY,0, -OUTERRAD*sin(langle), OUTERRAD*cos(langle)+centerY,0, 0,0,0, BLACK);
	}

	const std::string FONT = "./assets/freefont/FreeSerif.ttf";

	//Add notes to bottom of main Canvas
	queueBackground->drawText(0,-WINDOW_HEIGHT/2+20, 0, L"*Numbers indicate counts of items produced and consumed", FONT, 20, 0,0,0, BLACK);
	// Label Readers and Writers
	queueBackground->drawText(-WINDOW_WIDTH/2 + 60, WINDOW_HEIGHT/2-10,0, L"Producers", FONT, 24, 0,0,0, BLACK);
	queueBackground->drawText(WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/2-10,0, L"Consumers", FONT, 24, 0,0,0, BLACK);

	int LEGENDOFFSET = -25;

	//Text labels
	queueBackground->drawText(-140,LEGENDOFFSET-60,0,L"producing",FONT,24, 0,0,0, BLACK);
	queueBackground->drawText(-140,LEGENDOFFSET-120,0,L"waiting for lock",FONT,24, 0,0,0, BLACK);
	queueBackground->drawText(-140,LEGENDOFFSET-180,0,L"holding lock",FONT,24, 0,0,0, BLACK);
	queueBackground->drawText(140,LEGENDOFFSET-60,0,L"consuming",FONT,24, 0,0,0, BLACK);
	queueBackground->drawText(140,LEGENDOFFSET-120,0,L"waiting for lock",FONT,24, 0,0,0, BLACK);
	queueBackground->drawText(140,LEGENDOFFSET-180,0,L"holding lock",FONT,24, 0,0,0, BLACK);

	//Create legend items
	Circle waitingCircle(-WINDOW_WIDTH/2 + 50, LEGENDOFFSET-60, 0, 20, 0,0,0, BLACK); //waiting for lock
	Circle thinkingCircle(-WINDOW_WIDTH/2 + 50, LEGENDOFFSET-120, 0, 20, 0,0,0, BLACK); //waiting, not seeking lock
	Circle lockCircle(-WINDOW_WIDTH/2 + 50, LEGENDOFFSET-180, 0, 20, 0,0,0, BLACK); //has lock
	Square waitingSquare(WINDOW_WIDTH/2-50, LEGENDOFFSET-60, 0, 40, 0,0,0, BLACK);
	Square thinkingSquare(WINDOW_WIDTH/2-50, LEGENDOFFSET-120, 0, 40, 0,0,0, BLACK);
	Square lockSquare(WINDOW_WIDTH/2-50, LEGENDOFFSET-180, 0, 40, 0,0,0, BLACK);
	queueDisplay.add( &waitingCircle ); 	queueDisplay.add( &thinkingCircle );
	queueDisplay.add( &lockCircle ); 		queueDisplay.add( &waitingSquare );
	queueDisplay.add( &thinkingSquare );	queueDisplay.add( &lockSquare );

	//LEGENDEND

	std::thread legendUpdater (displayLegend, &waitingCircle, &waitingSquare, &queueDisplay);

	Producer** pro = new Producer*[numProducers]; //Array of Producers
	Consumer** con = new Consumer*[numConsumers];  //Array of Consumers

	//Fill the arrays of Producers and Consumers with Producer and Consumer objects
	for(int i = 0; i < numProducers; i++) {
		pro[i] = new Producer(sharedBuffer, i, queueDisplay);
	}

	for(int j = 0; j < numConsumers; j++) {
		con[j] = new Consumer(sharedBuffer, j, queueDisplay);
	}

	//Start up the pthreads for Producers and Consumers
	for(int k = 0; k < numProducers; k++) {
		pro[k]->start();
		sleep(0.3);
	}
	for(int l = 0; l < numConsumers; l++) {
		con[l]->start();
		sleep(0.3);
	}

	queueDisplay.wait();

	while( !sharedBuffer.isEmpty() ) {
		Star * tempPtr = sharedBuffer.remove();
		delete tempPtr;
	}

	//Now join them
	for(int p = 0; p < numProducers; p++) {	//Join the pthreads for the Producers
		pro[p]->join();
		delete pro[p];
	}

	for(int c = 0; c < numConsumers; c++) {   //Join the pthreads for the Consumers
		con[c]->join();
		delete con[c];
	}

	legendUpdater.join();

	delete [] pro;
	delete [] con;
	pro = NULL;
	con = NULL;

	return 0;
}
