//
// File: ChunkPool.h
// Desc: Chunk memory pool with allocator
//

#pragma once

#include <cstddef>
#include <forward_list>
#include <memory>

template <unsigned Size> struct ChunkPool {

  // Принцип работы аллокатора: выделяется область памяти для Size объектов
  // типа T, укзатель которой добавляется в односвязный список. При выделении
  // памяти даётся указатель в область chunk'а, который вычисляется как
  // адрес chunk'а + смещение в байтах. При уничтожении аллокатора
  // освобождается вся выделенная память для каждого chunk'а памяти.
  template <typename T> struct Allocator {
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;

    Allocator() : _offset{0}, _size{Size} { addChunk(Size); }
    ~Allocator() {
      for (const auto chunkPtr : _chunkList)
        free(chunkPtr);
    }

    pointer allocate(size_type n) {
      // Проверяем, что хватит ли памяти для новых n элементов
      if ((_size * sizeof(T) - _offset) < (n * sizeof(T))) {
        addChunk(_size * 2);
        _offset = 0;
        _size *= 2;
      }

      void *ptr = _currentChunk + _offset;
      _offset += n * sizeof(T);

      return static_cast<pointer>(ptr);
    }

    // Всё удаление памяти происходит в деструкторе аллокатора.
    // TODO: Можно реализовать уменьшение смещения внутри chunk'а памяти,
    // если вызывается deallocate полследнего отданного адреса. Тогда эту
    // память можно использовать повторно.
    void deallocate(pointer, size_t) {}

  private:
    using uchar_ptr = unsigned char *;

    std::forward_list<uchar_ptr> _chunkList;
    uchar_ptr _currentChunk;
    size_t _offset; // смещение от начала chunk'а в байтах
    size_t _size; // раземер текущего chunk'а в кол-ве объектов

    // Добавление нового chunk'а памяти, чтобы вместить туда n объектов
    void addChunk(size_type n) {
      _currentChunk = static_cast<uchar_ptr>(malloc(sizeof(T) * n));
      if (_currentChunk == nullptr)
        throw std::bad_alloc();
      _chunkList.push_front(_currentChunk);
    }
  };
};
