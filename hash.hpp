#ifndef HASH_HPP
#define HASH_HPP
#include <vector>

namespace hash_table
{

using hash_func = size_t (*)(const void*);
using comp_func = bool (*)(void*, void*);

namespace data_store
{
  // A simple key-value pair
  template <typename K, typename V>
  struct entry
  {
    K key;
    V val;
    
    entry()
      : key(K()),val(V())
    { }
    
    entry(const K k, const V v)
      : key(k),val(v)
    { }
  };

  
  // A linked key-value pair
  template <typename K, typename V>
  struct list_entry : entry<K,V>
  {
    list_entry<K,V>* next;

    list_entry()
      : entry<K,V>(), next(nullptr)
    { }

    list_entry(const K k, const V v)
      : entry<K,V>(k,v), next(nullptr)
    { }

    ~list_entry()
    {
      if (nullptr != next)
        delete next;
    }
  };

  
  // A tree node key-value pair
  template <typename K, typename V>
  struct tree_entry : entry<K,V>
  {
    tree_entry<K,V>* left;
    tree_entry<K,V>* right;

    tree_entry()
      : entry<K,V>(), left(nullptr), right(nullptr)
    { }

    tree_entry(const K k, const V v)
      : entry<K,V>(k,v), left(nullptr), right(nullptr)
    { }
  };

  
  // A basic hash table bucket containing a single entry
  template <typename K, typename V>
  struct basic_bucket : public entry<K,V> 
  { 
    basic_bucket()
      : entry<K,V>()
    { }

    basic_bucket(const K k, const V v)
      : entry<K,V>(k,v)
    { }
  };
  

  // A chained hash table bucket containing chained entries
  template <typename K, typename V>
  struct list_bucket 
  {
    list_entry<K,V>* head;
    size_t size;
    list_bucket(size_t n)
      : size(n)
    { 
      head = new list_entry<K,V>[n];
    }

    ~list_bucket()
    {
      delete head;
    }

    void
    add(const K k, const V v)
    {
      list_entry<K,V>* nh = new list_entry<K,V>(k,v);
      nh->next = head;
      head = nh;
    }
  };


  // An array based hash table bucket
  template <typename K, typename V>
  struct array_bucket
  {
    entry<K,V>* data;
    size_t size;
    array_bucket(size_t n)
      : size(n)      
    { 
      data = new entry<K,V>[n];
    }

    ~array_bucket()
    {
      delete[] data;
    }
  };


  // A tree based hash table bucket
  template <typename K, typename V>
  struct tree_bucket : tree_entry<K,V> { };

} // end namespace data_store

// Encapsulates the open addressing hash table family
namespace open
{
  // A hash table with open addressing and linear probing for
  // resolving collisions.
  template <typename K, typename V>
  class linear
  {
    // The current number of entries
    size_t size;

    // The capacity of the hash table
    size_t buckets;

    // Hashing function
    hash_func hash;

    // Data store
    std::vector< data_store::basic_bucket<K,V> > data;

  public:

    linear(size_t n, hash_func hsh)
      : size(0),buckets(n),data(n)
    { 
      hash = hsh;
    }

    
    ~linear() { }

    
    // Inserts a new key-value pair into the data store
    const data_store::basic_bucket<K,V>* insert(const K, const V);
    
    
    // Retrieves a key-value pair from the data store
    const data_store::basic_bucket<K,V>* find(const K);
    
    
    // Removes a key-value pair from the data store
    void erase(const K);

  private:
    
    // Resizes the hash table when the load reaches a particular value
    void resize();

    
    // Returns an index in the table for a given key. This is not necessarily
    // the actual index for the key.
    size_t get_hash_index(const K key) { return hash(key) % buckets; }  


    // Returns the index in the table for the given key. This is the actual
    // index in the table for the key.
    int get_entry_index(const K);  
    
  };
} //end namespace open

namespace closed
{

} // end namespace closed

} // end namespace hash_table

#endif