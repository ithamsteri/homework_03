//
// File: main.cpp
//

#include "Boxer.h"
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
using chunk_map = std::map<int, int, std::less<int>, chunk_allocator>;
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

  // Заполнение своего контейнера числами от 0 до 9
  Boxer<int> boxer;
  for (unsigned i = 0; i < 10; ++i)
    boxer.push_front(i);

  // Заполнение своего контейнера со своим аллокатором от 0 до 9
  ChunkAllocator<int> boxer_allocator(10);
  Boxer<int, ChunkAllocator<int>> chunk_boxer(boxer_allocator);
  for (unsigned i = 0; i < 10; ++i)
    chunk_boxer.push_front(i);

  // Вывод значений из своего контейнера со своим аллокатором.
  for (const auto n : chunk_boxer) {
    std::cout << n << std::endl;
  };
  return 0;
}
