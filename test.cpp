#include <algorithm>
#include <cstddef>
#include <stdexcept>

template <typename T>
class DynamicArray {
public:
    // Constructs a new dynamic array with an initial capacity of 16 elements.
    DynamicArray() : DynamicArray(16) {}

    // Constructs a new dynamic array with the given initial capacity.
    explicit DynamicArray(std::size_t capacity) : capacity_(capacity), size_(0), data_(new T[capacity]) {}

    // Destructor. Deallocates the memory used by the dynamic array.
    ~DynamicArray() { delete[] data_; }

    // Adds an element to the end of the array.
    void push_back(const T& element) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = element;
    }

    // Removes the last element from the array and returns it.
    T pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Cannot pop an element from an empty dynamic array");
        }
        return data_[--size_];
    }

    // Inserts an element at the given index.
    void insert(std::size_t index, const T& element) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
        std::move_backward(data_ + index, data_ + size_, data_ + size_ + 1);
        data_[index] = element;
        ++size_;
    }

    // Removes the element at the given index.
    void remove(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        std::move(data_ + index + 1, data_ + size_, data_ + index);
        --size_;
    }

    // Returns the element at the given index.
    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    // Returns the element at the given index.
    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    // Returns the number of elements in the array.
    std::size_t size() const { return size_; }

    // Returns the capacity of the array.
    std::size_t capacity() const { return capacity_; }

private:
    // Resizes the array to the given capacity, copying over the existing elements.
    void resize(std::size_t new_capacity) {
        T* new_data = new T[new_capacity];
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }