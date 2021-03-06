// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#ifndef LIBMATCH_HASH_HPP
#define LIBMATCH_HASH_HPP

// The hashing module. Encompasses a collection of hashing
// algorithms and data structures, as well as the algorithms
// needed to implement the structures.
//
// Inside the hashing library there are two major distinctions;
// Closed addressing vs Open addressing. 
// 
// The closed addressing namespace houses the more basic chained
// hash table variants (simple, list, array, tree) which utilize
// a back end store to resolve collisions. That is, each hash
// table bucket/slot can contain multiple entries.
//
// The open addressing namespace contains tables which implement
// a flat addressing scheme and resolve collisions by using
// different resolution algorithms to find an open address. 
// In this setup each hash table bucket/slot contains at most 
// one entry.
//
// We have chosen to use Boosts optional component to allow for
// in place construction/destruction of elements. With the expected
// release of a std::optional object this should become a more
// common place method in the future.

#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/optional.hpp>

namespace hash_table
{

	
// A list of primes numbers such that each value is slightly
// less than double the previous value.
static size_t const primes[] = {
          53,        97,       193,       379,       757,
        1511,      3019,      6037,     12073,     24137, 
       48271,     96527,    193051,    386093,    772181, 
     1544357,   3088711,   6177419,  12354821,  24709621,
    49419241,  98838479, 197676953, 395353901, 790707787,
  1581415553,
};
static size_t const nprimes = sizeof(primes) / sizeof(size_t);


// Performs a binary search on the list of prime numbers to find a new
// value that is slightly less than twice the value given (old).
static size_t const
next_prime(size_t old)
{
  // Our starting compare value will be twice the old value.
  old <<= 1;

  // Start in the middle of the primes list.
  size_t pivot = nprimes >> 1;

  while (1) {
    // Check if we have gone outside the list bounds.
    if (pivot + 1 >= nprimes)
      return 0;

    // If the pivot value is less than our compare value and the next
    // largest value is greater than our compare value, we have found
    // our next size.
    if (primes[pivot] < old && primes[pivot + 1] > old)
      break;

    // If the pivot value is greater than our compare value, look in 
    // the lower half. Otherwise look in the upper half.
    if (primes[pivot] > old)
      pivot -= pivot >> 1;
    else
      pivot += pivot >> 1;
  }

  return primes[pivot];
}


// The data store namespace contains the underlying backend stores
// for the hash table variants. They are implemented using Boosts
// optional object to allow for in place construction/destruction.
namespace data_store
{


// A basic entry type comprised of a key and a value.
template<typename K, typename V>
struct entry
{
  entry()
    : entry(K(), V())
  { }
  
  entry(K const& k, V const& v)
    : key(k), val(v)
  { }

  K key;
  V val;
};


// A linked list entry.
template<typename K, typename V>
struct list_entry : entry<K, V>
{
  list_entry()
    : entry<K, V>(), next(nullptr)
  { }

  list_entry(K const& k, V const& v)
    : entry<K, V>(k, v), next(nullptr)
  { }

  ~list_entry()
  {
    if (nullptr != next)
      delete next;
  }

  list_entry<K, V>* next;
};


// A tree node entry.
template<typename K, typename V>
struct tree_entry : entry<K, V>
{
  tree_entry()
    : entry<K, V>(), left(nullptr), right(nullptr)
  { }

  tree_entry(K const& k, V const& v)
    : entry<K, V>(k, v), left(nullptr), right(nullptr)
  { }

  tree_entry<K, V>* left;
  tree_entry<K, V>* right;
};


// A basic hash table bucket containing a single entry. This
// class is similar to a standard container.
//
// TODO: Define iterators so that this works like a regular
// container.
template<typename T>
class basic_bucket
{ 
public:
  // Constructors
  basic_bucket();
  basic_bucket(T const&);
  basic_bucket(T&);
  T& operator=(T&);


  // Equality comparators
  inline bool operator==(basic_bucket&) const;
  inline bool operator!=(basic_bucket&) const;

  // Accessors
  bool is_full() const;
  bool is_empty() const;
  
  T&       get();
  T const& get() const;


  // Mutators
  void insert(T const&);
  void insert(T&&);

  template<typename... Args> 
  void emplace(Args&&...);
  
  void erase(T const&);
  void clear();

private:
  // Data members
  boost::optional<T> data_;
};

// Basic bucket.
//
// Basic bucket default constructor.
template<typename T>
basic_bucket<T>::basic_bucket()
{ }


// Basic bucket copy constructor.
template<typename T>
basic_bucket<T>::basic_bucket(T const& v)
  : data_(v)
{ }


// Basic bucket move constructor.
template<typename T>
basic_bucket<T>::basic_bucket(T& v)
  : data_(std::move(v))
{ }


// Move assignment operator.
template<typename T>
T&
basic_bucket<T>::operator=(T& v)
{ 
  data_(std::move(v));
}


// Basic bucket equality comparison
template<typename T>
inline bool
basic_bucket<T>::operator==(basic_bucket<T>& other) const
{
  return (*data_ == *other.data_);
}


// Basic bucket inequality comparison
template<typename T>
inline bool
basic_bucket<T>::operator!=(basic_bucket<T>& other) const
{
  return !(this==other);
}


// Retrieves the current value from the bucket. The bucket
// shall not be full.
template<typename T>
inline T&
basic_bucket<T>::get()
{
  assert(!is_empty());
  return *data_;
}


// Retrieves the current value from the bucket. The bucket
// shall not be full.
template<typename T>
inline T const&
basic_bucket<T>::get() const
{
  assert(!is_empty());
  return *data_;
}


// Returns true if the bucket contains a value.
template<typename T>
inline bool 
basic_bucket<T>::is_full() const
{ 
  return (bool)data_; 
}


// Returns true if the bucket does not contain a value.
template<typename T>
inline bool 
basic_bucket<T>::is_empty() const
{ 
  return !(bool)data_; 
}


// Copies `t` to the bucket. The bucket shall not be full.
template<typename T>
inline void 
basic_bucket<T>::insert(T const& t)
{
  assert(is_empty());
  data_ = t;
}


// Moves `t` into the bucket. The bucket shall not be full.
template<typename T>
inline void 
basic_bucket<T>::insert(T&& t)
{
  assert(is_empty());
  data_ = std::move(t);
}


// Emplace a value with the given `args` into the bucket.
// The bucket shall not be full.
template<typename T>
template<typename... Args>
inline void
basic_bucket<T>::emplace(Args&&... args)
{
  data_.emplace(std::forward<Args>(args)...);
}


// Erase the value `t` from the bucket. If `t` is not in
// the bucket, no operation is performed.
template<typename T>
inline void 
basic_bucket<T>::erase(T const& t)
{
  if (data_ == t)
    data_ = boost::none;
}


// Clears the bucket of its current value.
template<typename T>
inline void 
basic_bucket<T>::clear()
{
  data_ = boost::none;
}


// A chained hash table bucket containing chained entries.
template<typename T>
class list_bucket 
{
public:
  // Constructors/Destructor
  list_bucket();
  list_bucket(T const&);
  list_bucket(T&);
  ~list_bucket();

  // Accessors
  bool is_empty() const;
  
  T&       get();
  T const& get() const;


  // Mutators
  void insert(T const&);
  void insert(T&&);

  template<typename... Args> 
  void emplace(Args&&...);
  
  void erase(T const&);
  void clear();

private:
  // Data Members
  boost::optional<T> data_;
};


// List bucket.
//
// Default list_bucket constructor.
template<typename T>
list_bucket<T>::list_bucket()
  : data_()
{ }


// Copy constructor for list_bucket.
template<typename T>
list_bucket<T>::list_bucket(T const& v)
  : data_(v)
{ }


// Move constructor for list_bucket.
template<typename T>
list_bucket<T>::list_bucket(T& v)
  : data_(std::move(v))
{ }


// Destructor for list_bucket.
//
// TODO: Implement this. Will most likely need to
// walk the list(s) and delete all the entries.
template<typename T>
list_bucket<T>::~list_bucket()
{ 

}


// Checks if the bucket is empty.
//
// TODO: Implement this.
template<typename T>
inline bool
list_bucket<T>::is_empty() const
{

}


// Accessor.
//
// TODO: Implement this.
template<typename T>
inline T&
list_bucket<T>::get()
{

}


// Const accessor.
//
// TODO: Implement this.
template<typename T>
inline T const&
list_bucket<T>::get() const
{

}


// Add entry by const reference.
//
// TODO: Implement this.
template<typename T>
inline void 
list_bucket<T>::insert(T const&)
{

}


// Add entry using move.
//
// TODO: Implement this.
template<typename T>
inline void 
list_bucket<T>::insert(T&&)
{

}


// Remove the entry given if it exists.
//
// TODO: Implement this.
template<typename T>
inline void 
list_bucket<T>::erase(T const& v)
{

}


// Clear the bucket of all entries.
//
// TODO: Implement this.
template<typename T>
inline void 
list_bucket<T>::clear()
{

}


// An array based hash table bucket.
//
// TODO: Implement this.
template<typename T>
class array_bucket
{
public:
  // Constructors/Destructor
  array_bucket();
  array_bucket(int);
  ~array_bucket();

  
  // Accessors
  bool is_full() const;
  bool is_empty() const;
  
  T&       get();
  T const& get() const;


  // Mutators
  void insert(T const&);
  void insert(T&&);

  template<typename... Args> 
  void emplace(Args&&...);
  
  void erase(T const&);
  void clear();

private:
  // Data Members
  boost::optional<T>* data_;  // Data store
  int size;                   // Current size
  int capacity;               // Maximum size
};


// A tree based hash table bucket.
//
// TODO: Implement this. Use an array, not pointers.
template<typename T>
class tree_bucket
{ 
public:
  //Constructors/Destructor
  tree_bucket();
  tree_bucket(T const& v);
  ~tree_bucket();


  // Accessors
  bool is_empty() const;
  
  T&       get();
  T const& get() const;


  // Mutators
  void insert(T const&);
  void insert(T&&);

  template<typename... Args> 
  void emplace(Args&&...);
  
  void erase(T const&);
  void clear();

private:
  // Data Members
  boost::optional<T> data_;
};


} // end namespace data_store


// Encapsulates the open addressing hash table implementations.
namespace open
{

// Linear hash table. A hash table with open addressing and linear
// probing for resolving collisions.
template<typename K, typename V, typename H, typename C>
class linear
{
public:
  using store_type = std::vector<data_store::basic_bucket<data_store::entry<K, V>>>;
  using value_type = data_store::basic_bucket<data_store::entry<K, V>>;
  using hasher = H;
  using compare = C;  

  class iterator
  {
    using difference_type = std::ptrdiff_t;
    using value_type = linear::value_type;
    using reference = linear::value_type&;
    using pointer = linear::value_type*;
    using iterator_category = std::random_access_iterator_tag;
  public:
    // Constructor/Destructor
    iterator();
    iterator(value_type&);
    iterator(value_type const&);
    
    ~iterator();

    // Assignment and Relational operators.
    iterator& operator=(iterator const&);
    bool operator==(iterator const&) const;
    bool operator!=(iterator const&);

    // Prefix increment
    iterator& operator++();
    // Postfix increment
    iterator& operator++(int);

    // Dereferencing operations
    reference operator*();
    pointer operator->();

  private:
    value_type* data_;
  };

  linear();
  linear(int);
  linear(int, hasher, compare); 

  // Observers
  int size() const { return size_; }
  int buckets() const { return buckets_; }
  double load() const { return (double)size_ / (double)buckets_; }
  bool empty() const { return size_ == 0; }

  // Lookup
  value_type const* find(K const&) const;
  iterator begin();
  iterator end();

  // Mutators
  value_type* insert(K const&, V const&);
  void erase(K const&);

private:
  int get_hash_index(K const&) const;
  int get_entry_index(K const&) const;
  
  // Resizes the hash table when the load reaches a particular value.
  void resize();

private:
  int        size_;    // Current number of entries
  int        buckets_; // Number of buckets
  hasher     hash_;    // Hash function
  compare    cmp_;     // Equality comparison
  store_type data_;    // Data store
};

// Linear hash table.
//
// Linear constructor definitions.
template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::linear()
  : linear(29)
{ }


template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::linear(int n)
  : linear(n, hasher(), compare())
{ }


template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::linear(int n, hasher h, compare c)
  : size_(0), buckets_(n), hash_(h), cmp_(c), data_(n)
{ }


// Retrieves a key-value pair from the data store.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::find(K const& key) const -> value_type const*
{
  int idx = get_entry_index(key);    
  if (idx < 0)
    return nullptr;
  return &data_[idx];
}


// Returns an iterator pointing before the first bucket that is full.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::begin() -> iterator
{
  for (auto it = data_.begin(); it != data_.end(); ++it)
    if (*it.is_full()) 
      return iterator(--it);
  return iterator();
}


// Returns an iterator pointing past the last bucket that is full.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::end() -> iterator
{
  for (auto it = data_.end(); it != data_.begin(); --it) {
    if (*it.is_full()) {
      return iterator(++it);
    }
  }
  return iterator();
}


// Inserts a new key-value pair into an empty bucket.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::insert(K const& key, V const& value) -> value_type*
{
  ++size;
  // Check if load is at the high water mark and resize if necessary.
  if (load() >= 0.7)
    resize();

  int idx = get_hash_index(key);
  // Look for an open bucket.
  while (data_[idx].is_full())
    idx = (idx + 1 < buckets ? idx + 1 : 0);
  // Add the entry.
  data_[idx].insert(data_store::entry<K, V>(key, value));
  // Return a pointer to the newly inserted entry.
  return &data_[idx];
}


// Clears the bucket containing the same key, if found.
template<typename K, typename V, typename H, typename C> 
void
linear<K, V, H, C>::erase(K const& key)
{
  int idx = get_entry_index(key);
  if (idx >= 0) {
    data_[idx].clear();
    --size;        
  }
}


// Returns the index in the table for the given key or -1 if not found. 
// This is guaranteed to be the actual index in the table for the key.
template<typename K, typename V, typename H, typename C> 
int
linear<K, V, H, C>::get_entry_index(K const& key) const
{
  int idx = get_hash_index(key); 
  while (!cmp_(data_[idx], key))
    idx = (idx + 1 < buckets ? idx + 1 : 0);
  return (cmp_(data_[idx], key) ? idx : -1);
}


// Returns an index in the table for a given key. This is not guaranteed
// to be the actual index for the key.
template<typename K, typename V, typename H, typename C> 
inline int
linear<K, V, H, C>::get_hash_index(K const& key) const
{ 
  return hash_(key) % buckets; 
}


// Linear iterator swap. Uses std::swap to exchange the values pointed
// to by two iterators.
template<class iterator> 
static void
iter_swap(iterator a, iterator b)
{
  std::swap(*a, *b);
}


// Linear iterator default constructor.
template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::iterator::iterator()
  : data_()
{ }


// Linear iterator copy constructor.
template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::iterator::iterator(value_type& data)
  : data_(data)
{ }


// Linear iterator copy constructor.
template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::iterator::iterator(value_type const& data)
  : data_(data)
{ }


// Linear iterator destructor.
template<typename K, typename V, typename H, typename C> 
linear<K, V, H, C>::iterator::~iterator()
{
  delete data_;
  data_ = nullptr;
}


// Assignment operator.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::iterator::operator=(iterator const& other) -> iterator&
{
  *data_ = *other.data_;
  return *this;
}


// Equality operator
template<typename K, typename V, typename H, typename C> 
inline bool 
linear<K, V, H, C>::iterator::operator==(iterator const& other) const
{
  return (*data_ == *other.data_);
}


// Inequality operator
template<typename K, typename V, typename H, typename C> 
inline bool 
linear<K, V, H, C>::iterator::operator!=(iterator const& other)
{
  return !(data_ == other.data_);
}


// Prefix increment.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::iterator::operator++() -> iterator& 
{
  while (++data_ != end() && data_->is_empty())
  { }
  return *this;
}


// Postfix increment.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::iterator::operator++(int) -> iterator&
{
  iterator temp = *this;
  while (++data_ != end() && data_->is_empty())
  { }
  return *temp;
}


// Dereferencing operator. Returns the bucket pointed to by the
// iterator.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::iterator::operator*() -> reference
{
  return data_->get();
}


// Dereferencing operator. Returns a pointer to the bucket that
// the iterator is pointing at.
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::iterator::operator->() -> pointer
{
  return data_;
}


} //end namespace open


// Contains the closed addressing hash table implementations.
//
// TODO: Implement this.
namespace closed
{

} // end namespace closed


} // end namespace hash_table


// A collection of hashing functions.
//
// TODO: Implement this.
namespace hash_function
{

} // end namespace hash_function

#endif
