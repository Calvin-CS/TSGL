/*
 * List.h
 * Provides a double linked list with a built in iterator for storing shapes
 *
 *  Created on: Jun 4, 2014
 *      Author: Mark Vander Stel - mbv26
 */

#ifndef LIST_H_
#define LIST_H_

#include <mutex>			// Needed for locking the list for thread-safety
#include <stdexcept>		// Needed for exceptions

template <typename Item>
class List {
private:
	struct Node {			// A double linked node that holds a pointer to the item
		Node(Item item, Node * previous) {
			item_ = item;
			previous_ = previous;
			next_ = NULL;
		}
		~Node() {
			delete item_;
		}

		Node * previous_;
		Node * next_;
		Item item_;
	};

	Node * first_;
	Node * last_;

	int maxSize_;
	unsigned int currentSize_;

	std::mutex mutex_;			// Mutex for locking the list so that only one
								// thread can read/write at a time
public:
	/* Default constructor. Sets no size limits
	 */
	List() {
		first_ = NULL;
		last_ = NULL;
		maxSize_ = -1;
		currentSize_ = 0;
	}

	/* Explicit constructor.
	 * Parameters:
	 * 		size: the max size of the list. Pushing beyond this size will cause
	 * 				the first (oldest) items to be deleted
	 */
	List(int size) {
		first_ = NULL;
		last_ = NULL;
		maxSize_ = size;
		currentSize_ = 0;
	}

	virtual ~List() {
		while (first_ != NULL) {
			first_ = first_->next_;
			delete first_->previous_;
		}
	}

	unsigned size() {
		return currentSize_;
	}

	/* pop() removes the last item and returns it
	 * Returns: the item
	 */
	Item pop() {
		if (first_ == NULL) {
			throw std::out_of_range("List::pop(): Queue is empty");
		} else {
			std::unique_lock<std::mutex> mlock(mutex_);
			Item * item = last_->item_;		// Store the last item
			if (last_ == first_) {			// If only one item in the list...
				clear();					// Simply clear it
			} else {
				last_ = last_->previous_;	// Set the second-to-last to last
				delete last_->next_;		// Delete the old last
				last_->next_ = NULL;		// Clean up
			}
			currentSize_--;
			mlock.unlock();
			return item;
		}
	}

	/* remove() removes the last item
	 *
	 */
	void remove() {
		if (first_ == NULL) {
			throw std::out_of_range("List::remove(): Queue is empty");
		} else {
			std::unique_lock<std::mutex> mlock(mutex_);
			if (last_ == first_) {			// If only one item in the list...
				clear();					// Simply clear it
			} else {
				last_ = last_->previous_;	// Set the second-to-last to last
				delete last_->next_;		// Delete the old last
				last_->next_ = NULL;		// Clean up
			}
			currentSize_--;
			mlock.unlock();
		}
	}

	/* clear() clears the list, removing all items
	 *
	 */
	void clear() {
		delete first_;
		first_ = NULL;
		last_ = NULL;
		currentSize_ = 0;
	}

	/* push takes an item and stores it on the end of the list
	 * Parameters:
	 * 		item: the item to store
	 */
	void push(const Item item) {
		std::unique_lock<std::mutex> mlock(mutex_);

		if (first_ == NULL) {						// If there are no nodes...
			first_ = new Node(item, last_);			// Make a first one
			last_ = first_;							// Which is also the last
		} else {
			last_->next_ = new Node(item, last_);	// Make a new one after the last
			last_ = last_->next_;					// And make it the new last
		}
		currentSize_++;
		if (currentSize_ > maxSize_) {				// If size is greater than the max...
			first_ = first_->next_;					// Move the first node
			delete first_->previous_;				// Delete the old first node
			first_->previous_ = NULL;
			currentSize_--;
		}
		mlock.unlock();
	}

	/* isEmpty() returns true if there are no items in the list, false otherwise
	 *
	 */
	bool isEmpty() { return first_ == NULL; }

	/* Iterator provides a simple iterator for the list, allowing for standard
	 * incrementation, as well as removing items from the list
	 */
	class Iterator {
	friend class List;
	public:
		/* removePrevious() removes the previous item from the list.
		 * If the list has changed since the iterator has moved, it will remove
		 * the item directly before its current item
		 *
		 * Cannot remove if a previous does not exist, i.e.: the current item is the
		 * first item in the list
		 */
		void removePrevious() {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			if (node_ == list_->first_) {
				list_->mutex_.unlock();
				throw std::out_of_range("List::Iterator::removePrevious(): There is no previous node");
			}
			Node * oldNode = node_->previous_;
			if (oldNode->previous_ != NULL) {		// If there is a node in front of previous
				oldNode->previous_->next_ = node_;	// point it at the current
			} else {
				list_->first_ = node_;				// Otherwise, current is the new first
			}
			node_->previous_ = oldNode->previous_;	// Link current to the previous' previous
			delete oldNode;							// Delete
			currentSize_--;
			list_->mutex_.unlock();
		}

		/* removeCurrent() removes the current item from the list. The iterator moves
		 * back to the previous item in the list.
		 *
		 * Cannot remove is a previous does not exist, i.e.: the current item is the
		 * first item in the list, because the iterator must move to the previous item
		 */
		void removeCurrent() {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			if (node_ == list_->first_) {
				list_->mutex_.unlock();
				throw std::out_of_range("List::Iterator::removeCurrent(): Cannot remove the first node");
			}
			Node * oldNode = node_;
			node_ = oldNode->previous_;			// Link the two surrounding nodes together
			node_->next_ = oldNode->next_;
			if (oldNode->next_ != NULL) {		// But only if there are two nodes
				oldNode->next_->previous_ = node_;
			}
			delete oldNode;						// Then delete the node
			currentSize_--;
			list_->mutex_.unlock();
		}

		/* operator*() returns the current item in the list
		 * Returns: the current item
		 */
		Item operator*() {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			Item item = node_->item_;		// Store the item
			list_->mutex_.unlock();
			return item;
		}

		/* Postfix ++() returns the current item, then increments the iterator
		 * Returns: the current item
		 */
		Item operator++(int i) {
			Item item;
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			if (node_ != NULL) {
				item = node_->item_;		// Store the item
				node_ = node_->next_;		// Move along
				list_->mutex_.unlock();
				return item;
			} else {
				list_->mutex_.unlock();
				throw std::out_of_range("Op++(): Queue is empty");
			}
		}

		/* operator!=() returns true if the nodes are not the same, and false if
		 * the are the same. (NOT if they store the same item)
		 *
		 */
		bool operator!= (const Iterator& other) {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			bool equals = (node_ != other.node_);
			list_->mutex_.unlock();
			return equals;
		}

	private:
		// In private because only the list should make iterators, as only the
		// list knows what nodes are
		Iterator(List * list, Node * start) {
			list_ = list;
			node_ = start;
		}

		List * list_;
		Node * node_;
	};

	/* begin() returns an iterator starting at the beginning of the list
	 * Returns: an iterator on the first item
	 */
	Iterator begin() {
		return Iterator(this, first_);
	}

	/* end() returns an iterator starting at the end of the list
	 * Returns: an iterator on the last item
	 */
	Iterator end() {
		return Iterator(this, NULL);
	}
};

#endif /* LIST_H_ */
