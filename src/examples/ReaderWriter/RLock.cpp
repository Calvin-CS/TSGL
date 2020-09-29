#include "RLock.h"
/**
 * RLock.cpp provides the monitor for the Reader-Writer visualization that gives preference to the Readers.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 */

/**
 * \brief readLock() implements the abstract method in Lock
 * \details Grants the calling thread access for reading, giving priority to readers
 */
void RLock::readLock() {
	pthread_mutex_lock( &this->lock );
	++this->waitingReaders;
	while( data->getItemCount() == 0 ) { //Wait for okToRead while vector is empty
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
 * \brief readUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's read lock
 */
void RLock::readUnlock() {
	pthread_mutex_lock( &this->lock );
	--this->activeReaders;
	if( this->activeReaders == 0 && this->waitingWriters > 0 ) { //Last out alerts Writers
		pthread_cond_signal( &this->okToWrite );
	}
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief writeLock() implements the abstract method in Lock
 * \details Grants the calling thread acces for writing, giving priority to Readers
 */
void RLock::writeLock() {
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
 * \brief writeUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's write lock
 */
void RLock::writeUnlock() {
	pthread_mutex_lock( &this->lock );
	--this->activeWriters;
	if( this->waitingReaders > 0 ) { //Alert all Readers data is available
		pthread_cond_broadcast( &this->okToRead );
	} else if( this->waitingWriters > 0 ) { //Alert other Writers only if no Readers
		pthread_cond_signal( &this->okToWrite );
	}
	pthread_mutex_unlock( &this->lock );
}
