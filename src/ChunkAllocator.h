//
// File: ChunkPool.h
// Desc: Chunk memory pool with allocator
//

#pragma once

#include <cstddef>
#include <forward_list>
#include <iostream>
#include <memory>

template <typename T, size_t Size> struct ChunkAllocator {
private:
  using uchar_ptr = unsigned char *;
  struct NavigationBlock {
    uchar_ptr prev;
  };
  using block_type = NavigationBlock;
  constexpr static size_t firstElement =
      (sizeof(block_type) + alignof(T) - 1) / alignof(T) * alignof(T);

public:
  using value_type = T;
  using pointer = T *;
  using size_type = size_t;

  template <typename U> struct rebind {
    using other = ChunkAllocator<U, Size>;
  };

  ChunkAllocator() : _currentChunk{nullptr}, _offset{0}, _size{Size} {}
  ~ChunkAllocator() {
    while (_currentChunk != nullptr) {
      uchar_ptr nextChunk = ((block_type *)_currentChunk)->prev;
      free(_currentChunk);
      _currentChunk = nextChunk;
    }
  }

  pointer allocate(size_type n) {
    if (_currentChunk == nullptr)
      addChunk(_size);
    if ((_size * sizeof(T) + firstElement - _offset) < (n * sizeof(T))) {
      addChunk(_size * 2);
      _size *= 2;
    }

    void *ptr = _currentChunk + _offset;
    _offset += n * sizeof(T);

    return static_cast<pointer>(ptr);
  }

  void deallocate(pointer p, size_t n) {
    if (reinterpret_cast<pointer>(_currentChunk + _offset - n * sizeof(T)) == p)
      _offset -= n * sizeof(T);
  }

private:
  uchar_ptr _currentChunk;
  size_t _offset;
  size_t _size;

  void addChunk(size_type n) {
    auto oldChunk = _currentChunk;
    _currentChunk =
        static_cast<uchar_ptr>(malloc(firstElement + sizeof(T) * n));
    new (_currentChunk) block_type{oldChunk};
    _offset = firstElement;
  }
};
