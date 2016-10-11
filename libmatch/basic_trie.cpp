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
static uint64_t mylog2 (uint64_t val) {
  if (val == 0) return UINT_MAX;
  if (val == 1) return 0;
  uint64_t ret = 0;
  while (val > 1) {
    val >>= 1;
    ret++;
  }
  return ret;
}

// Check whether Rule a is a subset of Rule b
// Rule (value, mask, priority)
// Rule a = 10****, Rule b = 1*****, which means Rule a is a subset of Rule b
/*
bool is_subset(Rule a, Rule b)
{
  return ( a.value & b.value == b.value ) && ( a.mask | b.mask == b.mask );
}

*/


// Convert the input rule format Rule(value, mask) into the integer type in trie
void convert_rule(vector<uint64_t>& rulesTable, Rule& rule)
{
  // Store the wildcard postion into vector maskPosion
  vector<uint32_t> maskPosition;
  // Check the mask field from the lower bit
  for(int i = 0; i < 64; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((rule.mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint32_t num = maskPosition.size(); // num is the number of wildcard
  uint64_t base = rule.value & (~rule.mask); // Get the value field of each rule
  for(int i = 0; i < (1 << num); i++) {
    uint64_t expandedRule = base; // This is the base rule, smallest integer rule
    for(int j = 0; j < num; j++) {
      if(((1 << j) & i) == 0) {
        // get the expandedRule for ruleTables
        expandedRule |= (1 << maskPosition.at(j));
      }
    }

    rulesTable.push_back(expandedRule);
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
  if (pNode->priority == 0) {
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
  if ( (pNode->children[0] == NULL) && (pNode->children[1] == NULL) ) {
    return true;
  }
  else {
    return false;
  }
}


void Trie::delete_trie()
{
  /*
  for (uint64_t i = 0; i < node_count; i++) {
    //pNode->priority = NULL;
    pNode->children[0] = NULL;
    pNode->children[1] = NULL;
  }
  */
  delete root;
  count = 0;
  node_count = 0;
}

// Insert rules: original insert function, without expanding ** part.
void Trie::insert_rule_value(uint64_t rule_value)
{
  int level;
  int index;
  trie_node* pRule;
  count++;
  pRule = root; // Created the first node in a trie
  // Here is 64, because of the fixed length uint64_t
  for (level=0; level<64; level++) {
    // Get the index value of each bit, totally is 32
    index = (rule_value >> (63-level)) & 1;
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
  for(int i = 0; i < 64; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((rule.mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint32_t num = maskPosition.size(); // num is the number of wildcard
  uint64_t base = rule.value & (~rule.mask); // Get the value field of each rule
  for(int i = 0; i < (1 << num); i++) {
    uint64_t expandedRule = base; // This is the base rule, smallest integer rule
    for(int j = 0; j < num; j++) {
      if(((1 << j) & i) == 0) {
        // get the expandedRule for ruleTables
        expandedRule |= (1 << maskPosition.at(j));
      }
    }
    insert_rule_value(expandedRule);

  }

}

// Get the "*" wildcard number after the multi-prefix number
uint32_t Trie::get_new_num(Rule& rule)
{
  int boundary = 0;
  for (int i=0; i<64; i++) {
    // Find the first bit "0" from the least significant bit
    // 10x1xx, so the mask is 001011
    if ( ((rule.mask >> i) & uint64_t(1)) == 0 ) {
      boundary = i;
      break;
      //cout << "boundary is" << " " << boundary << endl;
    }
    else {
      continue;
    }
  }
  vector<uint32_t> maskNewPosition;
  for (int j=(boundary+1); j<64; j++) {
    if ( ((rule.mask >> j) & uint64_t(1)) == 1 ) {
      maskNewPosition.push_back(j); // recored the positions that should be expanded (the bit is "1")
      continue; // record all the candidates in 64-bit
    }
    else {
      continue; // check all the 64-bit
    }
  }
  // Get all the "1" in the new rule, besides the prefix part
  // need to expand all the "1" part
  uint32_t new_num = maskNewPosition.size(); // num is the number of wildcard
  //cout << new_num << endl;
  return new_num;
}


void Trie::expand_rule( Rule& rule )
{

  //uint64_t expand_count_sum = 0;
  //cout << " The priority is " << " " << rule.priority << endl;
  int boundary1 = 0;
  for (int i = 0; i < 64; i++) {
    // Find the first bit "0" from the least significant bit
    // 10x1xx, so the mask is 001011
    if ( ((rule.mask >> i) & uint64_t(1)) == 0 ) {
      boundary1 = i;
      break;
      //cout << "boundary is" << " " << boundary << endl;
    }
    else {
      continue;
    }
  }
  vector<uint32_t> maskNewPosition1;
  for (int j=(boundary1+1); j<64; j++) {
    if ( ((rule.mask >> j) & uint64_t(1)) == 1 ) {
      maskNewPosition1.push_back(j); // recored the positions that should be expanded (the bit is "1")
      continue; // record all the candidates in 64-bit
    }
    else {
      continue; // check all the 64-bit
    }
  }
  // Get all the "1" in the new rule, besides the prefix part
  // need to expand all the "1" part
  uint32_t new_num1 = maskNewPosition1.size(); // num is the number of wildcard
  //cout << new_num1 << endl;

  /*

  if (new_num > 11) {
    cout << "return" << endl;
    return;
  }
  */

  Rule expandedRule;
  vector<Rule> expandedPingRulesTable;

  // From the priority = 40 rule, starts having wildcard, not the prifix rules,
  // need to be expanded

  //cout << " mask is " << " " << rule.mask << " value is " << " " << rule.value
  //<< " priority is " << " " << rule.priority << endl;
  //uint64_t expand_count = 0;
  for(uint64_t i = 0; i < ( uint64_t(1) << new_num1 ); i++) {

    expandedRule.value = rule.value; // base value
    // Show the last expandedRule value
    //cout << expandedRule.value << endl;
    // expand into the number of rules: depending on the number of wildcard
    //cout << " number of wildcard: " << " " << new_num << endl;
    for(int j = 0; j < new_num1; j++) {
      // Get the weight on all the new_num bit
      // which produce every conbination
      // ex. for the first combination value is "0"
      // so on each new_num bit, the weight are all 0
      // ex. for the second conbination value is "1"
      // then the "00001", so the weight of the first bit of new_num bits is (uint64_t(1) << 0)
      if(((uint64_t(1) << j) & i) == 0) {
        // get the expandedRule for ruleTables
        expandedRule.value = expandedRule.value;
        expandedRule.mask = ( uint64_t(1) << boundary1 ) - 1; // mask value should be a prefix value after expanded
        expandedRule.priority = rule.priority; // priority keeps the same
        //cout << " check the priority is " << " " << expandedRule.priority << endl;
      }
      else {
        // ((1 << j) & i) != 0
        expandedRule.value |= (uint64_t(1) << maskNewPosition1.at(j));
        expandedRule.mask = ( uint64_t(1) << boundary1 ) - 1; // mask value should be a prefix value after expanded
        expandedRule.priority = rule.priority; // priority keeps the same
      }
    }
    // Every combinations on new_num bit
    expandedPingRulesTable.push_back(expandedRule);
    insert_prefix_rule_priority(expandedRule);
    expand_count++;

  }

  //cout << expandedPingRulesTable.size() << endl; // check the expanded rule size, should be the power of 2

  //cout << "expanded count:" << " " << expand_count << endl;
  //cout << " total expanded count:" << " " << expand_count_sum << endl;

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
  uint32_t prefix_len = 64 - mask_num;

  for (int level = 0; level < prefix_len; level++) {
    // Get the index value of each bit, totally is 32
    int index = (rule.value >> (63-level)) & uint64_t(1);
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
void Trie::insert_prefix_rule(uint64_t value, uint64_t mask)
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
  uint32_t layer = 64 - mask_num;

  for (level=0; level<layer; level++) {
    // Get the index value of each bit, totally is 32
    index = (value >> (63-level)) & 1;
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
bool Trie::LPM_search_rule(uint64_t key)
{
  trie_node* pRule;
  int index;  // The index of children, here just has 2 children
  int level;
  pRule = root;

  for (level=0; level<64; level++) {
    index = (key >> (63-level)) & 1;

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
uint32_t Trie::LPM1_search_rule(uint64_t key)
{
  trie_node* pRule = root;
  uint32_t match = 0;

  for (int level=0; level<64; level++) {
    int index = (key >> (63-level)) & uint64_t(1);
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
bool Trie::search_rule(uint64_t key)
{
  int level;
  int index;  // The index of children, here just has 2 children
  trie_node* pRule;
  pRule = root;
  for (level=0; level<64; level++) {
    index = (key >> (63-level)) & 1;
    if ( !pRule->children[index] ) {
      return 0;
    }
    pRule = pRule->children[index];
  }
  // If return 0, match miss
  // If return 1, match hit
  return (0 != pRule && pRule->priority);

}

void Trie::resetRule(Rule& rule)
{
  rule.value = 0;
  rule.mask = 0;
  rule.priority = 0;
}


bool Trie::remove(trie_node* pNode, uint64_t rule, int level, int len)
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
      index = (rule >> (63-level)) & 1;
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
void Trie::delete_rule(uint64_t rule)
{
  int len = 64;
  if( len > 0 ) {
    remove(root, rule, 0, len);
  }
}

