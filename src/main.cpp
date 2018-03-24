//
// File: main.cpp
//

#include "ChunkAllocator.h"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

unsigned long fib(unsigned long n) {
  assert(n != 0); // Fibonacci number don't calculate for zero
  unsigned long a = 0, b = 1;
  while (n-- > 1) {
    unsigned long t = a;
    a = b;
    b += t;
  }
  return b;
}

using chunk_allocator = ChunkAllocator<std::pair<const int, int>>;
using chunk_map = std::map<int, int, std::less<>, chunk_allocator>;
using simple_map = std::map<int, int>;

int main(int, char *[]) {
  // Создайте экземпляр std::map и заполните 10 значениями.
  simple_map smap;
  for (unsigned i = 1; i <= 10; ++i) {
    smap[i] = fib(i);
  }

  // Создайте std::map и ограничите его 10 элементами.
  // Заполните значениями.
  chunk_allocator alloc(10);
  chunk_map cmap(alloc);
  for (unsigned i = 1; i <= 10; ++i) {
    cmap[i] = fib(i);
  }

  // Вывод всех значений из контейнера.
  for (unsigned i = 1; i <= 10; ++i) {
    std::cout << i << ' ' << cmap[i] << '\n';
  }

  return 0;
}
