/**
 * RWDatabase.h provides a RWDatabase class for the Reader-Writer visualization.
 * This class provides a superclass for the monitors with Reader or Writer preference, which are used by the threads.
 */

#ifndef RWMONITOR_H_
#define RWMONITOR_H_

#include <pthread.h>
#include <vector>
#include <iostream>
using namespace std;

template<class Item>

/**
 * \class RWDatabase
 * \brief An abstract database protecting a vector of data
 * \details Database has its locks and a vector
 * \details Locking methods must be implemented in subclass, giving priority to different types of threads.
 */
class RWDatabase {
public:
	RWDatabase();					//Default constructor
	RWDatabase(int max);				//Explicit constructor
	virtual void startRead() = 0;	//Must be defined by subclass
	virtual void endRead() = 0;		//Must be defined by subclass
	virtual void startWrite() = 0;	//Must be defined by subclass
	virtual void endWrite() = 0;	//Must be defined by subclass
	int getItemCount() { return vec.size(); }	//Get number of items in vector
	int getMaxCapacity() { return maxCapacity; }//Get maximum items in vector
	Item read(int index);						//Access item at index
	void write(Item it, unsigned index);				//Set item at index

protected:
	std::vector<Item> vec;
	int activeWriters, activeReaders, waitingWriters, waitingReaders;
	unsigned maxCapacity;
	pthread_mutex_t lock;
	pthread_cond_t okToRead, okToWrite;
};

/**
 * \brief Default constructor for the RWDatabase class
 * \return: The constructed RWDatabase object.
 */
template<class Item>
RWDatabase<Item>::RWDatabase() {
	vec = vector<Item>();
	activeWriters = activeReaders = waitingWriters = waitingReaders = maxCapacity = 0;
	lock = PTHREAD_MUTEX_INITIALIZER;
	okToRead = PTHREAD_COND_INITIALIZER;
	okToWrite = PTHREAD_COND_INITIALIZER;
}

/**
 * \brief Explicit constructor for the RWDatabase class
 * \param max, an int describing the maximum size of the contained vector
 * \return: The constructed RWDatabase object.
 */
template<class Item>
RWDatabase<Item>::RWDatabase(int max) {
	vec = vector<Item>();
	activeWriters = activeReaders = waitingWriters = waitingReaders = 0;
	lock = PTHREAD_MUTEX_INITIALIZER;
	okToRead = PTHREAD_COND_INITIALIZER;
	okToWrite = PTHREAD_COND_INITIALIZER;
	maxCapacity = max;
}

/**
 * \brief read() returns the Item from the vector at an  index
 * \param index, an index in the vector to access a piece of data
 */
template<class Item>
Item RWDatabase<Item>::read(int index) {
	return vec[index];
}

/**
 * \brief write() sets an item at an index
 * \param it, an Item to add to the vector
 * \param index, the index to add the Item it
 */
template<class Item>
void RWDatabase<Item>::write(Item it, unsigned index) {
	if( index >= vec.size() ) {
		if( index > maxCapacity )
			throw range_error("Item added beyond max capacity");
		else
			vec.push_back(it);
	} else {
		vec[index] = it;
	}
}

#endif /*RWMONITOR_H_*/
