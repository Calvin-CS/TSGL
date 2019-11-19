#include "WLock.h"
/**
 * WLock.cpp provides the monitor for the Reader-Writer visualization that gives preference to the Writers.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 */

/**
 * \brief readLock() implements the abstract method in Lock
 * \details Grants the calling thread access for reading, giving priority to writers
 */
void WLock::readLock() {
	pthread_mutex_lock( &this->lock );
	++this->waitingReaders;
	while( data->getItemCount() == 0 ) { //Wait for okToRead while vector is empty
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
 * \brief readUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's read lock
 */
void WLock::readUnlock() {
	pthread_mutex_lock( &this->lock );
	--this->activeReaders;
	if( this->activeReaders == 0 && this->waitingWriters > 0 ) {
		pthread_cond_signal( &this->okToWrite ); //Alert Writer data is available
	}
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief writeLock() implements the abstract method in Lock
 * \details Grants the calling thread acces for writing, giving priority to writers
 */
void WLock::writeLock() {
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
 * \brief writeUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's write lock
 */
void WLock::writeUnlock() {
	pthread_mutex_lock( &this->lock );
	--this->activeWriters;
	if( this->waitingWriters > 0 ) { //Alert waiting Writer
		pthread_cond_signal( &this->okToWrite );
	} else if( this->waitingReaders > 0 ) { //Alert waiting Readers only if no waiting Writers
		pthread_cond_broadcast( &this->okToRead );
	}
	pthread_mutex_unlock( &this->lock );
}
