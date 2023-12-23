#pragma once

#include <algorithm>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <cstdint>

template <typename T>
class Vector {
public:
    /**
     * TODO: Define the following associated types (i.e. `using blabla = T`, then blabla is an associated type)
     *       - `size_type`: should be an unsigned integer type
     *       - `difference_type`: should be a signed integer type
     *       - `reference`: reference to value_type
     *       - `const_reference`: const reference to value_type
     *       - `pointer`: pointer to value_type
     *       - `const_pointer`: const pointer to value_type
     *       - `iterator`: a random access iterator to the underlying data
     *       - `const_iterator`: a random access iterator to the underlying data
     *       - `reverse_iterator`: a random access iterator to the underlying data
     *       - `const_reverse_iterator`: a random access iterator to the underlying data
     *
     *       All of these are types aliases, and all of them are easy. Check e.g. the reference
     *       of `std::vector` (hint, pointers are iterators too), check reverse
     */
    using value_type = T;
    using size_type = std::uint64_t;
    using difference_type = std::int64_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::shared_ptr<value_type>;
    using const_pointer = std::shared_ptr<const value_type>;
    using iterator = value_type*; 
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Vector() = default;

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_type n, const_reference default_val) 
    :  _size{n}, _capacity{n}, _data{std::make_unique<value_type[]>(n)}{
        std::fill(_data.get(), _data.get() + n, default_val);
    }

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<value_type> l) 
    : _size{l.size()}, _capacity{l.size()}, _data{std::make_unique<value_type[]>(_size)} {
        std::copy(l.begin(), l.end(), _data.get());
    }

    Vector(const Vector& copy) 
    : _size(copy._size), _capacity(copy._capacity), _data{std::make_unique<value_type[]>(_size)} {
        std::copy(copy._data.get(), copy._data.get() + _size, _data.get());
    }

    Vector(Vector&& move) 
    : _size(move._size), _capacity(move._capacity), _data{std::make_unique<value_type[]>(move._size)} {
        std::unique_ptr<value_type[]> new_array = std::make_unique<value_type[]>(move._size);
        std::move(move._data.get(), move._data.get() + move._size, new_array.get());
        _size = move._size; 
        _capacity = move._capacity;
        _data = std::move(new_array);

        move._size = 0;
        move._capacity = 0;
    }

    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(const Vector& copy) {
        // std::unique_ptr<value_type[]> copy_array{copy._size};
        std::unique_ptr<value_type[]> copy_array  = std::make_unique<value_type[]>(copy._size);
        std::copy(copy._data.get(), copy._data.get() + copy._size, copy_array.get());
        _size = copy._size;
        _capacity = copy._capacity;

        _data = std::move(copy_array);

        return *this;
    }


    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(Vector&& move) noexcept {
        if (this != &move)
        {
            std::unique_ptr<value_type[]> new_array = std::make_unique<value_type[]>(move._size);
            std::move(move._data.get(), move._data.get() + move._size, new_array.get());
            _size = move._size; 
            _capacity = move._capacity;
            _data = std::move(new_array);

            move._size = 0;
            move._capacity = 0;
        }

        return *this;
    }

    size_type size() const noexcept { return _size; }

    size_type capacity() const noexcept { return _capacity; }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const_reference value) {
        resize(calculate_capacity(_size + 1));
        _data[_size] = value;
        _size++;
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value) {
        resize(calculate_capacity(_size + 1));
        _data[_size] = value;
        _size++; 
    }

    /**
     * Removes the last element of the vector.
     */
    void pop_back() {
        _data[_size - 1].~value_type();
        _size--;
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    reference at(const size_type pos) const {
        if (_size == 0 || pos > _size - 1)
        {
            throw std::out_of_range{"invalid range given."};
        }
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    reference at(const size_type pos) {
        if (_size == 0 || pos > _size - 1)
        {
            throw std::out_of_range{"invalid range given."};
        }
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    const_reference operator[](size_type index) const {
        return _data[index];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    reference operator[](const size_type index) {
        return _data[index];
    }

    // TODO: implement: 
    //           - begin, end (both const and non-const)
    //           - cbegin, cend,
    //           - rbegin, rend (const and non-const)
    //           - crbegin, crend (see e.g. https://en.cppreference.com/w/cpp/iterator/rbegin)
    //       This isn't necessarily very complicated, but a little tiresome, but that's part of C++ (yeah....)

    iterator begin()
    {
        return _data.get();
    }

    iterator end()
    {
        return _data.get() + _size;
    }

    const_iterator begin() const
    {
        return _data.get(); 
    }

    const_iterator end() const
    {
        return _data.get() + _size;
    }

    const_iterator cbegin() const
    {
        return _data.get();
    }

    const_iterator cend() const
    {
        return _data.get() + _size;
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const
    {

        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(begin());
    }


    /**
     * Stream a Vector to an output stream textually.
     */
    friend std::ostream& operator<<(std::ostream& o, Vector<value_type> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_type i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_type growth_factor = 2;
    size_type _size;
    size_type _capacity;

    // Holds vector's data.
    std::unique_ptr<value_type[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_type calculate_capacity(size_type new_size) {
        if (new_size >= _capacity)
        {
            return std::max(growth_factor * _capacity, new_size);
        }
        return _capacity;
    }

    /**
     * Resizes the vector to new_capacity.
     * If the current capacity is less than new_capacity
     * the vector moves all elements to a new array.
     */
    void resize(size_type new_capacity) {
        if (_capacity < new_capacity)
        {
            _capacity = new_capacity;

            std::unique_ptr<value_type[]> new_array = std::make_unique<value_type[]>(_capacity);
            std::move(_data.get(), _data.get() + _size, new_array.get());
            _data = std::move(new_array);
        }
    }
};
