//
// File: Boxer.h
// Desc: Boxer is little container <Forward List + Stack>
//

#pragma once

#include <iostream>
#include <iterator>
#include <memory>

template <class T, class Alloc = std::allocator<T>> class Boxer {
  // Internal representation Node of list
  struct Node {
    Node *next;
    T value;
  };

public:
  class iterator {
    Node *_node;

  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using iterator_category = std::forward_iterator_tag;

    iterator(Node *node) : _node{node} {};
    iterator(const iterator &rhs) : _node{rhs._node} {};
    ~iterator() = default;

    iterator &operator=(const iterator &rhs) {
      _node = rhs._node;
      return *this;
    }
    bool operator==(const iterator &rhs) const { return _node == rhs._node; }
    bool operator!=(const iterator &rhs) const { return _node != rhs._node; }

    iterator &operator++() {
      _node = _node->next;
      return *this;
    }
    iterator operator++(int) {
      iterator temp(*this);
      _node = _node->next;
      return temp;
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }
  };

  class const_iterator {
    Node *_node;

  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = const value_type;
    using pointer = const value_type *;
    using iterator_category = std::forward_iterator_tag;

    const_iterator(Node *node) : _node{node} {}
    const_iterator(const const_iterator &rhs) : _node{rhs._node} {}
    const_iterator(const iterator &rhs) : _node{rhs._node} {}
    ~const_iterator() = default;

    const_iterator &operator=(const const_iterator &rhs) {
      _node = rhs._node;
      return *this;
    }
    bool operator==(const const_iterator &rhs) const {
      return _node == rhs._node;
    }
    bool operator!=(const const_iterator &rhs) const {
      return _node != rhs._node;
    }

    const_iterator &operator++() {
      _node = _node->next;
      return *this;
    }

    const_iterator operator++(int) {
      iterator temp(*this);
      _node = _node->next;
      return temp;
    }

    reference operator*() const { return _node->value; }
    pointer operator->() const { return &(_node->value); }
  };

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = iterator;
  using const_iterator = const_iterator;
  using difference_type = typename iterator::difference_type;
  using size_type = std::size_t;

  using allocator_type =
      typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  using traits = std::allocator_traits<allocator_type>;
  using pointer = typename traits::pointer;

  Boxer() = default;
  Boxer(Alloc &a) : _allocator(a) {}
  ~Boxer() {
    for (Node *ptr = _first_node; ptr != nullptr; ptr = ptr->next)
      traits::deallocate(_allocator, ptr, 1);
  }

  // Get iterators
  iterator begin() { return iterator(_first_node); }
  iterator end() { return iterator(nullptr); }
  const_iterator cbegin() { return const_iterator(_first_node); }
  const_iterator cend() { return const_iterator(nullptr); }

  // Add elements to front
  void push_front(const T &value) {
    Node *ptr = traits::allocate(_allocator, 1);
    ptr->next = _first_node;
    ptr->value = value;
    _first_node = ptr;
    _size++;
  }

  // Remove element from front
  void pop_front() {
    auto front = _first_node;
    _first_node = _first_node->next;
    traits::deallocate(_allocator, front, 1);
  }

  size_type size() const noexcept { return _size; }
  bool empty() const noexcept { return _size == 0; }

private:
  allocator_type _allocator;
  Node *_first_node{nullptr};
  size_t _size{0};
};
