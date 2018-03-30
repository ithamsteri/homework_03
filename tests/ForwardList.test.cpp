//
// File: ForwardList.test.cpp
//

#define BOOST_TEST_MODULE ForwardListTest
#include <boost/test/unit_test.hpp>

#include "ForwardList.h"

BOOST_AUTO_TEST_CASE(EmptyList) {
  ForwardList<int> flist;

  BOOST_CHECK_EQUAL(flist.empty(), true);
  BOOST_CHECK_EQUAL(flist.size(), 0);
}

BOOST_AUTO_TEST_CASE(AddToList) {
  ForwardList<int> flist;

  flist.push_front(1);
  BOOST_CHECK_EQUAL(flist.size(), 1);
  BOOST_CHECK_EQUAL(flist.empty(), false);

  flist.push_front(2);
  BOOST_CHECK_EQUAL(flist.size(), 2);
}

BOOST_AUTO_TEST_CASE(IteratorTest) {
  ForwardList<int> flist;

  for (int i = 0; i < 5; ++i) flist.push_front(i);

  int n = 0;
  for (auto iter = flist.begin(); iter != flist.end(); ++iter)
    BOOST_CHECK_EQUAL(*iter, n++);
}
