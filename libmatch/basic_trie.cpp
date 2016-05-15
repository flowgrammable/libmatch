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

// Check whether Rule a is a subset of Rule b
// Rule (value, mask, priority)
// Rule a = 10****, Rule b = 1*****, which means Rule a is a subset of Rule b

bool is_subset(Rule a, Rule b)
{
  return ( a.value & b.value == b.value ) && ( a.mask | b.mask == b.mask );
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


// Insert rules: original insert function, without expanding ** part.
void Trie::insert_rule_value(uint32_t rule_value)
{
  int level;
  int index;
  trie_node* pRule;
  count++;
  pRule = root; // Created the first node in a trie
  // Here is 32, because of the fixed length uint32_t
  for (level=0; level<32; level++) {
    // Get the index value of each bit, totally is 32
    index = (rule_value >> (31-level)) & 1;
    // if the key is not present in the trie (is NULL), insert a new node
    if ( !pRule->children[index] ) {
      pRule->children[index] = new_node();
      node_count++; // Created a new node in a trie
    }
    pRule = pRule->children[index];
  }
  pRule->priority = count; // If the priority is not 0, the node is leaf node

}

// Insert rules with Rule(value, mask)
// Expand wildcard part in a Rule(value, mask)
void Trie::insert_rule( Rule& rule )
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
    insert_rule_value(newRule);

  }

}


// Insert prefix rules with Rule(value, mask, priority)
// Added the priority purpose
// Inserting rules will depend on the priority
// The priority must be in-order of insertion
// Which can reduce the number of inserted rules, make the data structure smaller

void Trie::insert_prefix_rule_priority( Rule& rule )
{
  trie_node* pRule = root;
  // Considering the prefix rules, thus the length of rule should be different
  // depends on the mask value, the length of each rule = 32-mask
  // The length of wildcard = mask number

  uint32_t mask_num = mylog2(rule.mask+1);
  // Has a bug here: when 32 bits are all wildcard, will overflow
  uint32_t prefix_len = 32 - mask_num;

  for (int level=0; level<prefix_len; level++) {
    // Get the index value of each bit, totally is 32
    int index = (rule.value >> (31-level)) & 1;
    // if the key is not present in the trie (is NULL), insert a new node
    if ( !pRule->children[index] ) {
      pRule->children[index] = new_node();
      node_count++; // Created a new node in a trie
    }
    // move to child node:
    pRule = pRule->children[index];
    // check if rule exists:
    if (pRule->priority != 0) {
      // do not insert rule...
      // probably print a message of interest
      return;
    }
  }
  // Insert the new rule:
  count++;
  pRule->priority = rule.priority; // If the priority is not 0, the node is leaf node
  //cout << pRule->priority << " " << pRule->star_num << endl;
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
// Return the match priority, show which rule is being matched
uint32_t Trie::LPM1_search_rule(uint32_t key)
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

  return match;
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

