//
// File: ChunkAllocator.h
//

#pragma once

#include <forward_list>
#include <iostream>
#include <memory>

class ChunkMemoryManager {
  using pointer = unsigned char *;

  std::forward_list<pointer> _chunks; // список выделенных chunk'ов памяти
  pointer _currentChunk{nullptr}; // текущий chunk памяти
  size_t _offset; // смещение от начала текущего chunk'а
  size_t _size;   // размер текущего chunk'а

public:
  // Конструктор принимает значение размера в байтах!
  ChunkMemoryManager(size_t n) : _size{n} {}
  ~ChunkMemoryManager() {
    for (pointer ptr : _chunks) {
      free(ptr);
    }
  }

  // Выделение в chunk'е n байт памяти
  void *do_allocate(size_t n) {
    // Первое выделение памяти.
    if (_currentChunk == nullptr) {
      addChunk(_size);
    }
    // Проверяем, что хватит ли памяти в chunk для нового объекта.
    if (_offset + n > _size)
      addChunk(_size * 2);

    auto result = _currentChunk + _offset;
    _offset += n;
    return result;
  }

  // Удаление памяти в текущем chunk'е происходит по типу стека.
  void do_deallocate(void *ptr, size_t size) {
    auto ptr_back = _currentChunk + _offset - size;
    if (ptr == ptr_back) {
      _offset -= size;
    }
  }

  size_t size() const { return _size; }

private:
  // Добавление нового chunk'а памяти.
  void addChunk(size_t n) {
    pointer ptr = static_cast<pointer>(malloc(n));
    if (ptr == nullptr)
      throw std::bad_alloc();

    _currentChunk = ptr;
    _offset = 0;
    _size = n;
    _chunks.emplace_front(ptr);
  }
};

template <class T> class ChunkAllocator {
  std::shared_ptr<ChunkMemoryManager> _allocator;

public:
  using value_type = T;
  using pointer = T *;

  ChunkAllocator(size_t n)
      : _allocator{std::make_shared<ChunkMemoryManager>(n * sizeof(T))} {}

  // У каждого аллокатора будет свой менеджер памяти
  template <typename U> friend class ChunkAllocator;
  template <class U>
  explicit ChunkAllocator(const ChunkAllocator<U> &rhs) noexcept {
    // Если совпадают размеры типов, то можно пользоваться одним менеджером
    // памяти, т.к. будет правильный размер выделенной памяти при первом
    // запросе.
    if (sizeof(T) == sizeof(U))
      _allocator = rhs._allocator;
    else
      _allocator = std::make_shared<ChunkMemoryManager>(rhs._allocator->size() /
                                                        sizeof(U) * sizeof(T));
  }

  pointer allocate(size_t n) {
    return static_cast<pointer>(_allocator->do_allocate(n * sizeof(T)));
  }
  void deallocate(pointer ptr, size_t size) {
    _allocator->do_deallocate(ptr, size * sizeof(T));
  }
};
