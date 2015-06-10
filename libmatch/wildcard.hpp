// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#ifndef LIBMATCH_WILDCARD_HPP
#define LIBMATCH_WILDCARD_HPP

// The wildcard module defines facilities for wildcard matching
// on integral keys.

#include <vector>
#include <boost/optional.hpp>


namespace libmatch
{


// The pattern class defines a pattern of a value type of T.
// A pattern is comprised of two values. A value with specified
// elements, and a value denoting the wildcard matches.
// 
// Here, T is assumed be an integral type or bitstring of
// some type. However, this could be easily generalized to
// define patterns in any string.
//
// Wild cards are partially ordered by their value and their
// masks. A pattern p1 is less than a pattern p2 if their values
// are equal and the mask of p1 is a proper subset of p2s.
template<typename T>
struct wildcard_pattern
{
  wildcard_pattern(T const& v, T const& m)
    : value(v), mask(m)
  { }

  T value;
  T mask;
};


// Two wildcard patterns are equal when they match the
// same values. This is the case when the values and
// masks are equal.
template<typename T>
inline bool
operator==(wildcard_pattern<T> const& a, wildcard_pattern<T> const& b)
{
  return a.value == b.value && a.mask == b.mask;
}


template<typename T>
inline bool
operator!=(wildcard_pattern<T> const& a, wildcard_pattern<T> const& b)
{
  return !(a == b);
}


// Returns true if and only if a is a subset of b.
//
// TODO: Guarantee that the T is bitstring compatible.
template<typename T>
inline bool
is_subset(T const& a, T const& b)
{
  return (a | b) == b;
}


// A 3-way comparison of two wildcard patterns. This returns 
// -1 if `a` is less than `b`, 1 if `a` is greater than `b`
// and `0` otherwise. The intuition is that a wildcard is
// "less" if it is a stronger match than another one: it matches
// more bits exactly.
template<typename T>
int
wildcard_compare(wildcard_pattern<T> const& a, wildcard_pattern<T> const& b)
{
  int result = 0;
  if (a.value == b.value) {
    if (is_subset(a.mask, b.mask))
      --result;
    if (is_subset(b.mask, a.mask))
      ++result;
  }
  return result;
}


// Returns true if and only if `k` is matched by the wildard 
// pattern `p`.
template<typename T>
inline bool 
wildcard_match(T const& k, wildcard_pattern<T> const& p)
{
  return p.value == (k & ~p.mask);
}


// Returns the first iterator i in the range [first, last) that
// where wildcard_match(*i, p) is true. 
template<typename I, typename T>
inline I
wildcard_find(I first, I last, wildcard_pattern<T> const& p)
{
  while (first != last && !wildcard_match(*first, p))
    ++first;
  return first;
}


// A table of wildcard patterns. A key K is in the table if
// any pattenr matches K. Note there may be multiple patterns
// matching K.
template<typename K, typename V>
class flat_wildcard_table
{
public:
  using value_type = std::pair<wildcard_pattern<K>, V>;
  using key_type = K;
  using mapped_type = V;
private:
  using storage_type = std::vector<value_type>;
public:
  using iterator       = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  
  // Obsrevers
  bool empty() const { return store_.empty(); }
  int  size() const  { return store_.size(); }

  // Lookup  
  const_iterator find(K const&) const;
  const_iterator find(const_iterator, K const&) const;

  // Mutators
  iterator insert(wildcard_pattern<K> const&, V const&);
  iterator erase(iterator);

  // Iterators
  const_iterator begin() const { return store_.begin(); }
  const_iterator end() const   { return store_.end(); }

private:
  storage_type store_;
};


// Insert the wildcard pattern into the table.
//
// FIXME: Should we try to guarantee uniqueness of the
// wildcard pattern?
template<typename K, typename V>
inline auto
flat_wildcard_table<K, V>::insert(wildcard_pattern<K> const& k, V const& v) -> iterator
{
  store_.push_back({k, v});
}


// Returns an iterator to the first pattern matching k.
template<typename K, typename V>
inline auto
flat_wildcard_table<K, V>::find(K const& k) const -> const_iterator
{
  return find(begin(), k);
}


// Returns an iterator in the range [i, end()) to the first 
// pattern matching k.
template<typename K, typename V>
inline auto
flat_wildcard_table<K, V>::find(const_iterator i, K const& k) const -> const_iterator
{
  while (i != end() && !wildcard_match(k, i->first))
    ++i;
  return i;
}



} // namespace libmatch



#endif