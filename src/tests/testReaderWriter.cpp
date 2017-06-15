/**
 * testReaderWriter.cpp contains the code to run the Readers-Writers Problem animation using TSGL and POSIX threads.
 * The program utilizes Reader and Writer classes as well as a custom monitor class to hold the shared data.
 * Usage: ./testReaderWriter [numberOfReaders] [numberOfWriters] [preference]
 * [preference] can be 'w' for writer priority or 'r' for reader priority and defaults to writer priority
 */

//#include <omp.h>
//#include <tsgl.h>
#include <unistd.h>
#include <cassert> // assert
#include "ReaderWriter/Reader.h"
#include "ReaderWriter/Writer.h"
#include "ReaderWriter/ColorItem.h"
#include "ReaderWriter/RWMonitor.h"
#include "ReaderWriter/WMonitor.h"
#include "ReaderWriter/RMonitor.h"
#include "../TSGL/tsgl.h"
#include "../TSGL/Canvas.h"
#include "../TSGL/Line.h"
using namespace tsgl;

//Constants
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 800, MARGIN = 45; //Size of Canvas and margin around data

int main(int argc, char* argv[]) {

	//Number of readers is the first argument or defaults to 4
	int numReaders  = ( (argc > 1) && (atoi(argv[1]) > 0) && (atoi(argv[1]) <= 8) ) ? atoi(argv[1]) : 4;
	//Number of writers is the second argument or defaults to 4
	int numWriters = ( (argc > 2) && (atoi(argv[2]) > 0) && (atoi(argv[2]) <= 8) ) ? atoi(argv[2]) : 4;

	//Start Reader-Writer visualization
	Canvas can(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Reader-Writer", 1.0f/2);  //Canvas to draw on

	//Create monitor
	RWMonitor<ColorItem> * monitor;
	string lockString; //String description of lock style
	int max = floor(200 / ColorItem::width) * floor(600 / ColorItem::width);
	if( argc > 3 && *argv[3] == 'r' ) { //Create Reader preference Monitor
		monitor = new RMonitor<ColorItem>(max);
		lockString = "Reader priority";
	} else {							//Create Writer preference Monitor
		monitor = new WMonitor<ColorItem>(max);
		lockString = "Writer priority";
	}

	Reader * readers = new Reader[numReaders]; //Array of Readers
	Writer * writers = new Writer[numWriters]; //Array of Writers

	//Start the visualization
	can.start();
	can.setBackgroundColor(WHITE);

	//Draw labels
	can.drawRectangle(ColorItem::dataX-MARGIN, WINDOW_HEIGHT/12, WINDOW_WIDTH*2/3+MARGIN, ColorItem::dataY, GRAY, true); // draw data area
	can.drawText(lockString, 50, WINDOW_HEIGHT-50, 20, BLACK);
	can.drawText("Numbers indicate", WINDOW_WIDTH-225, WINDOW_HEIGHT-50, 20, BLACK);
	can.drawText("counts of reads/writes", WINDOW_WIDTH-225, WINDOW_HEIGHT-30, 20, BLACK);
	can.drawText("Writers", 20, 20, 20, BLACK);
	can.drawText("Readers", WINDOW_WIDTH-150, 20, 20, BLACK);

	//Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = Reader(*monitor, i, can); //Reader created
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i] = Writer(*monitor, i, can); //Writer created
	}

	srand(time(NULL)); //Seed random number generator for colors and wait times

	//Start the Reader and Writer pthreads
	for(int i = 0; i < numWriters; i++) {
		writers[i].start();
		sleep(0.1);
	}
	sleep(1);
	for(int i = 0; i < numReaders; i++) {
		readers[i].start();
		sleep(0.1);
	}

	can.wait(); //Wait for the main Canvas to be closed

	//End threads
	for(int i = 0; i < numReaders; i++) {
		readers[i].join();
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i].join();
	}

	//Cleanup
	delete [] readers;
	delete [] writers;
	readers = NULL;
	writers = NULL;

	return 0;
}
