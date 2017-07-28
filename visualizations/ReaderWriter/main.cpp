/**
 * testReaderWriter.cpp contains the code to run the Readers-Writers Problem animation using TSGL and POSIX threads.
 * The program utilizes Reader and Writer classes as well as a custom monitor class to hold the shared data.
 * Usage: ./ReaderWriter [numberOfReaders] [numberOfWriters] [preference] [starved]
 * [preference] can be 'w' for writer priority or 'r' for reader priority and defaults to writer priority
 * [starved] should be 's' for the starved versions, or anything else for the balanced timing
 */

//#include <omp.h>
#include <tsgl.h>
#include <unistd.h>
#include "Reader.h"
#include "Writer.h"
#include "RWDatabase.h"
#include "WDatabase.h"
#include "RDatabase.h"
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

	//Start the visualization
	can.start();
	can.setBackgroundColor(WHITE);

	//Create monitor
	RWDatabase<Rectangle*> * monitor;
	string lockString; //String description of lock style
	int maxItems = floor(RWThread::dataWidth / RWThread::width) * floor(RWThread::dataHeight / RWThread::width);
	if( argc > 3 && *argv[3] == 'r' ) { //Create Reader preference Database
		monitor = new RDatabase<Rectangle*>(maxItems);
		lockString = "Reader priority";
	} else {							//Create Writer preference Database
		monitor = new WDatabase<Rectangle*>(maxItems);
		lockString = "Writer priority";
	}

	Reader * readers = new Reader[numReaders]; //Array of Readers
	Writer * writers = new Writer[numWriters]; //Array of Writers

	//Draw labels
	Rectangle dataRec(RWThread::dataX, RWThread::dataY-RWThread::dataHeight, RWThread::dataWidth, RWThread::dataHeight, GRAY); // draw data area
	dataRec.setHasOutline(false); dataRec.setLayer(2); can.add(&dataRec);
	Rectangle margins(RWThread::dataX-MARGIN, RWThread::dataY-RWThread::dataHeight, RWThread::dataWidth+2*MARGIN, RWThread::dataHeight, LIGHTGRAY);
	margins.setLayer(1); can.add(&margins); can.setDefaultLayer(3);
	Line readerLine(RWThread::dataX+RWThread::dataWidth+MARGIN*2.5, RWThread::dataY-RWThread::dataHeight, RWThread::dataX+RWThread::dataWidth+MARGIN*2.5, RWThread::dataY, BLACK);
	Line writerLine(RWThread::dataX-MARGIN*2.5, RWThread::dataY-RWThread::dataHeight, RWThread::dataX-MARGIN*2.5, RWThread::dataY, BLACK);
	Text lockText(lockString, 50, WINDOW_HEIGHT-50, 24, BLACK);
	Text numText("Numbers indicate", WINDOW_WIDTH-225, WINDOW_HEIGHT-50, 20, BLACK);
	Text numText2("counts of reads/writes", WINDOW_WIDTH-225, WINDOW_HEIGHT-30, 20, BLACK);
	Text writeText("Writers", 20, 40, 24, BLACK);
	Text readText("Readers", WINDOW_WIDTH-150, 40, 24, BLACK);
	Text dataLabel("Shared Data Store", RWThread::dataX, RWThread::dataY+30, 20, BLACK);
	dataLabel.setCenter(WINDOW_WIDTH/2, RWThread::dataY+15);
	Text readThink("Thinking", RWThread::dataX+RWThread::dataWidth+MARGIN*3, RWThread::dataY-RWThread::dataHeight, 20, BLACK);
	Text writeThink("Thinking", RWThread::dataX-MARGIN*4, RWThread::dataY-RWThread::dataHeight, 20, BLACK);
	Text readWait("Waiting", RWThread::dataX+RWThread::dataWidth+MARGIN*1.5, RWThread::dataY-RWThread::dataHeight, 20, BLACK);
	Text writeWait("Waiting", RWThread::dataX-MARGIN*2, RWThread::dataY-RWThread::dataHeight, 20, BLACK);
	readThink.setRotation(-45, RWThread::dataX+RWThread::dataWidth+MARGIN*3, RWThread::dataY-RWThread::dataHeight);
	writeThink.setRotation(-45, RWThread::dataX-MARGIN*4, RWThread::dataY-RWThread::dataHeight);
	readWait.setRotation(-45, RWThread::dataX+RWThread::dataWidth+MARGIN*1.5, RWThread::dataY-RWThread::dataHeight);
	writeWait.setRotation(-45, RWThread::dataX-MARGIN*2, RWThread::dataY-RWThread::dataHeight);
	can.add( &readerLine ); can.add( &writerLine ); can.add( &readThink ); can.add( &writeThink ); can.add( &readWait ); can.add( &writeWait );
	can.add( &lockText ); can.add( &numText ); can.add( &numText2 ); can.add( &writeText ); can.add( &readText ); can.add( &dataLabel );

	//Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = Reader(*monitor, i, can); //Reader created
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i] = Writer(*monitor, i, can); //Writer created
	}

	//Check if using starved version
	//Starves Readers if there are at least 3 Writers
	//Nearly starves Writers with 3 or more Readers
	if( argc > 4 && *argv[4] == 's' ) { //Set for starved possibility
		if( *argv[3] == 'r' ) { //Readers mostly starve Writers
			RWThread::WAIT_MIN = 2;
			RWThread::WAIT_RANGE = 25;
			RWThread::access_wait *= 5.0;
		} else { //Writers starve Readers
			RWThread::WAIT_MIN = 2;
			RWThread::WAIT_RANGE = 10;
		}
	} //else change nothing

	srand(time(NULL)); //Seed random number generator for colors and wait times

	can.bindToButton(TSGL_SPACE, TSGL_PRESS, []() { // toggle pause when spacebar is pressed
		RWThread::paused = !RWThread::paused;
	});

	//Start the Reader and Writer pthreads
	for(int i = 0; i < numWriters; i++) {
		writers[i].start();
		sleep(0.1);
	}
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
