/**
 * FLock.h provides the fair monitor for the Reader-Writer visualization that gives equal preference to the Readers and Writers.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 * implements the "commonly known solution" presented in: https://arxiv.org/pdf/1309.4507.pdf
 */

#ifndef RWMONITOR_H_
#define RWMONITOR_H_

#include <pthread.h>
#include <tsgl.h>
#include "Lock.h"
#include "RWDatabase.h"

/**
 * \class FLock
 * \brief A lock giving equal priority to Readers and Writers.
 * \details Inheritance: Lock class.
 * \details Implements the locking and unlocking methods of a monitor.
 */
class FLock : public Lock {
public:
	FLock();			//Default  constructor
	FLock(RWDatabase<tsgl::Rectangle*>& data);	//Explicit constructor
	void readLock();	//Inherited from Lock class
	void readUnlock();		//Inherited from Lock class
	void writeLock();//Inherited from Lock class
	void writeUnlock();	//Inherited from Lock class
private:

	// TODO: name these better
	pthread_mutex_t in;
	pthread_mutex_t wrt;

};

#endif /*RWMONITOR_H_*/
