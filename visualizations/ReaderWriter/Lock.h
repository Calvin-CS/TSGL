/**
 * Lock.h provides a Lock class for the Reader-Writer visualization.
 * This class provides a superclass for the monitors with Reader or Writer preference, which are used by the threads.
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <pthread.h>
#include <vector>
#include <iostream>
#include <queue>
#include <tsgl.h>
#include "RWDatabase.h"
using namespace std;

/**
 * \class Lock
 * \brief An abstract database protecting a vector of data
 * \details Database has its locks
 * \details Locking methods must be implemented in subclass, giving priority to different types of threads.
 */
class Lock {
public:
	Lock();																//Default constructor
	Lock(RWDatabase<tsgl::Rectangle*>& data);		//Explicit constructor
	virtual void readLock()    = 0;					//Must be defined by subclass
	virtual void readUnlock()  = 0;				//Must be defined by subclass
	virtual void writeLock()   = 0;					//Must be defined by subclass
	virtual void writeUnlock() = 0;				//Must be defined by subclass

protected:
	int activeWriters, activeReaders, waitingWriters, waitingReaders;
	pthread_mutex_t lock;
	pthread_cond_t okToRead, okToWrite;
	RWDatabase<tsgl::Rectangle*>* data;
};

#endif /*MONITOR_H_*/
