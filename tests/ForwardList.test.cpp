//
// File: ForwardList.test.cpp
//

#define BOOST_TEST_MODULE ForwardListTest
#include <boost/test/unit_test.hpp>

#include "ForwardList.h"
#include <map>
#include <vector>

template <typename T>
struct TestAllocator {
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;

    template <typename U>
    struct rebind {
        using other = TestAllocator<U>;
    };

    TestAllocator() = default;
    ~TestAllocator() {
        // Check Memory Leak
        BOOST_REQUIRE_EQUAL(mMemMap.empty(), true);
    }

    pointer allocate(size_type n) {
        size_type size = n * sizeof(value_type);

        void *ptr = ::operator new(size);
        if (ptr == nullptr)
            throw std::bad_alloc();
        mMemMap.emplace(ptr, size);

        return static_cast<pointer>(ptr);
    }

    void deallocate(pointer p, size_type size) {
        BOOST_REQUIRE_EQUAL(mMemMap.count(p), 1);
        BOOST_CHECK_EQUAL(mMemMap[p], size * sizeof(value_type));
        BOOST_CHECK_EQUAL(mMemMap.erase(p), 1);
        ::operator delete(p);
    }

  private:
    std::map<void *, size_type> mMemMap;
};

BOOST_AUTO_TEST_CASE(EmptyList) {
    ForwardList<int, TestAllocator<int>> forwardList;

    BOOST_CHECK_EQUAL(forwardList.empty(), true);
    BOOST_CHECK_EQUAL(forwardList.size(), 0);
}

BOOST_AUTO_TEST_CASE(AddElements) {
    ForwardList<int, TestAllocator<int>> forwardList;
    int lvalue = 21;

    forwardList.emplace_front(42);
    forwardList.push_front(lvalue);
    forwardList.push_front(51);

    BOOST_CHECK_EQUAL(forwardList.empty(), false);
    BOOST_CHECK_EQUAL(forwardList.size(), 3);
}

BOOST_AUTO_TEST_CASE(IteratorCheck) {
    ForwardList<std::string, TestAllocator<std::string>> forwardList;
    std::vector<std::string> strings{"Hello", "sick", "and", "crazy", "World"};

    for (const auto &str : strings)
        forwardList.push_front(str);

    BOOST_CHECK_EQUAL_COLLECTIONS(strings.cbegin(), strings.cend(), forwardList.begin(), forwardList.end());
}