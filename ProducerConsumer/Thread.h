/**
 * Thread.h contains the abstract class necessary in order to have pthreads for Producers and Consumers.
 * Adapted from: http://jrdodds.blogs.com/blog/2006/06/encapsulating_a.html
 *				 https://slworkthings.wordpress.com/2009/11/10/a-pthread-wrapper-class-part-1/
 *				 https://cppcodetips.wordpress.com/2013/12/05/pthread-c-wrapper/
 *				 http://peter.bourgon.org/blog/2010/10/27/who-needs-boost-a-simple-pthreads-wrapper.html 
 *				 Practical Guide To Pthread Programming in C++ By Swaminathan Bhaskar (pdf)
 */
#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <iostream>

/**
 * The Thread class encapsulates the details of creating a pthread.
 * It is abstract; cannot be instantiated. 
 */
class Thread {
public:
	Thread();  //Default-constructor
	Thread(unsigned long id);  //Constructor
	unsigned long getId() const;  //Accessor for the id
	void start();  //Create the pthread and run the corresponding method
	virtual void run() = 0;  //Needs to be defined by the inheriting subclass
	void join();   //Join at the end
	
protected:
	static void* threadFunction(void* obj); //The function that the pthread will run

private:
	unsigned long myId;   //Id for the Thread object
	pthread_t myThread;   //pthread
};

#endif /* THREAD_H_ */
