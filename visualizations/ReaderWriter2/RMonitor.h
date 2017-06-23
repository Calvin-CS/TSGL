/**
 * RMonitor.h provides the monitor for the Reader-Writer visualization that gives preference to the Readers.
 * This class is a subclass of RWMonitor and implements its locking and unlocking virtual methods.
 */

#ifndef RMONITOR_H_
#define RMONITOR_H_

#include <pthread.h>
#include "RWMonitor.h"

template<class Item>

/**
 * \class RMonitor
 * \brief A monitor giving priority to Readers.
 * \details Inheritance: RWMonitor class.
 * \details Implements the locking and unlocking methods of a monitor.
 */
class RMonitor : public RWMonitor<Item> {
public:
	RMonitor() : RWMonitor<Item>() {};			//Default  constructor
	RMonitor(int max) : RWMonitor<Item>(max) {};	//Explicit constructor
	void startRead();	//Inherited from RWMonitor class
	void endRead();		//Inherited from RWMonitor class
	void startWrite();//Inherited from RWMonitor class
	void endWrite();	//Inherited from RWMonitor class
};

/**
 * \brief startRead() implements the abstract method in RWMonitor
 * \details Grants the calling thread access for reading, giving priority to readers
 */
template<class Item>
void RMonitor<Item>::startRead() {
	pthread_mutex_lock( &this->lock );
	++this->waitingReaders;
	while( this->getItemCount() == 0 ) { //Wait for okToRead while vector is empty
		pthread_cond_wait( &this->okToRead, &this->lock );
	}
	while( this->activeWriters > 0 ) { //Yield only to active Writers
		pthread_cond_wait( &this->okToRead, &this->lock );
	}
	--this->waitingReaders;
	++this->activeReaders;
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief endRead() implements the abstract method in RWMonitor
 * \details Releases the calling thread's read lock
 */
template<class Item>
void RMonitor<Item>::endRead() {
	pthread_mutex_lock( &this->lock );
	--this->activeReaders;
	if( this->activeReaders == 0 && this->waitingWriters > 0 ) { //Last out alerts Writers
		pthread_cond_signal( &this->okToWrite );
	}
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief startWrite() implements the abstract method in RWMonitor
 * \details Grants the calling thread acces for writing, giving priority to Readers
 */
template<class Item>
void RMonitor<Item>::startWrite() {
	pthread_mutex_lock( &this->lock );
	while( this->activeWriters > 0 || this->activeReaders > 0 || this->waitingReaders > 0) {
		//Yield to active Writers and all Readers
		++this->waitingWriters;
		pthread_cond_wait( &this->okToWrite, &this->lock );
		--this->waitingWriters;
	}
	++this->activeWriters;
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief endWrite() implements the abstract method in RWMonitor
 * \details Releases the calling thread's write lock
 */
template<class Item>
void RMonitor<Item>::endWrite() {
	pthread_mutex_lock( &this->lock );
	--this->activeWriters;
	if( this->waitingReaders > 0 ) { //Alert all Readers data is available
		pthread_cond_broadcast( &this->okToRead );
	} else if( this->waitingWriters > 0 ) { //Alert other Writers only if no Readers
		pthread_cond_signal( &this->okToWrite );
	}
	pthread_mutex_unlock( &this->lock );
}

#endif /*RMONITOR_H_*/
