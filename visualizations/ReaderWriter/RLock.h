/**
 * RLock.h provides the monitor for the Reader-Writer visualization that gives preference to the Readers.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 */

#ifndef RMONITOR_H_
#define RMONITOR_H_

#include <pthread.h>
#include <tsgl.h>
#include "Lock.h"
#include "RWDatabase.h"

/**
 * \class RLock
 * \brief A lock giving priority to Readers.
 * \details Inheritance: Lock class.
 * \details Implements the locking and unlocking methods of a monitor.
 */
class RLock : public Lock {
public:
	RLock() : Lock() {};			//Default  constructor
	RLock(RWDatabase<tsgl::Rectangle*>& data) : Lock(data) {};	//Explicit constructor
	void readLock();	//Inherited from Lock class
	void readUnlock();		//Inherited from Lock class
	void writeLock();//Inherited from Lock class
	void writeUnlock();	//Inherited from Lock class
};

#endif /*RMONITOR_H_*/
