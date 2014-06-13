/*
 * Array.h provides a simple array for storing shapes
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/12/2014
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

//	std::mutex mutex_;			// Mutex for locking the list so that only one
								// thread can read/write at a time
public:
//	Array() {
//		// EHLOO
//	}

	Array(unsigned int size) {
		capacity_ = size;
		myArray = new Item[size];
		first_ = last_ = size_ = 0;
	}

	virtual ~Array() {
		clear();
		delete [] myArray;
	}

	/*
	 * clear() empties the array and resets it
	 */
	void clear() {
//		std::unique_lock<std::mutex> mlock(mutex_);
		if (first_ > last_) {						// If the array wraps around...
			for (; first_ < capacity_; first_++) {	// Delete from first to the end
				delete myArray[first_];
			}
			first_ = 0;								// Move first to the beginning
		}

		for (; first_ <= last_; first_++) {			// Delete from first to last
			delete myArray[first_];
			myArray[first_] = NULL;
		}

		first_ = last_ = size_ = 0;					// Reset all vars
//		mlock.unlock();
	}

	/*
	 * operator[] returns the item at the index
	 * Parameters:
	 * 		index, the index of where the item is
	 * Returns: the item at that index
	 */
	const Item operator[] (unsigned int index) {
//		std::unique_lock<std::mutex> mlock(mutex_);
		if (size_ == 0) {
//			mlock.unlock();
			throw std::out_of_range("Array::operator[](): Array is empty");
		} else {
			Item item = myArray[(first_ + index) % capacity_];	// Wrap around for the underlying array
//			mlock.unlock();
			return item;
		}
	}

	/*
	 * size() returns the number of items in the array
	 */
	unsigned int size() {
		return size_;
	}

	/*
	 * capacity() returns the maximum ammount of items the array can store
	 */
	unsigned int capacity() const {
		return capacity_;
	}

	/*
	 * isEmpty() returns true if the array has no items, false otherwise
	 */
	bool isEmpty() {
//		std::unique_lock<std::mutex> mlock(mutex_);
		bool empty = myArray[first_] == NULL;				// If there is no item...
//		mlock.unlock();
		return empty;
	}

	/*
	 * push() adds the item to the end of the array. It will also remove the
	 * 	oldest item if the array is full
	 * Parameters:
	 * 		item, the item to add
	 * Returns: the same item
	 */
	Item push(Item item) {
//		std::unique_lock<std::mutex> mlock(mutex_);
		if (myArray[first_] != NULL) {							// If the array has no items...
			(last_ + 1) == capacity_ ? last_ = 0 : last_++;		// Increment last
		}

		if (last_ == first_ && myArray[first_] != NULL) {		// If the array is out of room...
			delete myArray[first_];
			(first_ + 1) == capacity_ ? first_ = 0 : first_++;	// Increment first
		} else {
			size_++;											// Otherwise, we added an item
		}

		myArray[last_] = item;									// Actually add the item

//		mlock.unlock();
		return item;
	}
};

#endif /* ARRAY_H_ */
