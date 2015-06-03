#include <iostream>

#include "hash.hpp"

namespace hash_table
{
  template <typename K, typename V> 
  const data_store::basic_bucket<K,V>* 
  open::linear<K,V>::insert(const K key, const V val)
  {
    int idx = get_entry_index(K());
    if (idx < 0)
      return nullptr;
    data[idx] = data_store::basic_bucket<K,V>(key,val);
    ++size;
    return &data[idx];
  }

  
  template <typename K, typename V> 
  const data_store::basic_bucket<K,V>* 
  open::linear<K,V>::find(const K key)
  {
    int idx = get_entry_index(key);    
    if (idx < 0)
      return nullptr;
    return &data[idx];
  }

  
  template <typename K, typename V>
  void
  open::linear<K,V>::erase(const K key)
  {
    int idx = get_entry_index(key);
    if (idx >= 0) {
      data[idx] = data_store::basic_bucket<K,V>(K(),V());
      --size;  
    }
  }

  
  template <typename K, typename V>
  int
  open::linear<K,V>::get_entry_index(const K key)
  {
    int idx = get_hash_index(key); 
    for (int i = 0; i < buckets; i++) {
      if (data[idx].key == key) 
        break;
      idx = (idx + 1 < buckets ? idx + 1 : 0);
    }
    return (data[idx].key == key ? idx : -1);
  }
}