//
// File: allocator.test.cpp
//

#define BOOST_TEST_MODULE AllocatorTest
#include <boost/test/unit_test.hpp>

#include "ChunkPool.h"
#include <stdexcept>

// Аллокатор должен выдавать указатели на области памяти, где можно
// в будущем сконструировать объект. В случае ошибки выделения памяти
// может бросать исключение std::bad_alloc.
BOOST_AUTO_TEST_CASE(Allocate) { BOOST_CHECK(true); }
