/* QueueTester.cpp defines the test-methods for class Queue.
 * Student Name: cpd5 (Chris Dilley).
 * Date: 04/23/14 (Hand-in date).
 * Joel Adams, for CS 112 at Calvin College.
 */

#include "QueueTester.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
using namespace std;

void ArrayQueueTester::runTests() {
    cout << "Testing class Queue..." << endl;
    testConstructor();
    testAppend();
    testRemove();
  //  testCopyConstructor();
   // testAssignment();
    //new test method
   // testSetCapacity();
    cout << "All tests passed!\n" << endl;
}

void ArrayQueueTester::testRemove() {
    cout << "- testing remove()... " << flush;
    Queue<int> q1(4);
    cout << " 0 " << flush;
    // check with empty queue
  //  try {
  //      q1.remove();
  //      cerr << "\nremove() worked on empty Queue - 1\n";
  //      exit(1);
  //  } catch (EmptyQueueException& eqe) {
  //      cout << " 1 " << flush;
  //  }
    // check with 1 item
    q1.append(1);
    assert( !q1.isEmpty() );
    assert( q1.remove() == 1 );
    assert( q1.isEmpty() );
    cout << " 2 " << flush;
    // check with 2 items
    q1.append(11);
    q1.append(22);
    assert( !q1.isEmpty() );
    assert( q1.getFirst() == 11 );
    assert( q1.getLast() == 22 );
    assert( q1.remove() == 11 );
    assert( q1.getFirst() == 22 );
    assert( q1.getLast() == 22 );
    assert( q1.remove() == 22 );
    assert( q1.isEmpty() );
    cout << " 3 " << flush;
    // load it up
    q1.append(111);
    q1.append(222);
    q1.append(333);
    q1.append(444);
    assert( q1.isFull() );
    assert( !q1.isEmpty() );
    assert( q1.getFirst() == 111 );
    assert( q1.getLast() == 444 );
    // start removing items
    assert( q1.remove() == 111 );
    assert( !q1.isFull() );
    assert( !q1.isEmpty() );
    assert( q1.getFirst() == 222 );
    assert( q1.getLast() == 444 );
    cout << " 4a " << flush;
    assert( q1.remove() == 222 );
    assert( !q1.isFull() );
    assert( !q1.isEmpty() );
    assert( q1.getFirst() == 333 );
    assert( q1.getLast() == 444 );
    cout << " 4b " << flush;
    assert( q1.remove() == 333 );
    assert( !q1.isFull() );
    assert( !q1.isEmpty() );
    assert( q1.getFirst() == 444 );
    assert( q1.getLast() == 444 );
    cout << " 4c " << flush;
    assert( q1.remove() == 444 );
    assert( !q1.isFull() );
    assert( q1.isEmpty() );
    cout << " 4d " << flush;
    // recheck empty queue
  //  try {
  //      q1.remove();
  //      cerr << "\nremove() worked on empty Queue - 2\n";
  //      exit(1);
  //  } catch (EmptyQueueException& eqe) {
  //      cout << " 5 " << flush;
  //  }
    cout << "Passed!" << endl;
}

void ArrayQueueTester::testAppend() {
    cout << "- testing append(), getFirst(), getLast() ... " << flush;
    Queue<double> q1(4);
    assert( q1.isEmpty() );
    assert( !q1.isFull() );
  //  try {
  //      q1.getFirst();
  //      cerr << "\ngetFirst() worked on empty queue\n";
  //      exit(1);
  //  } catch (EmptyQueueException& eqe) {
  //      cout << " 0a " << flush;
  //  }
  //  try {
  //      q1.getLast();
  //      cerr << "\ngetLast() worked on empty queue\n";
  //      exit(1);
  //  } catch (EmptyQueueException& eqe) {
  //      cout << " 0b " << flush;
  //  }
    q1.append(11);
    assert( !q1.isEmpty() );
    assert( !q1.isFull() );
    assert( q1.getFirst() == 11 );
    assert( q1.getLast() == 11 );
    cout << " 1 " << flush;
    q1.append(22);
    assert( !q1.isEmpty() );
    assert( !q1.isFull() );
    assert( q1.getFirst() == 11 );
    assert( q1.getLast() == 22 );
    cout << " 2 " << flush;
    q1.append(33);
    assert( !q1.isEmpty() );
    assert( !q1.isFull() );
    assert( q1.getFirst() == 11 );
    assert( q1.getLast() == 33 );
    cout << " 3 " << flush;
    q1.append(44);
    assert( !q1.isEmpty() );
    assert( q1.isFull() );
    assert( q1.getFirst() == 11 );
    assert( q1.getLast() == 44 );
    cout << " 4 " << flush;
    // append to full queue
  //  try {
  //      q1.append(55);
  //      cerr << "\nappend() worked on full queue\n";
  //      exit(1);
  //  } catch (FullQueueException& fqe) {
  //      cout << " 5 " << flush;
  //  }
    cout << "Passed!" << endl;
}

void ArrayQueueTester::testConstructor() {
    cout << "- testing constructor... " << flush;
    // try with capacity zero
 //   try {
 //       Queue<int> q1(0);
 //       cerr << "Constructed Queue with capacity zero" << endl;
 //       exit(1);
 //   } catch (QueueException& qe) {
 //       cout << " 1 " << flush;
 //   }

    // positive capacity
    Queue<int> q2(3);
    assert( q2.isEmpty() );
    assert( !q2.isFull() );
    assert( q2.mySize == 3 );
    cout << " 2 " << flush;
    cout << " Passed!" << endl;
}
