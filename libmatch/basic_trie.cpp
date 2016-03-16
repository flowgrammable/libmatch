// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include <iostream>
#include <vector>
#include <string>
#include "basic_trie.hpp"

using namespace std;

// Convert the input rule format Rule(value, mask) into the integer type in trie
void convert_rule(vector<uint8_t>& rulesTable, Rule& rule)
{
  // Store the wildcard postion into vector maskPosion
  vector<uint8_t> maskPosition;
  // Check the mask field from the lower bit
  for(int i = 0; i < 8; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((rule.mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint8_t num = maskPosition.size(); // num is the number of wildcard
  uint8_t base = rule.value & (~rule.mask); // Get the value field of each rule
  for(int i = 0; i < (1 << num); i++) {
    uint8_t newRule = base; // This is the base rule, smallest integer rule
    for(int j = 0; j < num; j++) {
      if(((1 << j) & i) == 0) {
        // get the newRule for ruleTables
        newRule |= (1 << maskPosition.at(j));
      }
    }
    rulesTable.push_back(newRule);
  }
}

// Create a new trie node
trie_node* new_node()
{
  trie_node* pNode = new trie_node();
  if (pNode) {
    pNode->value = 0;
    pNode->children[0] = NULL;
    pNode->children[1] = NULL;
  }
  return pNode;
}

// Determine the node whether is leaf node
// If the value = 0, it is not leaf node
// If the value != 0, it is leaf node
bool is_rule_node(trie_node *pNode)
{
  if(pNode->value == 0) {
    return false;
  }
  else {
    return true;
  }
  // If the value != 0, it is a rule ending node
}

// Determine whether a node has children or not
// If the node has no children, then it is an independent node
// If the node has children, it is not
bool is_independent_node(trie_node *pNode)
{
  if( (pNode->children[0] == NULL) && (pNode->children[1] == NULL) ) {
    return true;
  }
  else {
    return false;
  }
}

// Insert rules
void Trie::insert_rule(uint8_t rule)
{
  int level;
  int index;
  trie_node* pRule;
  count++;
  pRule = root;
  // Here is 8, because of the fixed length uint8_t
  for (level=0; level<8; level++) {
    // Get the index value of each bit, totally is 8
    index = (rule >> (7-level)) & 1;
    // if the key is not present in the trie (is NULL), insert a new node
    if ( !pRule->children[index] ) {
      pRule->children[index] = new_node();
    }
    pRule = pRule->children[index];
  }
  pRule->value = count; // If the value is not 0, the node is leaf node

}

// Lookup--search rules
bool Trie::search_rule(uint8_t key)
{
  int level;
  int index;  // The index of children, here just has 2 children
  trie_node* pRule;
  pRule = root;
  for (level=0; level<8; level++) {
    index = (key >> (7-level)) & 1;
    if ( !pRule->children[index] ) {
      return 0;
    }
    pRule = pRule->children[index];
  }
  // If return 0, match miss
  // If return 1, match hit
  return (0 != pRule && pRule->value);

}


bool Trie::remove(trie_node* pNode, uint8_t rule, int level, int len)
{
  int index;
  if(pNode) {
    if(level == len) {
      pNode->value = 0; // Unmark leaf node
      // If this node has no children, can be deleted
      if(is_independent_node(pNode)) {
        return true;
      }
      else {
        return false;
      }
    }
    else {
      index = (rule >> (7-level)) & 1;
      if(remove(pNode->children[index], rule, level+1, len)) {
        delete pNode->children[index]; // Last node marked, delete it
        pNode->children[index] = NULL;
        // Determine whether the upper nodes should be deleted, parent node
        return(!is_rule_node(pNode) && is_independent_node(pNode));
      }
    }
  }
  return false;
}

// Delete rules
void Trie::delete_rule(uint8_t rule)
{
  int len = 8;
  if( len > 0 ) {
    remove(root, rule, 0, len);
  }
}

