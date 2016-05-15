// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

// Trie is a tree where each vertex represents a word or prefix
// Trie here is used to find the longest prefix match in a routing table
// Matching on integral keys or bitstring of some type
// Arbitrary match: need to consider the wildcard in the key
// Wildcard has its value and masks


#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// For arbitrary match, using format Rule(value, mask, priority) as input
struct Rule
{
  uint32_t value;
  uint32_t mask;
  uint32_t priority;

  Rule()
  {
    value = 0;
    mask = 0;
    priority = 0;
  }

  Rule(uint32_t x, uint32_t y, uint32_t z)
  {
    value = x;
    mask = y;
    priority = z;
  }

};

// Convert Rule(value, mask) into integer type in a trie data structure
// Guarantee using the same input format Rule(value, mask)
void convert_rule(vector<uint32_t>& rulesTable, Rule& rule);

struct trie_node
{
  uint32_t priority; // Used to mark rule nodes, and also can show pripority
  trie_node* children[2]; // Trie stride = 1, has two pointer, '0' and '1'

  // trie_node constructor
  trie_node()
  {
    priority = 0;
    children[0] = NULL;
    children[1] = NULL;
  }

};


// Create a new trie node
trie_node* new_node();

// Determine the node whether is an end node of a rule, according to the value
bool is_rule_node(trie_node* pNode);

// Determine the node whether is independent, whether has children or not
// If it is independent, which means it can be deleted
// If not, it cannot be deleted
bool is_independent_node(trie_node* pNode);


class Trie
{
public:

  // This is the input of rules (value, mask)
  trie_node* root;
  int count; // The number of rules in a trie
  int node_count; // The number of nodes in a trie

  // Trie constructor
  Trie()
  {
    root = new_node();
    count = 0;
    node_count = 1; // Include the root node
  }
  ~Trie()
  {
    // delete_Tire();
  }

  // Insert rules into the trie

  void insert_rule_value(uint32_t rule_value);

  void insert_prefix_rule(uint32_t value, uint32_t mask);

  void insert_rule(Rule& rule);

  void insert_prefix_rule_priority(Rule& rule);

  // Search the incoming packet in the trie
  // If return 0, match miss
  // If return 1, match hit
  bool search_rule(uint32_t key);

  bool prefix_search_rule(uint32_t key);
  bool LPM_search_rule(uint32_t key);
  uint32_t LPM1_search_rule(uint32_t key);

  // Delete rules in the trie
  // When deleting nodes, needs to satisfy two conditions
  // 1. the deleting node is not the end node of a rule ( is_rule_node() )
  // 2. the deleting node does not have childrens ( is_independent_node() )
  void delete_rule(uint32_t rule);

private:
  // Remove trie node in the trie
  // Helping delete rules
bool remove(trie_node *pNode, uint32_t rule, int level, int len);

};

