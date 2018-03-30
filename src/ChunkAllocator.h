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
  using uchar = unsigned char;
  struct NavigationBlock {
    std::unique_ptr<uchar> prev;
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

  template <typename U>
  ChunkAllocator(const ChunkAllocator<U, Size> &rhs) : _currentChunk{nullptr} {}

  ~ChunkAllocator() {
    while (_currentChunk != nullptr) {
      std::unique_ptr<uchar> nextChunk(
          std::move(reinterpret_cast<block_type *>(_currentChunk.get())->prev));
      _currentChunk.release();
      _currentChunk = std::move(nextChunk);
    }
  }

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
    if (reinterpret_cast<pointer>(_currentChunk.get() + _offset -
                                  n * sizeof(T)) == p)
      _offset -= n * sizeof(T);
  }

private:
  std::unique_ptr<uchar> _currentChunk;
  size_t _offset;
  size_t _size;

  void addChunk(size_type n) {
    std::unique_ptr<uchar> oldChunk(_currentChunk.get());
    _currentChunk.reset(
        static_cast<uchar *>(::operator new(firstElement + sizeof(T) * n)));
    new (_currentChunk.get()) block_type{std::move(oldChunk)};
    _offset = firstElement;
  }
};
