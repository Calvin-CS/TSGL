/*
 * Array.h provides a simple array for storing shapes
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/11/2014
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <mutex>			// Needed for locking the list for thread-safety
#include <stdexcept>		// Needed for exceptions

template <typename Item>
class Array {
private:
	unsigned int first_, last_, capacity_;
	Item* myArray;				// Our array that doesn't look like one...

	std::mutex mutex_;			// Mutex for locking the list so that only one
								// thread can read/write at a time
public:
//	Array() {
//		// EHLOO
//	}

	Array(unsigned int size) {
		capacity_ = size;
		myArray = new Item[size];
		first_ = last_ = 0;
	}

	virtual ~Array() {
		clear();
		delete [] myArray;
	}

	void clear() {
		std::unique_lock<std::mutex> mlock(mutex_);
		if (first_ > last_) {
			for (; first_ < capacity_; first_++) {
				delete myArray[first_];
			}
			first_ = 0;
		}

		for (; first_ <= last_; first_++) {
			delete myArray[first_];
			myArray[first_] = NULL;
		}

		first_ = last_ = 0;
		mlock.unlock();
	}

	const Item operator[] (unsigned int index) {
		std::unique_lock<std::mutex> mlock(mutex_);
		Item item = myArray[(first_ + index) % capacity_];
		mlock.unlock();
		return item;
	}

	unsigned int size() {
		std::unique_lock<std::mutex> mlock(mutex_);
		unsigned int size;
		if (myArray[first_] == NULL) {
			size = 0;
		} else {
			size = first_ > last_  ? last_ + first_ - capacity_ + 1 : last_ - first_ + 1;
		}
		mlock.unlock();
		return size;
	}

	unsigned int capacity() const {
		return capacity_;
	}

	bool isEmpty() {
		std::unique_lock<std::mutex> mlock(mutex_);
		bool empty = myArray[first_] == NULL;
		mlock.unlock();
		return empty;
	}

	Item push(Item item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		if (myArray[first_] != NULL) {
			(last_ + 1) == capacity_ ? last_ = 0 : last_++;
		}

		if (last_ == first_ && myArray[first_] != NULL) {
			delete myArray[first_];
			(first_ + 1) == capacity_ ? first_ = 0 : first_++;
		}

		myArray[last_] = item;
		mlock.unlock();
		return item;
	}
};

#endif /* ARRAY_H_ */
