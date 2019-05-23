#include "FLock.h"
/**
 * FLock.cpp provides the monitor for the Reader-Writer visualization that gives a fair preference.
 * This class is a subclass of Lock and implements its locking and unlocking virtual methods.
 */

FLock::FLock() : Lock() {
	in = PTHREAD_MUTEX_INITIALIZER;
	wrt = PTHREAD_MUTEX_INITIALIZER;
}

FLock::FLock(RWDatabase<tsgl::Rectangle*>& data) : Lock(data) {
	in = PTHREAD_MUTEX_INITIALIZER;
	wrt = PTHREAD_MUTEX_INITIALIZER;
}

/**
 * \brief readLock() implements the abstract method in Lock
 * \details Grants the calling thread access for reading
 */
void FLock::readLock() {
	pthread_mutex_lock( &this->in );
	pthread_mutex_lock( &this->lock );
	if( (++activeReaders) == 1) pthread_mutex_lock( &this->wrt );
	pthread_mutex_unlock( &this->in );
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief readUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's read lock
 */
void FLock::readUnlock() {
	pthread_mutex_lock( &this->lock );
	if ( (--activeReaders) == 0 ) pthread_mutex_unlock( &this->wrt );
	pthread_mutex_unlock( &this->lock );
}

/**
 * \brief writeLock() implements the abstract method in Lock
 * \details Grants the calling thread acces for writing
 */
void FLock::writeLock() {
	pthread_mutex_lock( &this->in );
	pthread_mutex_lock( &this->wrt );
}

/**
 * \brief writeUnlock() implements the abstract method in Lock
 * \details Releases the calling thread's write lock
 */
void FLock::writeUnlock() {
	pthread_mutex_unlock( &this->wrt );
	pthread_mutex_unlock( &this->in );
}
