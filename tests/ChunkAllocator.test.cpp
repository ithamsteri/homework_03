//
// File: ChunkAllocator.test.cpp
//

#define BOOST_TEST_MODULE AllocatorTest
#include <boost/test/unit_test.hpp>

#include "ChunkAllocator.h"

// Аллокатор должен выдавать указатели на области памяти, где можно
// в будущем сконструировать объект. В случае ошибки выделения памяти
// может бросать исключение std::bad_alloc.
BOOST_AUTO_TEST_CASE(Allocate) {
  ChunkAllocator<int, 1> allocator;

  auto ptr = allocator.allocate(1);
  BOOST_CHECK(ptr != nullptr);

  allocator.deallocate(ptr, 1);
}

// Аллокатор диннамически выделяет память при исчерпании памяти в
// выделенном chunk'е памяти, выделяя новый chunk памяти.
BOOST_AUTO_TEST_CASE(AllocateDynamic) {
  ChunkAllocator<int, 2> allocator;

  auto ptr = allocator.allocate(10);
  BOOST_CHECK(ptr != nullptr);

  allocator.deallocate(ptr, 10);
}
