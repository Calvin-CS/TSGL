/*
 * Array.h provides a simple array for storing shapes.
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdexcept>  // Needed for exceptions

namespace tsgl {

/*!
 * \class Array
 * \brief Custom internal array used by Canvas.
 * \details The Array class manages a custom array for storing shapes to be drawn.
 *   It contains utility methods for checking if the internal array is empty,
 *   methods for emptying and resetting the internal array, and a subscript operator
 *   for accessing individual elements.
 * \note The Array has wrap-around behavior, behaving similarly to a circular queue.
 * \note If a new shape is pushed into a full Array, the first element is deleted
 *   and the pointer to the first element is incremented.
 */
template<typename Item>
class Array {
 private:
    unsigned int first_, last_, capacity_, size_;
    Item* myArray;
 public:

    /*!
     * \brief Array constructor method.
     * \param size The maximum capacity of the Array.
     * \return An Array with capacity <code>size</code>.
     */
    Array(unsigned int size) {
        capacity_ = size;
        myArray = new Item[size];
        myArray[0] = nullptr;
        first_ = last_ = size_ = 0;
    }

    /*!
     * \brief Array destructor method.
     * \details Frees up memory allocated to an Array instance.
     */
    virtual ~Array() {
        clear();
        delete[] myArray;
    }

    /*!
     * \brief Empties the internal array and resets it, deleting contained objects.
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
     * \brief Empties the internal array but does not delete the objects it contains.
     * \note This method doesn't delete the shapes inside of it; it only moves pointers around.
     * \warning <b>This will result in a memory leak if the objects are not pointed to anywhere else!</b>
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
     * \brief Returns the item at index <code>index</code>.
     * \param index The index of the item in the internal array.
     * \note This is the read version of the subscript operator.
     * \return The item at index <code>index</code>.
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
     * \brief Returns the item at index <code>index</code>.
     * \param index The index of the item in the internal array.
     * \note This is the write version of the subscript operator.
     * \return The item at index <code>index</code>.
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


    /*! \brief Returns the number of items in the internal array. */
    unsigned int size() const {
        return size_;
    }

    /*! \brief Returns the maximum amount of items the internal array can store. */
    unsigned int capacity() const {
        return capacity_;
    }

    /*! \brief Returns true if the internal array contains no items, false otherwise. */
    bool isEmpty() const {
        return (size_ == 0);
    }

    /*!
     * \brief Adds the item <code>item</code> to the end of the internal array.
     *  \note If the internal array is full, push() will remove the oldest item.
     * \param item The item to add.
     * \return The same item.
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

    /*!
     * \brief Removes the item <code>item</code> from the internal array.
     *  \note If the internal array does not contain the item, this does nothing.
     * \param item The item to remove.
     */
    void remove(Item item) {
        int itemIndex = -1;
        for (unsigned int i = 0; i < size_; i++) {
            if(myArray[i] == item) {
                itemIndex = i;
                break;
            }
        }
        if(itemIndex != -1) {
            for (unsigned int i = itemIndex; i < size_; i++) {
                if(i == size_ - 1) {
                    size_--;
                    myArray[i] = nullptr;
                } else {
                    myArray[i] = myArray[i+1];
                }
            }
        }
    }
};

}

#endif /* ARRAY_H_ */
