/**
 * main.cpp contains the code that shows the visualization for the Producer-Consumer problem using TSGL and pthreads.
 * It utilizes a custom Queue class to make the shared buffer.
 * Producer and Consumer classes have been made in order to make the Producers and Consumers
 * A Thread class has been made in order to have an encapsulated pthread (which the Producer and Consumer class both inherit from).
 * Usage: ./ProducerConsumer [numberOfProducers] [numberOfConsumers]
 * (Update NOTE: The Canvases may be blank at startup sometimes. If this happens, kill the process in the terminal (using ^C), and
 *	 	         rerun ProducerConsumer.)
 */

#include <stdlib.h>
#include <unistd.h>
#include <tsgl.h>
#include <exception> //for try-catch debugging
#include "PCThread.h"
#include "Producer.h"
#include "Consumer.h"
using namespace tsgl;


//Global constants
const int INNERRAD = 75;  // radius of the inner circle
const int OUTERRAD = 150; // radius of the outercircle
const int CAPACITY = 8;
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 500, MAX_DATA = 8; //Size of Canvas and limit on amount of data to be stored in Queue
Canvas queueDisplay(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Producer-Consumer", FRAME );  //Canvas to draw on
Queue<Star*> sharedBuffer(MAX_DATA, queueDisplay);  //Shared buffer (has colored data)
Canvas legendDisplay(0, WINDOW_HEIGHT+40, WINDOW_WIDTH, 200, "Producer-Consumer Legend", FRAME );


/**
 * displayLegend helps the main method by controlling the legendDisplay
 */
void displayLegend() {
	//Setup Canvas
	legendDisplay.start();
	legendDisplay.setBackgroundColor(WHITE);

	int colorChanger = 0; //Counting int to control random bright colors
	Circle waitingCircle(50, 60, 20, 32, BLACK);
	Circle thinkingCircle(50, 130, 20, 32, BLACK);
	Rectangle waitingSquare(WINDOW_WIDTH-70, 40, 40, 40, BLACK);
	Rectangle thinkingSquare(WINDOW_WIDTH-70, 110, 40, 40, BLACK);
	legendDisplay.add( &waitingCircle );
	legendDisplay.add( &thinkingCircle );
	legendDisplay.add( &waitingSquare );
	legendDisplay.add( &thinkingSquare );

	//TODO: add text
	while( legendDisplay.isOpen() ) {
		waitingCircle.setColor( Colors::highContrastColor(colorChanger) );
		waitingSquare.setColor( Colors::highContrastColor(colorChanger) );
			legendDisplay.sleepFor(1.0);
		colorChanger++;
		if( !queueDisplay.isOpen() )
			legendDisplay.stop(); //Closes and waits for Canvas
	}

	queueDisplay.wait();
	// pthread_exit(0);
}

//Main method
int main(int argc, char * argv[]) {
	int numProducers, numConsumers;  //Number of producers, consumers

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

	Producer * pro = new Producer[numProducers]; //Array of Producers
	Consumer * con = new Consumer[numConsumers];  //Array of Consumers

	//Fire up the visualization
  queueDisplay.start();
	queueDisplay.setBackgroundColor(WHITE);

	queueDisplay.bindToButton(TSGL_SPACE, TSGL_PRESS, []() { // toggle pause when spacebar is pressed
		PCThread::paused = !PCThread::paused;
	});

	//Prepare the display with background items
	int centerY = queueDisplay.getWindowHeight()/2;
	int centerX = queueDisplay.getWindowWidth()/2;
	Line * queueLines[CAPACITY];
	for(int i = 0; i < CAPACITY; i++) {
		float langle = (i*2*PI)/CAPACITY; // line angle
		queueLines[i] = new Line(-INNERRAD*sin(langle)+centerX, INNERRAD*cos(langle)+centerY, -OUTERRAD*sin(langle)+centerX, OUTERRAD*cos(langle)+centerY, BLACK);
		queueDisplay.add(queueLines[i]);
	}

	UnfilledCircle outerQueue(centerX, centerY, OUTERRAD, CAPACITY, BLACK);
	queueDisplay.add(&outerQueue);
	UnfilledCircle innerQueue(centerX, centerY, INNERRAD, CAPACITY, BLACK);
	queueDisplay.add(&innerQueue);

	//TODO: fix text
	// queueDisplay.drawText("Numbers indicate counts", WINDOW_WIDTH-260, WINDOW_HEIGHT-50, 20, BLACK);
	// queueDisplay.drawText("of produced/consumed", WINDOW_WIDTH-235, WINDOW_HEIGHT-30, 20, BLACK);
	//queueDisplay.add(&note1);
	//queueDisplay.add(&note2);

	// Label Readers and Writers
	// queueDisplay.drawText("Producers", 20, 20, 20, BLACK);
	// queueDisplay.drawText("Consumers", WINDOW_HEIGHT-20, 20, 20, BLACK);
	//queueDisplay.add(&proText);
	//queueDisplay.add(&conText);

	//Fill the arrays of Producers and Consumers with Producer and Consumer objects
	for(int i = 0; i < numProducers; i++) {
		pro[i] = Producer(sharedBuffer, i, queueDisplay);
	}

	for(int j = 0; j < numConsumers; j++) {
		con[j] = Consumer(sharedBuffer, j, queueDisplay);
	}

	//Start up the pthreads for Producers and Consumers
	for(int k = 0; k < numProducers; k++) {
		pro[k].start();
		sleep(0.3);
	}
	for(int l = 0; l < numConsumers; l++) {
		con[l].start();
		sleep(0.3);
	}

	//Legend shapes
	//TODO:Add text labeling both circles
	// pthread_t legendThread;
	// pthread_create(&legendThread, NULL, displayLegend, NULL);
	//
	// //Wait for user to exit main Canvas
	// // queueDisplay.wait();
	// //Join the thread controlling legend
	// std::cout << pthread_join(legendThread, NULL) << std::endl;

	displayLegend();

	//Now join them
	for(int p = 0; p < numProducers; p++) {	//Join the pthreads for the Producers
		pro[p].join();
	}

	for(int c = 0; c < numConsumers; c++) {   //Join the pthreads for the Consumers
		con[c].join();
	}


	while( !sharedBuffer.isEmpty() ) {
		Star * tempPtr = sharedBuffer.remove();
		delete tempPtr;
	}

	delete [] pro;
	delete [] con;
	pro = NULL;
	con = NULL;

	return 0;
}
