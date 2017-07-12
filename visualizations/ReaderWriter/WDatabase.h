/**
 * WDatabase.h provides the monitor for the Reader-Writer visualization that gives preference to the Writers.
 * This class is a subclass of RWDatabase and implements its locking and unlocking virtual methods.
 */

#ifndef WMONITOR_H_
#define WMONITOR_H_

#include <pthread.h>
#include "RWDatabase.h"

template<class Item>

/**
 * \class WDatabase
 * \brief A database with lock giving priority to Writers in the visualization.
 * \details Inheritance: RWDatabase class.
 * \details Implements the locking and unlocking methods of a monitor.
 */
class WDatabase : public RWDatabase<Item> {
public:
	WDatabase() : RWDatabase<Item>() {};			//Default  constructor
	WDatabase(int max) : RWDatabase<Item>(max) {};	//Explicit constructor
	void startRead();	//Inherited from RWDatabase class
	void endRead();		//Inherited from RWDatabase class
	void startWrite();//Inherited from RWDatabase class
	void endWrite();	//Inherited from RWDatabase class
};

/**
 * \brief startRead() implements the abstract method in RWDatabase
 * \details Grants the calling thread access for reading, giving priority to writers
 */
template<class Item>
void WDatabase<Item>::startRead() {
	pthread_mutex_lock( &this->lock );
	++this->waitingReaders;
	while( this->getItemCount() == 0 ) { //Wait for okToRead while vector is empty
		pthread_cond_wait( &this->okToRead, &this->lock );
	}
	while( this->activeWriters > 0 || this->waitingWriters > 0) { //Yield to all Writers
		pthread_cond_wait( &this->okToRead, &this->lock );
	}
	--this->waitingReaders;
	++this->activeReaders;
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief endRead() implements the abstract method in RWDatabase
 * \details Releases the calling thread's read lock
 */
template<class Item>
void WDatabase<Item>::endRead() {
	pthread_mutex_lock( &this->lock );
	--this->activeReaders;
	if( this->activeReaders == 0 && this->waitingWriters > 0 ) {
		pthread_cond_signal( &this->okToWrite ); //Alert Writer data is available
	}
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief startWrite() implements the abstract method in RWDatabase
 * \details Grants the calling thread acces for writing, giving priority to writers
 */
template<class Item>
void WDatabase<Item>::startWrite() {
	pthread_mutex_lock( &this->lock );
	while( this->activeWriters > 0 || this->activeReaders > 0) { //Wait for anyone already in data
		++this->waitingWriters;
		pthread_cond_wait( &this->okToWrite, &this->lock );
		--this->waitingWriters;
	}
	++this->activeWriters;
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief endWrite() implements the abstract method in RWDatabase
 * \details Releases the calling thread's write lock
 */
template<class Item>
void WDatabase<Item>::endWrite() {
	pthread_mutex_lock( &this->lock );
	--this->activeWriters;
	if( this->waitingWriters > 0 ) { //Alert waiting Writer
		pthread_cond_signal( &this->okToWrite );
	} else if( this->waitingReaders > 0 ) { //Alert waiting Readers only if no waiting Writers
		pthread_cond_broadcast( &this->okToRead );
	}
	pthread_mutex_unlock( &this->lock );
}

#endif /*WMONITOR_H_*/
