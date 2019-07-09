#include "Thread.h"

/**
 * \brief Default-constructor for a Thread object.
 * \return: The constructed Thread object with id == 0.
 */
Thread::Thread() {
	myId = 0;
}

/**
 * \brief Explicit-constructor for a Thread object.
 * \param myId, an unsigned long representing the id for the Thread object.
 * \return: The constructed Thread object with id == myId.
 */
Thread::Thread(unsigned long id) {
	myId = id;
}

/**
 * \brief threadFunction() is the function that the pthread should run as soon as it is created.
 * \param obj, a void* that will be statically casted into a Thread object.
 *              It will run the run() function, which will be defined by the inheriting subclass.
 * \return: NULL.
 */
void* Thread::threadFunction(void* obj) {
	Thread * thread = static_cast<Thread*>(obj);
	thread->run();  //This NEEDS to be defined by the subclass
	pthread_exit(0); //Exit upon completion
}

/**
 * \brief start() function creates the pthread inside of the Thread class.
 */
void Thread::start() {
	pthread_create(&myThread, NULL, threadFunction, this);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
}

/**
 * \brief join() function cancels the pthread inside of the Thread class.
 */
void Thread::join() {
	pthread_cancel(myThread); // solution to hanging process bug
}

/**
 * \brief getId() is the accessor function for the id of the Thread object.
 * \return: myId, an unsigned long representing the id of the Thread object.
 */
unsigned long Thread::getId() const {
	return myId;
}
