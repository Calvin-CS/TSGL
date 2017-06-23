/**
 * Reader.h contains the class necessary in order to create a Reader object for the Reader-Writer visualization.
 */
#ifndef READER_H_
#define READER_H_

#include <omp.h>
#include <tsgl.h>
#include "RWMonitor.h"
#include "WMonitor.h"
#include "RWThread.h"
#include "Thread.h"
using namespace tsgl;

/**
 * \class Reader
 * \brief Reader class inherits from the RWThread class in order to create a Reader object.
 * \details Inheritance: RWThread class.
 * \details Implements the run() method, which calls the read() method.
 */
class Reader : public RWThread {
public:
	Reader(); //Default constructor
	Reader(RWMonitor<Rectangle*> & sharedMonitor, unsigned long id, Canvas & can); //Explicit constructor
	void read(); //Read method
	void run();  //Inheirted from RWThread class; function that the pthread should run.
private:
	void drawArrow(int x, int y);
};

#endif /* READER_H_ */
