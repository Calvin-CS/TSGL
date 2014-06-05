/*
 * List.h
 *
 *  Created on: Jun 4, 2014
 *      Author: mbv26
 */

#ifndef LIST_H_
#define LIST_H_

#include <mutex>
#include <stdexcept>
#include <stdlib.h>

template <typename Item>
class List {
friend class Iterator;
private:
	struct Node {
		Node(Item item, Node * previous) {
			item_ = item;
			previous_ = previous;
			next_ = NULL;
		}
		~Node() {
			delete next_;
		}

		Node * previous_;
		Node * next_;
		Item item_;
	};

	Node * first_;
	Node * last_;

	std::mutex mutex_;
public:
	List() {
		first_ = NULL;
		last_ = NULL;
	}

	virtual ~List() {
		delete first_;
	}

	Item pop() {
		if (first_ == NULL) {
			throw std::out_of_range("Pop(): Queue is empty");
		} else {
			std::unique_lock<std::mutex> mlock(mutex_);
			Item * item = last_->_item;
			last_ = last_->_previous;
			delete last_->_next;
			if (last_ == NULL) {
				first_ == NULL;
			}

			mlock.unlock();
			return item;
		}
	}

	void remove() {
		if (first_ == NULL) {
			throw std::out_of_range("Remove(): Queue is empty");
		} else {
			std::unique_lock<std::mutex> mlock(mutex_);
			last_ = last_->previous_;
			delete last_->next_;
			if (last_ == NULL) { first_ = NULL; }

			mlock.unlock();
		}
	}

	void clear() {
		delete first_;
	}

	void push(const Item item) {
		std::unique_lock<std::mutex> mlock(mutex_);

		if (first_ == NULL) {
			first_ = new Node(item, last_);
			last_ = first_;
		} else {
			last_->next_ = new Node(item, last_);
			last_ = last_->next_;
		}
		mlock.unlock();
	}

	bool isEmpty() { return first_ == NULL; }

	class Iterator {
	public:
		Iterator(List * list, Node * start) {
			list_ = list;
			node_ = start;
		}

		void removeCurrent() {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			if (node_ == list_->first_) {
				delete node_->item_;
				node_ = node_->next_;
				node_->previous_->next_ = NULL;
				list_->first_ = node_;
			} else if (node_ == list_->last_) {
				list_->last_ = list_->last_->previous_;
				delete node_;
				if (list_->last_ == NULL) { list_->first_ = NULL; }
			} else {
				Node * oldNode = node_;
				node_ = oldNode->previous_;
				node_->next_ = oldNode->next_;
				oldNode->next_->previous_ = node_;
				oldNode->next_ = NULL;
				delete oldNode;
			}
			list_->mutex_.unlock();
		}

		Item operator*() {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			Item item = node_->item_;
			list_->mutex_.unlock();
			return item;
		}

		void operator++(int i) {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			node_ = node_->next_;
			if (node_->previous_->item_ == NULL) {
				delete node_->previous_;
			}
			list_->mutex_.unlock();
		}

		bool operator!= (const Iterator& other) {
			std::unique_lock<std::mutex> mlock(list_->mutex_);
			bool equals = (node_ != other.node_);
			list_->mutex_.unlock();
			return equals;
		}

	private:
		List * list_;
		Node * node_;
	};

	Iterator begin() {
		return Iterator(this, first_);
	}

	Iterator end() {
		return Iterator(this, NULL);
	}
};

#endif /* LIST_H_ */
