//
// File: ForwardList.h
// Desc: Simple version of Forward List
//
//
// C++ concepts: SequenceContainer
// A SequenceContainer is a Container that stores objects of the same type
// in a linear arrangement.
//
// http://en.cppreference.com/w/cpp/concept/SequenceContainer
//
// NOTE: Реализация SequenceContainer не выполнена полностью в угоду простоты.
//

#pragma once

#include <iterator>
#include <limits>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class ForwardList {
  struct Node {
    T value;
    Node *next;
  };

 public:
  class Iterator;
  class ConstIterator;

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using difference_type =
      typename std::iterator_traits<iterator>::difference_type;
  using size_type = std::size_t;

 private:
  // Используем аллокатор не для типа T, а для типа узла Node.
  using allocator_type =
      typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  // Используем allocator_traits как посредника при работе с аллокатором,
  // т.к. он помогает реализовывать стандартные методы и типы, которых
  // может не быть в указанном аллокаторе.
  using traits = std::allocator_traits<allocator_type>;

  // Каждый список создаёт копию объекта аллокатора.
  allocator_type _allocator;
  // Список хранит две ссылки на "голову" и "хвост" списка.
  // Чтение списка происходит с "хвоста", а "голова" нужна для
  // добовления нового узла в списке.
  Node *_head{nullptr};  // "голова"
  Node *_tail{nullptr};  // "хвост"
  size_type _size{0};    // количество узлов в списке

 public:
  ForwardList() : _allocator(allocator_type{}) {}

  // Процесс уничтожения списка происходит с "хвоста". Постепенно проходя
  // каждый узел, вызывается деструктор значения и освобождение памяти этого
  // узла.
  ~ForwardList() {
    Node *node = _tail;
    while (node != nullptr) {
      Node *next_node = node->next;
      traits::destroy(_allocator, &(node->value));
      traits::deallocate(_allocator, node, 1);
      node = next_node;
    }
  }

  size_type size() const noexcept { return _size; }
  bool empty() const noexcept { return _size == 0; }

  iterator begin() { return Iterator{_tail}; }
  iterator end() { return Iterator{nullptr}; }

  void push_front(const T &value) {
    Node *node = traits::allocate(_allocator, 1);
    traits::construct(_allocator, &(node->value), value);
    node->next = nullptr;

    if (_tail == nullptr) _tail = node;
    if (_head != nullptr) _head->next = node;

    _head = node;
    _size++;
  }
};

// Реализация концепта итератора в C++
// http://en.cppreference.com/w/cpp/concept/Iterator
template <typename T, typename Alloc>
class ForwardList<T, Alloc>::Iterator {
  using node_type = ForwardList<T, Alloc>::Node;
  node_type *_node;

 public:
  // Типы для iterator_traits
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using difference_type = ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  // C++ concepts: CopyConstructible
  Iterator(node_type *node) : _node{node} {}
  Iterator(const Iterator &rhs) : _node{rhs._node} {}
  // C++ concepts: CopyAssignable
  Iterator &operator=(const Iterator &rhs) {
    _node = rhs._node;
    return *this;
  }
  // C++ concepts: Destructible
  ~Iterator() = default;

  // Требования InputIterator
  bool operator==(const Iterator &rhs) const { return _node == rhs._node; }
  bool operator!=(const Iterator &rhs) const { return _node != rhs._node; }
  reference operator*() const { return _node->value; }
  pointer operator->() const { return &(_node->value); }

  ForwardList<T, Alloc>::iterator &operator++() {
    _node = _node->next;
    return *this;
  }
  void operator++(int) { *this ++; }
};

template <typename T, typename Alloc>
class ForwardList<T, Alloc>::ConstIterator {
  using node_type = ForwardList<T, Alloc>::Node;
  node_type *_node;

 public:
  using value_type = T;
  using pointer = const T *;
  using reference = const T &;
  using difference_type = ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  ConstIterator(node_type *node) : _node{node} {}
  ConstIterator(const ConstIterator &rhs) : _node{rhs._node} {}
  ConstIterator &operator=(const ConstIterator &rhs) {
    _node = rhs._node;
    return *this;
  }
  ~ConstIterator() = default;

  bool operator==(const ConstIterator &rhs) const { return _node == rhs._node; }
  bool operator!=(const ConstIterator &rhs) const { return _node != rhs._node; }
  reference operator*() const { return _node->value; }
  pointer operator->() const { return &(_node->value); }

  ForwardList<T, Alloc>::iterator &operator++() {
    _node = _node->next;
    return *this;
  }
  void operator++(int) { *this ++; }
};
