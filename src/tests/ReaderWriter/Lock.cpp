#include "Lock.h"
/**
 * Lock.cpp provides a Lock class for the Reader-Writer visualization.
 * This class provides a superclass for the monitors with Reader or Writer preference, which are used by the threads.
 */

/**
 * \brief Default constructor for the Lock class
 * \return: The constructed Lock object.
 */
Lock::Lock() {
	activeWriters = activeReaders = waitingWriters = waitingReaders = 0;
	lock = PTHREAD_MUTEX_INITIALIZER;
	okToRead = PTHREAD_COND_INITIALIZER;
	okToWrite = PTHREAD_COND_INITIALIZER;
}

/**
 * \brief Explicit constructor for the Lock class
 * \param data, the database being protected
 * \return: The constructed Lock object.
 */
Lock::Lock(RWDatabase<tsgl::Rectangle*>& data) {
	activeWriters = activeReaders = waitingWriters = waitingReaders = 0;
	lock = PTHREAD_MUTEX_INITIALIZER;
	okToRead = PTHREAD_COND_INITIALIZER;
	okToWrite = PTHREAD_COND_INITIALIZER;
	this->data = &data;
}
