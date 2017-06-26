/**
 * Queue.h contains the class template code in order to create a custom Queue object.
 * Used in the Producer-Consumer visualization in order to visualize a shared buffer amongst pthreads.
 * A mutex and condition variables are used to synchronize accesses to the Queue and to avoid race conditions.
 * (Condition variable logic was adapted from the TS_Queue code given to me by Professor Joel Adams).
 * (Self-synchronization logic was also adapted from TS_Queue).
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <tsgl.h>
using namespace tsgl;

template<class Item>

/**
 * Queue class contains the data necessary in order to create a custom Queue object.
 * Used as a shared buffer in the Producer-Consumer visualization.
 */
class Queue {
public:
	Queue(); //Default constructor
	Queue(int size, Canvas & can);  //Explicit Constructor
	void append(Item it, int proId);	//Push an Item onto the Queue
	Item remove();    //Get an Item off of the Queue
	//Accessors
	Item* getArray();
	int getCount();
	Item getFirst();
	Item getLast();
	int getCapacity();
	int* getPthreadIds();  //Stored Pthread ids
	int getLastIndex() { return myLast; }
	int getFirstIndex() { return myFirst; }
	//Utility methods
	bool isEmpty() const;
	bool isFull() const;
	void producerLock();
	void producerUnlock();
	void consumerLock();
	void consumerUnlock();
	~Queue(); //Destructor

private:
	Item * myArray;  //Array of data
	int mySize, myCount, myFirst, myLast;	 //Size, number of elements, and first and last indice
	friend class ArrayQueueTester;    //Tester class
	pthread_mutex_t myMutex;
	//Broadcast a signal indicating that the Queue is not full (append, Producers) and that the Queue is not empty (remove, Consumers)
	pthread_cond_t notEmpty;  //Condition variable for remove
	pthread_cond_t notFull;   //Condition variable for append
	pthread_cond_t notOpen;   //Condition variable for determining if the Canvas is open
	//Reference to the Canvas
	Canvas * myCan;
	int * myPthreadIds; //Producer pthread ids (to color the rectangles around the Queue in the visualization)
};

/**
 * Default constructor for the Queue class.
 */
template<class Item>
Queue<Item>::Queue() {
	myArray = NULL;
	myPthreadIds = NULL;
	mySize = myCount = myFirst = myLast = 0;
	pthread_cond_init( &notEmpty, NULL );  //Initialize the condition variables and the mutex
	pthread_cond_init( &notFull, NULL );
	pthread_cond_init( &notOpen, NULL );
	pthread_mutex_init( &myMutex, NULL );
	myCan = NULL;
}

/**
 * Explicit-constructor for the Queue class.
 * @param: size, the size of the Queue.
 */
template<class Item>
Queue<Item>::Queue(int size, Canvas & can) {
	myArray = new Item[size];
	myPthreadIds = new int[size];
	mySize = size;  //Max number of elements to be in the Queue
	myCount = 0;  //Number of elements currently in the Queue
	myFirst = myLast = 0;
	pthread_cond_init( &notEmpty, NULL );   //Initialize the condition variables and the mutex
	pthread_cond_init( &notFull, NULL );
	pthread_cond_init( &notOpen, NULL );
	pthread_mutex_init( &myMutex, NULL );
	myCan = &can;
}

/**
 * append() puts an item into the Queue.
 * @param: it, an Item to put into the Queue.
 * @param: proId, an int representing the pthread id.
 * (Pthread condition variable logic adapted from TS_Queue code given to me by Professor Joel Adams).
 */
template<class Item>
void Queue<Item>::append(Item it, int proId) {
	myArray[myLast] = it;
	myLast = (myLast + 1) % mySize;  //Increment the indexer to my last item
	myCount++;
	myPthreadIds[myLast] = proId;  //Store the most recent pthread id
	pthread_cond_signal(&notEmpty);  //Signal that the Queue is not empty.
	myPthreadIds[myLast] = proId;  //Store the most recent pthread id
}

/**
 * producerLock() locks the Queue for the thread to append an item
 */
template<class Item>
void Queue<Item>::producerLock() {
	pthread_mutex_lock( &myMutex );  //Lock the mutex so only one thread can append
	while(myCount == mySize) {
		pthread_cond_wait(&notFull, &myMutex); //The Queue is full, please wait until it is not full.
	}
	if(!myCan->isOpen()) {  //If the Canvas is not open, wake up all of the sleeping threads
		pthread_cond_broadcast( &notOpen );
		pthread_mutex_unlock( &myMutex );
	}	//Else...
}

/**
 * producerUnlock() unlocks the Queue after the thread appends an item
 */
template<class Item>
void Queue<Item>::producerUnlock() {
	pthread_cond_signal(&notEmpty);  //Signal that the Queue is not empty.
	pthread_mutex_unlock( &myMutex );		//Unlock for other threads
}

/**
 * remove() takes an Item out of the Queue.
 * @return: temp, an Item that was taken out of the Queue.
 * (Pthread condition variable logic adapted from TS_Queue code given to me by Professor Joel Adams).
 */
template<class Item>
Item Queue<Item>::remove() {
	//ColorInt white(255, 255, 255);
	Item temp = myArray[myFirst]; // Item to be removed from Queue
	//myArray[myFirst] = white;
	myFirst = (myFirst + 1) % mySize;
	myCount--;
	return temp;
}

/**
 * consumerLock() locks the Queue for the thread to remove an item
 */
template<class Item>
void Queue<Item>::consumerLock() {
	pthread_mutex_lock( &myMutex );  //Lock the mutex
	if(!myCan->isOpen()) { //If the Canvas is not open, wake up all of the sleeping threads
		pthread_cond_broadcast( &notOpen );
		pthread_mutex_unlock( &myMutex );
	} else {	//Else...
		while(myCount == 0) {
			pthread_cond_wait(&notEmpty, &myMutex);  //The Queue is empty, please wait until it is not empty
		}
	}
}

/**
 * consumerUnlock() locks the Queue after the thread to removes an item
 */
template<class Item>
void Queue<Item>::consumerUnlock() {
	pthread_cond_signal(&notFull);  //Signal that the Queue is not full
	pthread_mutex_unlock( &myMutex );  //Unlock before returning
}

//Accessors

/**
 * getArray() is the accessor method for the array of the Queue.
 * @return: array, the array of items in the Queue.
 */
template<class Item>
Item* Queue<Item>::getArray() {
	Item * array;
	pthread_mutex_lock( &myMutex );
	array = myArray;
	pthread_mutex_unlock( &myMutex );
	return array;
}

/**
 * getCount() is the accessor method for the number of elements currently in the Queue.
 * @return: count, an int representing the current number of elements in the Queue.
 */
template<class Item>
int Queue<Item>::getCount() {
	int count;
	pthread_mutex_lock( &myMutex );
	count = myCount;
	pthread_mutex_unlock( &myMutex );
	return count;
}

/**
 * getFirst() is the accessor method for the first element in the Queue.
 * @return: first, an Item representing the first element in the Queue.
 */
template<class Item>
Item Queue<Item>::getFirst() {
	Item first;
	pthread_mutex_lock( &myMutex );
	first = myArray[myFirst];
	pthread_mutex_unlock( &myMutex );
	return first;
}

/**
 * getLast() is the accessor method for the last element in the Queue.
 * @return: last, an Iteme representing the last element in the Queue.
 */
template<class Item>
Item Queue<Item>::getLast() {
	Item last;
	pthread_mutex_lock( &myMutex );
	last = myArray[(myLast-1+mySize) % mySize];
	pthread_mutex_unlock( &myMutex );
	return last;
}

/**
 * getCapacity() is the accessor method for the size of the Queue.
 * @return: size, an int representing the size of the Queue.
 */
template<class Item>
int Queue<Item>::getCapacity() {
	int size;
	pthread_mutex_lock( &myMutex );
	size = mySize;
	pthread_mutex_unlock( &myMutex );
	return size;
}

/**
 * getPthreadIds() is the accessor method for the stored pthread ids of the Producers.
 * (They are used to determine the color of the Rectangle drawn around the circles in the visualization).
 * @return: ids, the array of stored pthread ids.
 */
template<class Item>
int* Queue<Item>::getPthreadIds() {
	int * ids;
	pthread_mutex_lock( &myMutex );
	ids = myPthreadIds;
	pthread_mutex_unlock( &myMutex );
	return ids;
}

//Utility methods

/**
 * isEmpty() determines if the Queue is empty or not.
 * @return: myCount == 0, a boolean expression which evalutes to true or false.
 */
template<class Item>
bool Queue<Item>::isEmpty() const {
	return myCount == 0;
}

/**
 * isFull() determines if the Queue is full or not.
 * @return: myCount == mySize, a boolean expression which evalutes to true or false.
 */
template<class Item>
bool Queue<Item>::isFull() const {
	return myCount == mySize;
}

/**
 * Destructor for the Queue.
 */
template<class Item>
Queue<Item>::~Queue() {
	pthread_cond_destroy(&notEmpty);  //Destroy the condition variables and mutex
	pthread_cond_destroy(&notFull);
	pthread_cond_destroy(&notOpen);
	pthread_mutex_destroy(&myMutex);
	delete [] myArray;      //Deallocate the array
	myArray = NULL;
}

#endif /*QUEUE_H_*/
