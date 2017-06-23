/**
 * Writer.h contains the class necessary in order to create a Writer object for the Reader-Writer visualization.
 */
#ifndef WRITER_H_
#define WRITER_H_

//#include <omp.h>
#include <tsgl.h>
#include "RWMonitor.h"
#include "WMonitor.h"
#include "RWThread.h"
#include "Thread.h"
using namespace tsgl;

/**
 * \class Writer
 * \brief A Thread that writes items in the visualization.
 * \details Writer class creates a Writer object and inherits from the RWThread class.
 * \details Inheritance: RWThread class.
 * \details Implements the abstract run() method from the Thread class so that the pthread runs the write() method.
 */
class Writer : public RWThread {
public:
	Writer(); //Default constructor
	Writer(RWMonitor<Rectangle*> & sharedMonitor, unsigned long id, Canvas & can); //Explicit constructor
	void write(); //Generate color data
	void run();   //Implementation of run() method for pthread
private:
	ColorInt randColor();
	int randIndex();
	void drawArrow(int x, int y);
	Rectangle * makeRec(int index);
};

#endif /* WRITER_H_ */
