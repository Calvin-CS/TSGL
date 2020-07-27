/**
 * RWDatabase.h provides a RWDatabase class for the Reader-Writer visualization.
 * This class provides a superclass for the monitors with Reader or Writer preference, which are used by the threads.
 */

#ifndef RWDATA_H_
#define RWDATA_H_

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
	int getItemCount() { return vec.size(); }	//Get number of items in vector
	int getMaxCapacity() { return maxCapacity; }//Get maximum items in vector
	Item read(unsigned index);						//Access item at index
	void write(Item it, unsigned index);				//Set item at index

protected:
	std::vector<Item> vec;
	unsigned maxCapacity;
};

/**
 * \brief Default constructor for the RWDatabase class
 * \return: The constructed RWDatabase object.
 */
template<class Item>
RWDatabase<Item>::RWDatabase() {
	vec = vector<Item>();
}

/**
 * \brief Explicit constructor for the RWDatabase class
 * \param max, an int describing the maximum size of the contained vector
 * \return: The constructed RWDatabase object.
 */
template<class Item>
RWDatabase<Item>::RWDatabase(int max) {
	vec = vector<Item>();
	maxCapacity = max;
}

/**
 * \brief read() returns the Item from the vector at an  index
 * \param index, an index in the vector to access a piece of data
 */
template<class Item>
Item RWDatabase<Item>::read(unsigned index) {
	if( index >= vec.size() )
		throw range_error("Access attempted beyond present items");
	else
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

#endif /*RWDATA_H_*/
