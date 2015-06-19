/*
 * Array.h provides a simple array for storing shapes.
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdexcept>  // Needed for exceptions

namespace tsgl {

/*!
 * \class Array
 * \brief Creates an Array object.
 * \details Array creates an Array object capable of storing shapes to be drawn.
 * \details It creates an Array object with a specified capacity.
 * \details Shapes to be drawn are pushed into the Array object.
 * \details Contains utility methods for checking if the Array object is empty,
 *          emptying and resetting an Array object, and a subscript operator
 *          for accessing individual elements.
 * \note The Array has wrap-around behavior so it behaves in a similar fashion as a circular Queue.
 * \note If a new shape is pushed into a full Array object, the first element is deleted
 *       and the pointer to the first element is incremented.
 * \note There is also a method that clears an Array object out but doesn't
 *       delete the shapes inside of it ( see shallowClear() ).
 */
template<typename Item>
class Array {
 private:
    unsigned int first_, last_, capacity_, size_;
    Item* myArray;
 public:

    /*!
     * \brief Constructor for an Array object.
     * \param size the requested size of the Array object.
     * \return An Array object with capacity size.
     */
    Array(unsigned int size) {
        capacity_ = size;
        myArray = new Item[size];
        myArray[0] = nullptr;
        first_ = last_ = size_ = 0;
    }

    /*!
     * \brief Destroys an Array object.
     * \details Destructor for an Array object.
     * \details Frees up memory allocated to an Array object.
     */
    virtual ~Array() {
        clear();
        delete[] myArray;
    }

    /*!
     * \brief clear() empties the array and resets it.
     */
    void clear() {
        if (first_ > last_) {                       // If the array wraps around...
            for (; first_ < capacity_; first_++) {  // Delete from first to the end
                delete myArray[first_];
                myArray[first_] = nullptr;
            }
            first_ = 0;                             // Move first to the beginning
        }
        for (; first_ <= last_; first_++) {         // Delete from first to last
            delete myArray[first_];
            myArray[first_] = nullptr;
        }
        first_ = last_ = size_ = 0;                 // Reset all vars
    }

    /*!
     * \brief shallowClear empties the array but does not delete the objects.
     * \warning WILL RESULT IN MEMORY LEAK IF THE OBJECTS ARE NOT POINTED TO ANYWHERE ELSE!
     */
    void shallowClear() {
        if (first_ > last_) {                       // If the array wraps around...
            for (; first_ < capacity_; first_++)    // Delete from first to the end
                myArray[first_] = nullptr;
            first_ = 0;                             // Move first to the beginning
        }
        for (; first_ <= last_; first_++)           // Delete from first to last
            myArray[first_] = nullptr;
        first_ = last_ = size_ = 0;                 // Reset all vars
    }

    /*!
     * \brief operator[] returns the item at the index.
     * \param index the index of where the item is.
     * \note This is the read version of the subscript operator.
     * \return the item at that index.
     */
    const Item& operator[](unsigned int index) const {
        if (size_ == 0)
            throw std::out_of_range("Array::operator[](): array is empty");
        else if (index >= size_)
            throw std::out_of_range("Array::operator[](): index is larger than number of items in array");
        else
            return myArray[(first_ + index) % capacity_];  // Wrap around for the underlying array
    }

    /*!
     * \brief operator[] returns the item at the index.
     * \param index the index of where the item is.
     * \note This is the write version of the subscript operator.
     * \return the item at that index.
     */
    Item& operator[](unsigned int index) {
        if (size_ == 0) {
            throw std::out_of_range("Array::operator[](): array is empty");
        } else if (index >= size_) {
            throw std::out_of_range("Array::operator[](): index is larger than number of items in array");
        } else {
            return myArray[(first_ + index) % capacity_];  // Wrap around for the underlying array
        }
    }


    /*! \brief size() returns the number of items in the array. */
    unsigned int size() const {
        return size_;
    }

    /*! \brief capacity() returns the maximum amount of items the array can store. */
    unsigned int capacity() const {
        return capacity_;
    }

    /*! \brief isEmpty() returns true if the array has no items, false otherwise. */
    bool isEmpty() const {
        return (size_ == 0);
    }

    /*!
     * \brief push() adds the item to the end of the array. It will also remove the
     *  oldest item if the array is full.
     * \param item the item to add.
     * \return the same item.
     */
    Item push(Item item) {
        if (myArray[first_] != nullptr)                         // If the array has items...
        (last_ + 1) == capacity_ ? last_ = 0 : last_++;         // Increment last

        if (last_ == first_ && myArray[first_] != nullptr) {    // If the array is out of room...
            delete myArray[first_];                             // Delete the first element
            (first_ + 1) == capacity_ ? first_ = 0 : first_++;  // Increment first
        } else
            size_++;                                            // Otherwise, we added an item

        return myArray[last_] = item;                           // Actually add the item
    }
};

}

#endif /* ARRAY_H_ */
