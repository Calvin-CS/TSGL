/**
 * testReaderWriter.cpp contains the code to run the Readers-Writers Problem animation using TSGL and POSIX threads.
 * The program utilizes Reader and Writer classes as well as a custom monitor class to hold the shared data.
 * Usage: ./ReaderWriter <numberOfReaders> <numberOfWriters> <preference> <starved>
 * <preference> can be 'w' for writer priority or 'r' for reader priority and defaults to writer priority
 * <starved> should be 's' for the starved versions, or anything else for the balanced timing
 */

//#include <omp.h>
#include <tsgl.h>
#include <unistd.h>
#include "Reader.h"
#include "Writer.h"
#include "RWDatabase.h"
#include "Lock.h"
#include "RLock.h"
#include "WLock.h"
#include "FLock.h"
using namespace tsgl;

//Constants
const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 800, MARGIN = 45; //Size of Canvas and margin around data

int main(int argc, char* argv[]) {

	if( argc == 1) {
		std::cout << "\nTo run the program with different values, use the format:\n\t./ReaderWriter <numberOfReaders> <numberOfWriters> <preference> <starved>"
		<< "\nwhere <preference> is w or r for reader or writer priority lock and \n\t<starved> is s for starving lower priority threads or b for the more balanced version." << std::endl;
	}

	//Number of readers is the first argument or defaults to 6
	int numReaders  = ( (argc > 1) && (atoi(argv[1]) > 0) && (atoi(argv[1]) <= 9) ) ? atoi(argv[1]) : 6;
	//Number of writers is the second argument or defaults to 6
	int numWriters = ( (argc > 2) && (atoi(argv[2]) > 0) && (atoi(argv[2]) <= 9) ) ? atoi(argv[2]) : 6;

	//Start Reader-Writer visualization
	Canvas can(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "Reader-Writer", 1.0f/2);  //Canvas to draw on

	//Start the visualization
	can.start();
	can.setBackgroundColor(WHITE);

	//Create monitor
	Lock * lock;
	string lockString; //String description of lock style
	int maxItems = floor(RWThread::dataWidth / RWThread::width) * floor(RWThread::dataHeight / RWThread::width);
	RWDatabase<Rectangle*>* database = new RWDatabase<Rectangle*>(maxItems);
	if( argc > 3 && *argv[3] == 'r' ) {
		//Reader preference
		lock = new RLock(database);
		lockString = "Reader priority";
	} else if (argc > 3 && *argv[3] == 'w' ) {
		//Writer preference
		lock = new WLock(database);
		lockString = "Writer priority";
	} else {
		lock = new FLock(database);
		lockString = "Fair priority";
	}

	Reader * readers = new Reader[numReaders]; //Array of Readers
	Writer * writers = new Writer[numWriters]; //Array of Writers

	//Create labels
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

	//Create and rotate more labels
	Text readThink("Thinking", RWThread::dataX+RWThread::dataWidth+MARGIN*3, RWThread::dataY-RWThread::dataHeight, 28, LIGHTGRAY);
	Text readWait("Waiting", RWThread::dataX+RWThread::dataWidth+MARGIN*1.5, RWThread::dataY-RWThread::dataHeight, 28, LIGHTGRAY);
	readThink.setRotation(90, readThink.getX(), readThink.getY());
	readWait.setRotation(90, readWait.getX(), readWait.getY());
	Text writeThink("Thinking", RWThread::dataX-MARGIN*3, RWThread::dataY-RWThread::dataHeight, 28, LIGHTGRAY);
	Text writeWait("Waiting", RWThread::dataX-MARGIN*1.5, RWThread::dataY-RWThread::dataHeight, 28, LIGHTGRAY);
	writeThink.setLocation(writeThink.getX(), writeThink.getY()+writeThink.getStringWidth());
	writeWait.setLocation(writeWait.getX(), writeWait.getY()+writeWait.getStringWidth());
	writeThink.setRotation(-90, writeThink.getX(), writeThink.getY());
	writeWait.setRotation(-90, writeWait.getX(), writeWait.getY());

	//Add labels
	can.add( &readerLine ); can.add( &writerLine ); can.add( &readThink ); can.add( &writeThink ); can.add( &readWait ); can.add( &writeWait );
	can.add( &lockText ); can.add( &numText ); can.add( &numText2 ); can.add( &writeText ); can.add( &readText ); can.add( &dataLabel );

	//Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = Reader(*database, *lock, i, can); //Reader created
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i] = Writer(*database, *lock, i, can); //Writer created
	}

	//Check if using starved version
	//Starves Readers if there are at least 3 Writers
	//Nearly starves Writers with 3 or more Readers
	if( argc > 4 && *argv[4] == 's' ) { //Set for starved possibility
		if( *argv[3] == 'r' ) { //Readers mostly starve Writers
			RWThread::WAIT_MIN = 0;
			RWThread::WAIT_RANGE = 20;
			RWThread::access_wait *= 20.0;
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
