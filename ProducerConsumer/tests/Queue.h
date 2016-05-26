/**
 *
 *
 *
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <pthread.h>
#include <tsgl.h>
using namespace tsgl;

template<class Item> 

class Queue {
public:
	Queue(); //Default constructor
	Queue(int size);  //Explicit Constructor
	void append(Item it);	//Push an Item onto the Queue
	Item remove();    //Get an Item off of the Queue
	//Accessors	
	Item* getArray() const;
	int getCount() const;
	Item getFirst() const;
	Item getLast() const;
	int getCapacity() const;
	//Utility methods	
	bool isEmpty() const;
	bool isFull() const;
	~Queue(); //Destructor	
	

private:
	Item * myArray;  //Array of data
	int mySize, myCount, myFirst, myLast;	 //Size, number of elements, and first and last indice
	friend class ArrayQueueTester;
	pthread_mutex_t myMutex;
	//Broadcast a signal indicating that the Queue is not full (append, Producers) and that the Queue is not empty (remove, Consumers)
	pthread_cond_t notEmpty;  //Condition variable for remove
	pthread_cond_t notFull;   //Condition variable for append
};

/**
 * Default constructor for the Queue class.
 * Precondition:
 * Postcondition:
 */
template<class Item>
Queue<Item>::Queue() {
	myArray = NULL;
	mySize = myCount = myFirst = myLast = 0;
	pthread_cond_init( &notEmpty, NULL );
	pthread_cond_init( &notFull, NULL );
	pthread_mutex_init( &myMutex, NULL );
}

/**
 * Explicit-constructor for the Queue class.
 * @param: size, the size of the Queue.
 * Precondition:
 * Postcondition:
 */
template<class Item>
Queue<Item>::Queue(int size) {
	myArray = new Item[size];
	mySize = size;  //Max number of elements to be in the Queue
	myCount = 0;  //Number of elements in the Queue
	myFirst = myLast = 0;  
	pthread_cond_init( &notEmpty, NULL );
	pthread_cond_init( &notFull, NULL );
	pthread_mutex_init( &myMutex, NULL );
}

/**
 * append() puts an item into the Queue.
 * @param: item, a reference to an Item to put into the Queue. 
 * Precondition:
 * Postcondition:
 */
template<class Item>
void Queue<Item>::append(Item it) {
	pthread_mutex_lock( &myMutex );  //Lock the mutex so only one thread can append
	while(myCount == mySize) {
		pthread_cond_wait(&notFull, &myMutex); //The Queue is full, please wait until it is not full.
	}	
	myArray[myLast] = it;
	myCount++;
	myLast = (myLast + 1) % mySize;  //Increment the indexer to my last item, but keep myFirst to 0 (always will be for an Array).
	pthread_cond_signal(&notEmpty);  //Signal that the Queue is not empty.
	pthread_mutex_unlock( &myMutex );		//Unlock for other threads
}

/**
 * remove() takes an Item out of the Queue.
 * Precondition:
 * Postcondition:
 */
template<class Item>
Item Queue<Item>::remove() {
	pthread_mutex_lock( &myMutex );  //Lock the mutex
	Item temp;	
	while(myCount == 0) {
		pthread_cond_wait(&notEmpty, &myMutex);  //The Queue is empty, please wait until it is not empty.
	} 
	temp = myArray[myFirst];
	myFirst = (myFirst + 1) % mySize;
	myCount--;    
	pthread_cond_signal(&notFull);  //Signal that the Queue is not full.
	pthread_mutex_unlock( &myMutex );  //Unlock before returning
	return temp;
}

/**
 *
 *
 */
template<class Item> 
Queue<Item>::~Queue() {
	pthread_cond_destroy(&notEmpty);
	pthread_cond_destroy(&notFull);
	pthread_mutex_destroy(&myMutex);
	delete [] myArray;
	myArray = NULL;
}

/**
 * getArray() is the accessor method for the array of the Queue.
 */
template<class Item>
Item* Queue<Item>::getArray() const {
	return myArray;
}

/**
 * getCount() is the accessor method for the number of elements currently in the Queue.
 */
template<class Item>
int Queue<Item>::getCount() const {
	return myCount;
}

/**
 * getFirst() is the accessor method for the first element in the Queue.
 */
template<class Item>
Item Queue<Item>::getFirst() const {
	return myArray[myFirst];
}

/**
 *	getLast() is the accessor method for the last element in the Queue.
 */
template<class Item>
Item Queue<Item>::getLast() const {
	return myArray[(myLast-1+mySize) % mySize];
}

/**
 *
 */
template<class Item>
int Queue<Item>::getCapacity() const {
	return mySize;
}

template<class Item>
bool Queue<Item>::isEmpty() const {
	return myCount == 0;
}

template<class Item>
bool Queue<Item>::isFull() const {
	return myCount == mySize;
}

#endif /*QUEUE_H_*/
