
#include "../libmatch/wildcard.hpp"

#include <cassert>
#include <numeric>
#include <vector>
#include <iostream>

using namespace libmatch;

// Matches 0b00**. That is, this should match the following:
//
//    0000 0001 0010 0011
//
// and nothing else.
void
test1()
{
  wildcard_pattern<int> p {
    0b0000, // value
    0b0011  // mask (wildcards)
  };

  // We match only 0, 1, 2 and 3.
  assert(wildcard_match(0, p));
  assert(wildcard_match(1, p));
  assert(wildcard_match(2, p));
  assert(wildcard_match(3, p));
  
  // And nothing else...
  assert(!wildcard_match(4, p));
  assert(!wildcard_match(5, p));
  assert(!wildcard_match(6, p));
  assert(!wildcard_match(7, p));
  assert(!wildcard_match(8, p));
}


// The pattern 0*0* matches
//
//  0000 0001 0100 0101
void 
test2()
{
  wildcard_pattern<int> p { 0b0000, 0b0101 };

  // Build a vector of the first 16 values.
  std::vector<int> v(16);
  std::iota(v.begin(), v.end(), 0);

  auto i1 = wildcard_find(v.begin(), v.end(), p);
  assert(*i1 == 0);

  auto i2 = wildcard_find(i1 + 1, v.end(), p);
  assert(*i2 == 1);

  auto i3 = wildcard_find(i2 + 1, v.end(), p);
  assert(*i3 == 4);

  auto i4 = wildcard_find(i3 + 1, v.end(), p);
  assert(*i4 == 5);

  auto i5 = wildcard_find(i4 + 1, v.end(), p);
  assert(i5 == v.end());
}


void
test3()
{
  wildcard_pattern<int> p1 { 0, 1 }; // 0b000*
  wildcard_pattern<int> p2 { 0, 3 }; // 0b00** 

  assert(p1 == p1);
  assert(p1 != p2);

  assert(wildcard_compare(p1, p2) == -1);
  assert(wildcard_compare(p2, p1) == 1);
  assert(wildcard_compare(p1, p1) == 0);
}


void
test_table_1()
{
  flat_wildcard_table<int, int> t;
  t.insert({0x00, 0xf0}, 1);  // ****0000
  t.insert({0x00, 0x0f}, 2);  // 0000****
  t.insert({0x00, 0x07}, 3);  // 00000***
  t.insert({0x00, 0x03}, 4);  // 000000**

  auto i1 = t.find(0x10);     // 00010000
  assert(i1->second == 1);

  auto i2 = t.find(0x01);     // 00000001
  assert(i2->second == 2);
  
  auto i3 = t.find(i2 + 1, 0x01);
  assert(i3->second == 3);
  
  auto i4 = t.find(i3 + 1, 0x01);
  assert(i4->second == 4);

  assert(wildcard_compare(i2->first, i3->first) == 1);
  assert(wildcard_compare(i2->first, i4->first) == 1);
  assert(wildcard_compare(i3->first, i4->first) == 1);
}



int
main()
{
  test1();
  test2();
  test3();


  test_table_1();
}
