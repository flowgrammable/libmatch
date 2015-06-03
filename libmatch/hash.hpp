// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#ifndef LIBMATCH_HASH_HPP
#define LIBMATCH_HASH_HPP

// The hashing module...
//
// TODO: Actually document the hash table.

#include <vector>


namespace hash_table
{

namespace data_store
{

// A simple key-value pair
template<typename K, typename V>
struct entry
{
  entry()
    : key(K()),val(V())
  { }
  
  entry(K const& k, V const& v)
    : key(k),val(v)
  { }

  K key;
  V val;
};


// A linked key-value pair
template<typename K, typename V>
struct list_entry : entry<K, V>
{
  list_entry()
    : entry<K, V>(), next(nullptr)
  { }

  list_entry(K const& k, V const& v)
    : entry<K, V>(k,v), next(nullptr)
  { }

  ~list_entry()
  {
    if (nullptr != next)
      delete next;
  }

  list_entry<K, V>* next;
};


// A tree node key-value pair
template<typename K, typename V>
struct tree_entry : entry<K, V>
{
  tree_entry()
    : entry<K, V>(), left(nullptr), right(nullptr)
  { }

  tree_entry(K const& k, V const& v)
    : entry<K, V>(k,v), left(nullptr), right(nullptr)
  { }

  tree_entry<K, V>* left;
  tree_entry<K, V>* right;
};


// A basic hash table bucket containing a single entry
template<typename K, typename V>
struct basic_bucket : public entry<K, V> 
{ 
  basic_bucket()
    : entry<K, V>()
  { }

  basic_bucket(K const& k, V const& v)
    : entry<K, V>(k,v)
  { }
};


// A chained hash table bucket containing chained entries
template<typename K, typename V>
struct list_bucket 
{
  list_bucket(int n)
    : size(n)
  { 
    head = new list_entry<K, V>[n];
  }

  ~list_bucket()
  {
    delete head;
  }

  void
  add(K const& k, V const& v)
  {
    list_entry<K, V>* nh = new list_entry<K, V>(k,v);
    nh->next = head;
    head = nh;
  }

  list_entry<K, V>* head;
  int size;
};


// An array based hash table bucket
template<typename K, typename V>
struct array_bucket
{
  int size;
  array_bucket(int n)
    : size(n)      
  { 
    data = new entry<K, V>[n];
  }

  ~array_bucket()
  {
    delete[] data;
  }

  entry<K, V>* data;
};


// A tree based hash table bucket
template<typename K, typename V>
struct tree_bucket : tree_entry<K, V> { };


} // end namespace data_store


// Encapsulates the open addressing hash table family
namespace open
{

// A hash table with open addressing and linear probing for
// resolving collisions.
template<typename K, typename V, typename H, typename C>
class linear
{
  using store_type = std::vector<data_store::basic_bucket<K, V>>;
public:
  using value_type = data_store::basic_bucket<K, V>;

  using hasher = H;
  using compare = C;

  linear()
    : linear(17)
  { }

  linear(int n)
    : linear(n, hasher(), compare())
  { }

  linear(int n, hasher h, compare c)
    : size_(0), buckets_(n), hash_(h), cmp_(c), data_(n)
  { }  

  // Observers
  int size() const { return size_; }
  int buckets() const { return buckets_; }
  bool empty() const { return size_ == 0; }

  // Lookup
  value_type const* find(K const&) const;

  // Mutators
  value_type* insert(K const&, V const&);
  void erase(K const&);

private:
  int get_hash_index(K const&) const;
  int get_entry_index(K const&) const;
  
  // Resizes the hash table when the load reaches a particular value
  void resize();

private:
  int        size_;    // Current number of entries
  int        buckets_; // Number of buckets
  hasher     hash_;    // Hash function
  compare    cmp_;     // Equality comparison
  store_type data_;    // Data store
};


// Retrieves a key-value pair from the data store
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::find(K const& key) const -> value_type const*
{
  int idx = get_entry_index(key);    
  if (idx < 0)
    return nullptr;
  return &data_[idx];
}


// Inserts a new key-value pair into the data store.,
template<typename K, typename V, typename H, typename C> 
auto
linear<K, V, H, C>::insert(K const& key, V const& value) -> value_type*
{
  int idx = get_entry_index(K());
  if (idx < 0)
    return nullptr;
  data_[idx] = data_store::basic_bucket<K, V>(key, value);
  ++size;
  return &data_[idx];
}


// Removes a key-value pair from the data store.
template<typename K, typename V, typename H, typename C> 
void
linear<K, V, H, C>::erase(K const& key)
{
  int idx = get_entry_index(key);
  if (idx >= 0) {
    data_[idx] = data_store::basic_bucket<K, V>(K(), V());
    --size;  
  }
}


// Returns the index in the table for the given key. This is the actual
// index in the table for the key.
template<typename K, typename V, typename H, typename C> 
int
linear<K, V, H, C>::get_entry_index(K const& key) const
{
  int idx = get_hash_index(key); 
  for (int i = 0; i < buckets; i++) {
    if (data_[idx].key == key) 
      break;
    idx = (idx + 1 < buckets ? idx + 1 : 0);
  }
  return (data_[idx].key == key ? idx : -1);
}


// Returns an index in the table for a given key. This is not necessarily
// the actual index for the key.
template<typename K, typename V, typename H, typename C> 
inline int
linear<K, V, H, C>::get_hash_index(K const& key) const
{ 
  return hash(key) % buckets; 
}



} //end namespace open


namespace closed
{

} // end namespace closed

} // end namespace hash_table

#endif