//
// File: Malloc.h
// Desc: Overloading ::operator new and ::operator delete
//

#ifndef MALLOC_H
#define MALLOC_H

#include <cstdlib>

void *operator new(std::size_t sz) { return std::malloc(sz); }
void operator delete(void *ptr) noexcept { std::free(ptr); }

#endif // MALLOC_H