/**
 * WLock.h provides the monitor for the Reader-Writer visualization that gives preference to the Writers.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 */

#ifndef WLOCK_H_
#define WLOCK_H_

#include <pthread.h>
#include "Lock.h"

/**
 * \class WLock
 * \brief A lock giving priority to Writers in the visualization.
 * \details Inheritance: Lock class.
 * \details Implements the locking and unlocking methods of a monitor.
 */
class WLock : public Lock {
public:
	WLock() : Lock() {};						//Default  constructor
	WLock(RWDatabase<tsgl::Rectangle*>* data) : Lock(data) {};	//Explicit constructor
	void readLock();											//Inherited from Lock class
	void readUnlock();										//Inherited from Lock class
	void writeLock();											//Inherited from Lock class
	void writeUnlock();										//Inherited from Lock class
};

#endif /*WLOCK_H_*/
