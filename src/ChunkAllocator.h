//
// File: ChunkAllocator.h
// Desc: Chunk memory pool with allocator
//

#ifndef CHUNK_ALLOCATOR_H
#define CHUNK_ALLOCATOR_H

// Overload ::operator new and ::operator delete
#include "Malloc.h"

#include <cstddef>
#include <memory>

template <typename T, size_t Size>
class ChunkAllocator {
    using uchar = unsigned char;
    using uchar_ptr = uchar *;

    constexpr static size_t firstElement = (sizeof(uchar_ptr) + alignof(T) - 1) / alignof(T) * alignof(T);

    class Deleter {
      public:
        void operator()(uchar_ptr ptr) {
            auto p = reinterpret_cast<uchar_ptr *>(ptr);
            while (p != nullptr) {
                auto prev = *p;
                ::operator delete(p);
                p = reinterpret_cast<uchar_ptr *>(prev);
            }
        }
    };

    std::unique_ptr<uchar, Deleter> _currentChunk;
    size_t _offset;
    size_t _size;

  public:
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;

    template <typename U>
    struct rebind {
        using other = ChunkAllocator<U, Size>;
    };

    ChunkAllocator() : _currentChunk{nullptr}, _offset{0}, _size{Size} {}

    pointer allocate(size_type n) {
        if (_currentChunk == nullptr)
            addChunk(_size);
        if ((_size * sizeof(T) + firstElement - _offset) < (n * sizeof(T))) {
            addChunk(_size * 2);
            _size *= 2;
        }

        void *ptr = _currentChunk.get() + _offset;
        _offset += n * sizeof(T);

        return static_cast<pointer>(ptr);
    }

    void deallocate(pointer p, size_t n) {
        if (reinterpret_cast<pointer>(_currentChunk.get() + _offset - n * sizeof(T)) == p)
            _offset -= n * sizeof(T);
    }

  private:
    void addChunk(size_type n) {
        std::unique_ptr<uchar> oldChunk(_currentChunk.release());
        _currentChunk.reset(reinterpret_cast<uchar_ptr>(::operator new(firstElement + sizeof(T) * n)));
        new (_currentChunk.get()) uchar_ptr{oldChunk.release()};
        _offset = firstElement;
    }
};

#endif // CHUNK_ALLOCATOR_H