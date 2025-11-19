#pragma once


#include <iostream>
#include <memory_resource>
#include <cstdlib>
#include <map>
#include <memory>


class CustomMemoryResource : public std::pmr::memory_resource {
    std::map<void*, size_t> used_blocks;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        if (bytes == 0) bytes = 1;

        if (alignment == 0 || (alignment & (alignment - 1)) != 0)
            throw std::bad_alloc();

        void* ptr = ::operator new(bytes, std::align_val_t(alignment));
        if (!ptr)
            throw std::bad_alloc();

        used_blocks[ptr] = bytes;
        return ptr;
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        auto it = used_blocks.find(ptr);
        if (it == used_blocks.end())
            throw std::logic_error("Attempt to free unknown block");

        ::operator delete(ptr, std::align_val_t(alignment));
        used_blocks.erase(it);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~CustomMemoryResource() override {
        for (auto& [ptr, _] : used_blocks) {
            std::free(ptr);
        }
        used_blocks.clear();
    }
};




template <class T>
class DynamicArrayIterator {
    using MapIter = typename std::map<size_t, T*>::iterator;
    MapIter it;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    DynamicArrayIterator(MapIter it) : it(it) {}

    reference operator*() const { return *(it->second); }
    pointer operator->() const { return it->second; }

    DynamicArrayIterator& operator++() { ++it; return *this; }

    bool operator==(const DynamicArrayIterator& other) const { return it == other.it; }
    bool operator!=(const DynamicArrayIterator& other) const { return it != other.it; }
};




template <class T>
class DynamicArray {
    using Alloc = std::pmr::polymorphic_allocator<T>;

    std::map<size_t, T*> elements; 
    Alloc alloc;
    size_t size_;

public:
    using iterator = DynamicArrayIterator<T>;

    DynamicArray(Alloc alloc = Alloc{}) : alloc(alloc), size_(0) {}

    size_t size() const noexcept { return size_; }

    void push_back(const T& value) {
        T* ptr = alloc.allocate(1);
        alloc.construct(ptr, value);
        elements[size_++] = ptr;
    }

    T& operator[](size_t index) {
        if (!(elements.contains(index)))
            throw std::out_of_range("Index out of range");
        return *(elements[index]);
    }

    const T& operator[](size_t index) const {
        if (!(elements.contains(index)))
            throw std::out_of_range("Index out of range");
        return *(elements[index]);
    }

    iterator begin() { return iterator(elements.begin()); }
    iterator end() { return iterator(elements.end()); }

    ~DynamicArray() {
        for (auto& [i, ptr] : elements) {
            alloc.destroy(ptr);
            alloc.deallocate(ptr, 1);
        }
    }
};