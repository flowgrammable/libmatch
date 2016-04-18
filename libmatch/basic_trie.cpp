// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include <iostream>
#include <vector>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <algorithm>
#include "basic_trie.hpp"

using namespace std;

// This function is caculating the log2 value
static uint32_t mylog2 (uint32_t val) {
  if (val == 0) return UINT_MAX;
  if (val == 1) return 0;
  uint32_t ret = 0;
  while (val > 1) {
    val >>= 1;
    ret++;
  }
  return ret;
}

// Convert the input rule format Rule(value, mask) into the integer type in trie
void convert_rule(vector<uint32_t>& rulesTable, Rule& rule)
{
  // Store the wildcard postion into vector maskPosion
  vector<uint32_t> maskPosition;
  // Check the mask field from the lower bit
  for(int i = 0; i < 32; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((rule.mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint32_t num = maskPosition.size(); // num is the number of wildcard
  uint32_t base = rule.value & (~rule.mask); // Get the value field of each rule
  for(int i = 0; i < (1 << num); i++) {
    uint32_t newRule = base; // This is the base rule, smallest integer rule
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
    pNode->priority = 0;
    pNode->star_num = 0;
    pNode->children[0] = NULL;
    pNode->children[1] = NULL;
  }
  return pNode;
}

// Determine the node whether is leaf node
// If the priority = 0, it is not leaf node
// If the priority != 0, it is leaf node
bool is_rule_node(trie_node *pNode)
{
  if(pNode->priority == 0) {
    return false;
  }
  else {
    return true;
  }
  // If the priority != 0, it is a rule ending node
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
void Trie::insert_rule(uint32_t rule)
{
  int level;
  int index;
  trie_node* pRule;
  count++;
  pRule = root;
  // Here is 32, because of the fixed length uint32_t
  for (level=0; level<32; level++) {
    // Get the index value of each bit, totally is 32
    index = (rule >> (31-level)) & 1;
    // if the key is not present in the trie (is NULL), insert a new node
    if ( !pRule->children[index] ) {
      pRule->children[index] = new_node();
    }
    pRule = pRule->children[index];
  }
  pRule->priority = count; // If the priority is not 0, the node is leaf node

}

// Insert rules with Rule(value, mask)
// Expand wildcard part in a Rule(value, mask)
void Trie::insert_rule(uint32_t value, uint32_t mask)
{
  // Store the wildcard postion into vector maskPosion
  vector<uint32_t> maskPosition;
  // Check the mask field from the lower bit
  for(int i = 0; i < 32; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint32_t num = maskPosition.size(); // num is the number of wildcard
  uint32_t base = value & (~mask); // Get the value field of each rule
  for(int i = 0; i < (1 << num); i++) {
    uint32_t newRule = base; // This is the base rule, smallest integer rule
    for(int j = 0; j < num; j++) {
      if(((1 << j) & i) == 0) {
        // get the newRule for ruleTables
        newRule |= (1 << maskPosition.at(j));
      }
    }
    insert_rule(newRule);

  }

}

// Insert prefix rules with Rule(value, mask)
void Trie::insert_prefix_rule(uint32_t value, uint32_t mask)
{
  int level;
  int index;
  trie_node* pRule;
  count++;
  pRule = root;
  // Considering the prefix rules, thus the length of rule should be different
  // depends on the mask value, the length of each rule = 32-mask
  // The length of wildcard = mask number

  uint32_t mask_num = mylog2(mask+1);
  uint32_t layer = 32 - mask_num;

  for (level=0; level<layer; level++) {
    // Get the index value of each bit, totally is 32
    index = (value >> (31-level)) & 1;
    // if the key is not present in the trie (is NULL), insert a new node
    if ( !pRule->children[index] ) {
      pRule->children[index] = new_node();
    }
    pRule = pRule->children[index];
  }
  pRule->priority = count; // If the priority is not 0, the node is leaf node
  pRule->star_num = mask_num;
  //cout << pRule->priority << " " << pRule->star_num << endl;
}

// Prefix rules lookup--search rules
bool Trie::LPM_search_rule(uint32_t key)
{
  trie_node* pRule;
  int index;  // The index of children, here just has 2 children
  int level;
  pRule = root;

  for (level=0; level<32; level++) {
    index = (key >> (31-level)) & 1;

    if ( !pRule->children[index] && pRule->priority == 0) {
      return 0;
    }
    if ( !pRule->children[index] && pRule->priority != 0) {
      return 1;
    }
    if ( pRule->children[index] && pRule->children[index]->priority == 0) {
      return 1;
    }
    if ( pRule->children[index] && pRule->children[index]->priority != 0) {
      pRule = pRule->children[index];
      continue;
    }
  }

  return false;
  // If return 0, match miss
  // If return 1, match hit
}

// Prefix rules lookup--search rules
bool Trie::LPM1_search_rule(uint32_t key)
{
  trie_node* pRule = root;
  uint32_t match = 0;

  for (int level=0; level<32; level++) {
    int index = (key >> (31-level)) & 1;
    // Choose child based on relelvant bit in key.
    // If child exists, recurse:
    pRule = pRule->children[index];
    if ( pRule ) {
      // Node is a rule, save match:
      if ( pRule->priority != 0 ) {
        match = pRule->priority;
      }
    }
    else {
      // Else, child does't exist. Stop and return last match.
      break;
    }
  }

  return match != 0;
  // If return 0, match miss
  // If return 1, match hit
}


// Lookup--search rules
bool Trie::search_rule(uint32_t key)
{
  int level;
  int index;  // The index of children, here just has 2 children
  trie_node* pRule;
  pRule = root;
  for (level=0; level<32; level++) {
    index = (key >> (31-level)) & 1;
    if ( !pRule->children[index] ) {
      return 0;
    }
    pRule = pRule->children[index];
  }
  // If return 0, match miss
  // If return 1, match hit
  return (0 != pRule && pRule->priority);

}


bool Trie::remove(trie_node* pNode, uint32_t rule, int level, int len)
{
  int index;
  if(pNode) {
    if(level == len) {
      pNode->priority = 0; // Unmark leaf node
      // If this node has no children, can be deleted
      if(is_independent_node(pNode)) {
        return true;
      }
      else {
        return false;
      }
    }
    else {
      index = (rule >> (31-level)) & 1;
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
void Trie::delete_rule(uint32_t rule)
{
  int len = 32;
  if( len > 0 ) {
    remove(root, rule, 0, len);
  }
}

