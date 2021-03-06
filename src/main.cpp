//
// File: main.cpp
//

// Overload ::operator new and ::operator delete
#include "Malloc.h"

#include "ChunkAllocator.h"
#include "ForwardList.h"
#include <algorithm>
#include <iostream>
#include <map>

const static int container_size = 10;

template <typename T>
constexpr T factorial(T n) {
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

  std::for_each(cmap.begin(), cmap.end(), [](const auto &p) {
    std::cout << p.first << ' ' << p.second << std::endl;
  });

  // Section 02: ForwardList and allocators
  ForwardList<int> slist;
  for (int i = 0; i < container_size; ++i)
    slist.push_front(i);

  ForwardList<int, ChunkAllocator<int, container_size>> flist;
  for (int i = 0; i < container_size; ++i)
    flist.push_front(i);

  std::for_each(flist.begin(), flist.end(),
                [](const auto &n) { std::cout << n << std::endl; });

  return 0;
}
