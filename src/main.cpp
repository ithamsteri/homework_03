//
// File: main.cpp
//

// Overload ::operator new and ::operator delete
//#include "Malloc.h"

#include "ChunkAllocator.h"
#include "ForwardList.h"
#include <iostream>
#include <map>

const static int container_size = 10;

constexpr unsigned factorial(unsigned n) {
  return (n == 0) ? 1 : n * factorial(n - 1);
}

using chunk_allocator_10 =
    ChunkAllocator<std::pair<const int, int>, container_size>;
using chunk_map_10 = std::map<int, int, std::less<int>, chunk_allocator_10>;
using simple_map = std::map<int, int>;

int main(int, char *[]) {
  // Section 01: std::map and allocators
  simple_map smap;
  for (int i = 0; i < container_size; ++i)
    smap[i] = factorial(i);

  chunk_map_10 cmap;
  for (int i = 0; i < container_size; ++i)
    cmap[i] = factorial(i);
  for (const auto p : cmap)
    std::cout << p.first << ' ' << p.second << std::endl;

  // Section 02: ForwardList and allocators
  ForwardList<int> slist;
  for (int i = 0; i < container_size; ++i)
    slist.push_front(i);

  ForwardList<int, ChunkAllocator<int, 10>> flist;
  for (int i = 0; i < container_size; ++i)
    flist.push_front(i);

  for (const auto n : flist) {
    std::cout << n << std::endl;
  }
  return 0;
}
