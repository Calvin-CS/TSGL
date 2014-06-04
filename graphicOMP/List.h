/*
 * List.h
 *
 *  Created on: Jun 4, 2014
 *      Author: mbv26
 */

#ifndef LIST_H_
#define LIST_H_

#include <thread>
#include <mutex>
#include <stdlib.h>

template <typename Item>
class List {
friend class Iterator;
public:
	List() {
		_first = _last = NULL;
	}

	virtual ~List() {
		delete _first;
	}

	Item pop() {
		std::unique_lock<std::mutex> mlock(mutex_);

		Item item = _last->_item;
		_last = _last->_previous;
		delete _last->_next;
		if (_last == NULL) { _first == NULL; }

		mlock.unlock();

		return item;
	}

	void remove() {
		std::unique_lock<std::mutex> mlock(mutex_);

		_last = _last->_previous;
		delete _last->_next;
		if (_last == NULL) { _first == NULL; }

		mlock.unlock();
	}

	void push(const Item item) {
		std::unique_lock<std::mutex> mlock(mutex_);

		if (_first == NULL) {
			_first = _last = new Node(item, _last);
		} else {
			_last = _last->_next = new Node(item, _last);
		}

		mlock.unlock();
	}

	bool isEmpty() { return _first == NULL; }

private:
	struct Node {
//	friend class Iterator;
		Node(Item item, Node * previous) {
			_item = item;
			_previous = previous;
			_next = NULL;
		}
		~Node() {
			delete _next;
		}

		Node * _previous, _next;
		Item _item;
	};

	Node * _first, _last;

	std::mutex mutex_;
};

//class Iterator {
////friend class Node;
//public:
//	Iterator(List * list) {
//		_list = list;
//		current = _list._first;
//	}
//
//
//private:
//	List * _list;
//	Node * current;
//};

#endif /* LIST_H_ */
