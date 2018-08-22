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

#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstddef>
#include <iostream>
#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class ForwardList {
  private:
    class Deleter;

    struct Node {
        T value;
        std::unique_ptr<Node, Deleter> next;
    };

  public:
    class iterator {
      public:
        using self_type = iterator;
        using value_type = T;
        using reference = T &;
        using pointer = T *;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        iterator(Node *ptr) : ptr_(ptr) {}

        self_type &operator++() {
            ptr_ = ptr_->next.get();
            return *this;
        }

        self_type operator++(int) {
            self_type old_iter = *this;
            ++(*this);
            return old_iter;
        }

        reference operator*() { return ptr_->value; }
        pointer operator->() { return &(ptr_->value); }
        bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type &rhs) { return !(*this == rhs); }

      private:
        Node *ptr_;
    };

    using value_type = T;
    using reference = T &;
    using size_type = std::size_t;

    using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using traits = std::allocator_traits<allocator_type>;

    ForwardList() = default;
    ForwardList(const ForwardList &) = delete;
    ForwardList &operator=(const ForwardList &) = delete;

    size_type size() const noexcept { return mSize; }
    bool empty() const noexcept { return mSize == 0; }

    template <typename... Args>
    void emplace_front(Args &&... args) {
        makeNewNode(args...);
    }

    void push_front(const value_type &value) { makeNewNode(value); }
    void push_front(value_type &&value) { makeNewNode(std::move(value)); }

    iterator begin() { return iterator{mTail.get()}; }
    iterator end() { return iterator{nullptr}; }

  private:
    class Deleter {
        allocator_type *mAlloc;

      public:
        Deleter(allocator_type *allocator) : mAlloc{allocator} {}
        void operator()(Node *ptr) {
            traits::destroy(*mAlloc, &(ptr->value));
            traits::destroy(*mAlloc, &(ptr->next));
            traits::deallocate(*mAlloc, ptr, 1);
        }
    };

    class NopDeleter {
      public:
        NopDeleter() = default;
        void operator()(Node *) {}
    };

    template <typename... Args>
    void makeNewNode(Args &&... args) {
        std::unique_ptr<Node> node{traits::allocate(mAllocator, 1)};
        traits::construct(mAllocator, &(node.get()->value), std::forward<Args>(args)...);
        traits::construct(mAllocator, &(node.get()->next), nullptr, &mAllocator);

        if (mSize == 0) {
            mTail.reset(node.get());
        } else {
            mHead->next.reset(node.get());
            mHead.release();
        }
        mHead.reset(node.release());

        mSize++;
    }

    allocator_type mAllocator{};
    std::size_t mSize{0};
    std::unique_ptr<Node, Deleter> mTail{nullptr, &mAllocator};
    std::unique_ptr<Node, NopDeleter> mHead{nullptr};
};

#endif // FORWARD_LIST_H