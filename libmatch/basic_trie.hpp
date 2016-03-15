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

struct Rule
{
    uint8_t value;
    uint8_t mask;
    Rule()
    {
        value = 0;
        mask = 0;
    }

    Rule(uint8_t x, uint8_t y)
    {
        value = x;
        mask = y;
    }

};

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

    // This is the input of rules (value, mask)
    vector<uint8_t> rulesTable;
    trie_node *root;
    int count; // The number of rules in a trie

    // Trie constructor

    Trie()
    {
        root = NULL;
        count = 0;
    }
    ~Trie()
    {
        // delete_Tire();
    }

    // Return a new trie node
    static trie_node* get_node();

    // Determine the node whether is a end node of a rule, according to the value
    static bool is_rule_node(trie_node *pNode);

    // Determine the node whether is independent, whether has children or not
    // If it is independent, which means it can be deleted
    // If not, it cannot be deleted
    static bool is_independent_node(trie_node *pNode);

    // Convert Rule(value, mask) into integer type in a trie data structure
    // Guarantee using the same input Rule(value, mask)
    // static void convert_rule(vector<uint8_t>& rulesTable, Rule& rule);

    // Initilize a trie
    static void init_trie(Trie *pTrie);

    // Insert rules into the trie

    static void insert_rule(Trie *pTrie, uint8_t rule);

    // Search the incoming packet in the trie
    // If return 0, match miss
    // If return 1, match hit

    static bool search_rule(Trie *pTrie, uint8_t key);

    // Delete rules in the trie
    // When deleting nodes, needs to satisfy two conditions
    // 1. the deleting node is not the end node of a rule ( is_rule_node() )
    // 2. the deleting node does not have childrens ( is_independent_node() )

    static void delete_rule(Trie *pTrie, uint8_t rule);

    // Remove trie node in the trie
    // Helping delete rules

    static bool remove(trie_node *pNode, uint8_t rule, int level, int len);

};

