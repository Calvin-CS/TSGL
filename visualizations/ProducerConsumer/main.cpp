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
#include "PCThread.h"
#include "Producer.h"
#include "Consumer.h"
using namespace tsgl;


//Global constants
const int INNERRAD = 75;  // radius of the inner circle
const int OUTERRAD = 150; // radius of the outercircle
const int CAPACITY = 8;
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 550, MAX_DATA = 8; //Size of Canvas and limit on amount of data to be stored in Queue
Canvas queueDisplay(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Producer-Consumer", FRAME );  //Canvas to draw on
Queue<Star*> sharedBuffer(MAX_DATA, queueDisplay);  //Shared buffer (has colored data)


/**
 * displayLegend helps the main method by controlling the legendDisplay
 */
void displayLegend(Circle *waitingCircle, Rectangle *waitingSquare, Canvas *queueDisplay) {
	int colorChanger = 0;
	while( queueDisplay->isOpen() ) {
		waitingCircle->setColor( Colors::highContrastColor(colorChanger) );
		waitingSquare->setColor( Colors::highContrastColor(colorChanger) );
			queueDisplay->sleepFor(1.0);
		colorChanger++;
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

	Producer * pro = new Producer[numProducers]; //Array of Producers
	Consumer * con = new Consumer[numConsumers];  //Array of Consumers

	//Fire up the visualization
	queueDisplay.setBackgroundColor(WHITE);
  queueDisplay.start();

	queueDisplay.bindToButton(TSGL_SPACE, TSGL_PRESS, []() { // toggle pause when spacebar is pressed
		PCThread::paused = !PCThread::paused;
	});

	//Prepare the display with background items
	int centerY = 175;
	int centerX = 300;
	Line * queueLines[CAPACITY];
	for(int i = 0; i < CAPACITY; i++) {
		float langle = (i*2*PI)/CAPACITY; // line angle
		queueLines[i] = new Line(-INNERRAD*sin(langle)+centerX, INNERRAD*cos(langle)+centerY, -OUTERRAD*sin(langle)+centerX, OUTERRAD*cos(langle)+centerY, BLACK);
		queueDisplay.add(queueLines[i]);
	}

	UnfilledRegularPolygon outerQueue(centerX, centerY, OUTERRAD, CAPACITY, BLACK);
	queueDisplay.add(&outerQueue);
	UnfilledRegularPolygon innerQueue(centerX, centerY, INNERRAD, CAPACITY, BLACK);
	queueDisplay.add(&innerQueue);

	//Add notes to bottom of main Canvas
	Text note1("*Numbers indicate counts of items produced and consumed", WINDOW_WIDTH-370, WINDOW_HEIGHT-20, 12, BLACK);
	queueDisplay.add(&note1);

	// Label Readers and Writers
	Text proText("Producers", 30, 20, 24, BLACK);
	Text conText("Consumers", WINDOW_WIDTH-150, 20, 24, BLACK);
	queueDisplay.add(&proText);
	queueDisplay.add(&conText);

	//Fill the arrays of Producers and Consumers with Producer and Consumer objects
	for(int i = 0; i < numProducers; i++) {
		pro[i] = Producer(sharedBuffer, i, queueDisplay);
	}

	for(int j = 0; j < numConsumers; j++) {
		con[j] = Consumer(sharedBuffer, j, queueDisplay);
	}

	// displayLegend();



	//LEGENDSTART

	int LEGENDOFFSET = 300;

	int colorChanger = 0; //Counting int to control random bright colors
	Circle waitingCircle(50, 60+LEGENDOFFSET, 20, BLACK); //waiting for lock
	Circle thinkingCircle(50, 120+LEGENDOFFSET, 20, BLACK); //waiting, not seeking lock
	Circle lockCircle(50, 180+LEGENDOFFSET, 20, WHITE); //has lock
	Rectangle waitingSquare(WINDOW_WIDTH-70, 40+LEGENDOFFSET, 40, 40, BLACK);
	Rectangle thinkingSquare(WINDOW_WIDTH-70, 100+LEGENDOFFSET, 40, 40, BLACK);
	Rectangle lockSquare(WINDOW_WIDTH-70, 160+LEGENDOFFSET, 40, 40, WHITE);
	queueDisplay.add( &waitingCircle ); 	queueDisplay.add( &thinkingCircle );
	queueDisplay.add( &lockCircle ); 		queueDisplay.add( &waitingSquare );
	queueDisplay.add( &thinkingSquare );	queueDisplay.add( &lockSquare );

	//Text labels
	Text colorText("producing",100,70+LEGENDOFFSET,24,BLACK);
	Text blackText("waiting for lock",100,130+LEGENDOFFSET,24,BLACK);
	Text whiteText("holding lock",100,190+LEGENDOFFSET,24,BLACK);
	queueDisplay.add( &colorText ); queueDisplay.add( &blackText ); queueDisplay.add( &whiteText );
	Text colorText2("consuming",350,70+LEGENDOFFSET,24,BLACK);
	Text blackText2("waiting for lock",350,130+LEGENDOFFSET,24,BLACK);
	Text whiteText2("holding lock",350,190+LEGENDOFFSET,24,BLACK);
	queueDisplay.add( &colorText2 ); queueDisplay.add( &blackText2 ); queueDisplay.add( &whiteText2 );

	//LEGENDEND

	std::thread legendUpdater (displayLegend, &waitingCircle, &waitingSquare, &queueDisplay);

	//Start up the pthreads for Producers and Consumers
	for(int k = 0; k < numProducers; k++) {
		pro[k].start();
		sleep(0.3);
	}
	for(int l = 0; l < numConsumers; l++) {
		con[l].start();
		sleep(0.3);
	}

	queueDisplay.wait();


	// sleep(0.5);

	//Now join them
	for(int p = 0; p < numProducers; p++) {	//Join the pthreads for the Producers
		pro[p].join();
	}

	for(int c = 0; c < numConsumers; c++) {   //Join the pthreads for the Consumers
		con[c].join();
	}
	legendUpdater.join();

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
