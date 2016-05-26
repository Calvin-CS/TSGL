
#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <iostream>

/**
 * Adapted from: https://cppcodetips.wordpress.com/2013/12/05/pthread-c-wrapper/
 *               https://slworkthings.wordpress.com/2009/11/10/a-pthread-wrapper-class-part-1/
 *               Practical Guide To Pthread Programming in C++ By Swaminathan Bhaskar (pdf).
 */
class Thread {
public:
	Thread();
	Thread(unsigned long id);  //Constructor
	unsigned long getId() const;
	void start();  //Create the pthread and run the corresponding method
	void run();  //Needs to be defined by the Producer or the Consumer
	void join();   //Join at the end
	
protected:
	static void* threadFunction(void* obj);
private:
	unsigned long myId;
	pthread_t myThread;
	
};

#endif /* THREAD_H_ */
