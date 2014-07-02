/*
 * Array.h provides a simple array for storing shapes
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/20/2014
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <mutex>				// Needed for locking the list for thread-safety
#include <stdexcept>			// Needed for exceptions

template <typename Item>
class Array {
private:
	unsigned int first_, last_, capacity_, size_;
	Item* myArray;				// Our array that doesn't look like one...
public:
	Array(unsigned int size) {
		capacity_ = size;
		myArray = new Item[size];
		myArray[0] = nullptr;
		first_ = last_ = size_ = 0;
	}

	virtual ~Array() { clear(); delete [] myArray; }

	/*
	 * clear() empties the array and resets it
	 */
	void clear() {
		if (first_ > last_) {						// If the array wraps around...
			for (; first_ < capacity_; first_++) {	// Delete from first to the end
				delete myArray[first_];
				myArray[first_] = nullptr;
			}
			first_ = 0;								// Move first to the beginning
		}
		for (; first_ <= last_; first_++) {			// Delete from first to last
			delete myArray[first_];
			myArray[first_] = nullptr;
		}
		first_ = last_ = size_ = 0;					// Reset all vars
	}

	/*
	 * shallowClear empties the array but does not delete the objects
	 * 	WILL RESULT IN MEMORY LEAK IF THE OBJECTS ARE NOT POINTED TO ANYWHERE ELSE!
	 */
	void shallowClear() {
			if (first_ > last_) {						// If the array wraps around...
				for (; first_ < capacity_; first_++)	// Delete from first to the end
					myArray[first_] = nullptr;
				first_ = 0;								// Move first to the beginning
			}
			for (; first_ <= last_; first_++)			// Delete from first to last
				myArray[first_] = nullptr;
			first_ = last_ = size_ = 0;					// Reset all vars
		}

	/*
	 * operator[] returns the item at the index
	 * Parameters:
	 * 		index, the index of where the item is
	 * Returns: the item at that index
	 */
	const Item operator[] (unsigned int index) const {
		if (size_ == 0)
			throw std::out_of_range("Array::operator[](): array is empty");
		else if (index >= size_)
			throw std::out_of_range("Array::operator[](): index is larger than number of items in array");
		else
			return myArray[(first_ + index) % capacity_];	// Wrap around for the underlying array
	}

	/* size() returns the number of items in the array */
	unsigned int size() const { return size_; }

	/* capacity() returns the maximum ammount of items the array can store */
	unsigned int capacity() const { return capacity_; }

	/* isEmpty() returns true if the array has no items, false otherwise */
	bool isEmpty() const { return (size_ == 0); }

	/*
	 * push() adds the item to the end of the array. It will also remove the
	 * 	oldest item if the array is full
	 * Parameters:
	 * 		item, the item to add
	 * Returns: the same item
	 */
	Item push(Item item) {
		if (myArray[first_] != nullptr)						// If the array has items...
			(last_ + 1) == capacity_ ? last_ = 0 : last_++;		// Increment last

		if (last_ == first_ && myArray[first_] != nullptr) {	// If the array is out of room...
			delete myArray[first_];								// Delete the first element
			(first_ + 1) == capacity_ ? first_ = 0 : first_++;	// Increment first
		} else size_++;											// Otherwise, we added an item

		return myArray[last_] = item;							// Actually add the item
	}
};

#endif /* ARRAY_H_ */
