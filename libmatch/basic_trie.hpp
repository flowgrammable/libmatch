// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

// Trie is a tree where each vertex represents a word or prefix
// Trie here is used to find the longest prefix match in a routing table

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct trie_node
{
  int value; // Used to mark leaf nodes
  trie_node *children[2]; // Trie stride = 1, has two pointer, '0' and '1'
  // trie_node constructor

  trie_node()
  {
    value = 0;
    children[0] = NULL;
    children[1] = NULL;
  }

};

class Trie
{
public:
  trie_node *root;
  int count; // The number of rules in a trie

  // Trie constructor

  Trie()
  {
    root = NULL;
    count = 0;
  }

  // Return a new trie node
  static trie_node* get_node(); 

  // Initilize a trie
  static void init_trie(Trie *pTrie);

  // Insert string type rules into the trie
  static void insert_rule(Trie *pTrie, string key);

  // Search the incoming packet in the trie
  // if return 0, then miss match
  // else, hit match
  static int search_rule(Trie *pTrie, string packet);

  // Delete rules in the trie
  static void delete_rule();

};

