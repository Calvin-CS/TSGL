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
		std::unique_lock<std::mutex> mlock(mutex_);

		if (first_ == NULL) {
			mlock.unlock();
			throw std::out_of_range("Pop(): Queue is empty");
		} else {
			Item item = last_->_item;
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
		std::unique_lock<std::mutex> mlock(mutex_);

		if (first_ == NULL) {
			mlock.unlock();
			throw std::out_of_range("Remove(): Queue is empty");
		} else {
			last_ = last_->previous_;
			delete last_->next_;
			if (last_ == NULL) { first_ == NULL; }

			mlock.unlock();
		}
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
		Iterator(Node * start) {
			node_ = start;
		}

		Item operator*() {
			return node_->item_;
		}

		Item operator++(int i) {
			Item item;
			if (node_ != NULL) {
				item = node_->item_;
			} else {
				throw std::out_of_range("Op++(): Queue is empty");
			}
			node_ = node_->next_;
			return item;
		}

		bool operator!= (const Iterator& other) {
			return node_ != other.node_;
		}

	private:
		Node * node_;
	};

	Iterator begin() {
		return Iterator(first_);
	}

	Iterator end() {
		return Iterator(NULL);
	}
};

#endif /* LIST_H_ */
