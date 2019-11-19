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
#include "ReaderWriter/Reader.h"
#include "ReaderWriter/Writer.h"
#include "ReaderWriter/RWDatabase.h"
#include "ReaderWriter/Lock.h"
#include "ReaderWriter/RLock.h"
#include "ReaderWriter/WLock.h"
#include "ReaderWriter/FLock.h"
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
	wstring lockString; //String description of lock style
	int maxItems = floor(RWThread::dataWidth / RWThread::width) * floor(RWThread::dataHeight / RWThread::width);
	RWDatabase<Rectangle*>* database = new RWDatabase<Rectangle*>(maxItems);
	if( argc > 3 && *argv[3] == 'r' ) {
		//Reader preference
		lock = new RLock(database);
		lockString = L"Reader priority";
	} else if (argc > 3 && *argv[3] == 'w' ) {
		//Writer preference
		lock = new WLock(database);
		lockString = L"Writer priority";
	} else {
		lock = new FLock(database);
		lockString = L"Fair priority";
	}

	Reader ** readers = new Reader*[numReaders]; //Array of Readers
	Writer ** writers = new Writer*[numWriters]; //Array of Writers

	//Create labels
	can.drawRectangle(RWThread::dataX-MARGIN, RWThread::dataY-RWThread::dataHeight, RWThread::dataWidth+2*MARGIN, RWThread::dataHeight, GRAY);
	can.drawRectangle(RWThread::dataX, RWThread::dataY-RWThread::dataHeight, RWThread::dataWidth, RWThread::dataHeight, DARKGRAY); // draw data area
	can.drawLine(RWThread::dataX+RWThread::dataWidth+MARGIN*2.5, RWThread::dataY-RWThread::dataHeight, RWThread::dataX+RWThread::dataWidth+MARGIN*2.5, RWThread::dataY, BLACK);
	can.drawLine(RWThread::dataX-MARGIN*2.5, RWThread::dataY-RWThread::dataHeight, RWThread::dataX-MARGIN*2.5, RWThread::dataY, BLACK);
	can.drawText(lockString, 100, RWThread::dataY + 40, 20, BLACK);
	can.drawText(L"Numbers indicate", WINDOW_WIDTH-200, RWThread::dataY + 40, 20, BLACK);
	can.drawText(L"counts of reads/writes", WINDOW_WIDTH-200, RWThread::dataY + 60, 20, BLACK);
	can.drawText(L"Writers", 72, 60, 24, BLACK);
	can.drawText(L"Readers", WINDOW_WIDTH-140, 60, 24, BLACK);
	can.drawText(L"Shared Data Store", 226, 692, 20, BLACK);

	//Create and rotate more labels
	can.drawText(L"Thinking", 484, 127, 28, GRAY, "", PI/2);
	can.drawText(L"Waiting", 423, 127, 28, GRAY, "", PI/2);
	can.drawText(L"Thinking", 14, 127, 28, GRAY, "", -PI/2);
	can.drawText(L"Waiting", 88, 127, 28, GRAY, "", -PI/2);

	//Fill the Reader and Writer arrays with their objects
	for(int i = 0; i < numReaders; i++) {
		readers[i] = new Reader(*database, *lock, i, can); //Reader created
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i] = new Writer(*database, *lock, i, can); //Writer created
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
		writers[i]->start();
		sleep(0.1);
	}
	for(int i = 0; i < numReaders; i++) {
		readers[i]->start();
		sleep(0.1);
	}

	can.wait(); //Wait for the main Canvas to be closed

	//End threads
	for(int i = 0; i < numReaders; i++) {
		readers[i]->join();
	}
	for(int i = 0; i < numWriters; i++) {
		writers[i]->join();
	}

	//Cleanup
	delete [] readers;
	delete [] writers;
	readers = NULL;
	writers = NULL;

	return 0;
}
