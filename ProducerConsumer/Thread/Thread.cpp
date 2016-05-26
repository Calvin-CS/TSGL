#include "Thread.h"

Thread::Thread() {
	myId = 0;
}

Thread::Thread(unsigned long id) {
	myId = id;
}

void* Thread::threadFunction(void* obj) {
	Thread * thread = static_cast<Thread*>(obj);
	thread->run();  //This NEEDS to be defined by the subclass
	return NULL;
}

void Thread::start() {
	pthread_create(&myThread, NULL, threadFunction, this);
}

void Thread::join() {
	pthread_join(myThread, NULL);
}

unsigned long Thread::getId() const {
	return myId;
}

void Thread::run() {
	std::cout << "Hi from Thread " << getId() << "!\n";
}
