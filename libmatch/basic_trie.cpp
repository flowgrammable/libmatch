// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include <iostream>
#include <vector>
#include <string>
#include "basic_trie.hpp"
 
using namespace std;

// Trie node which contains two pointers: left and right
// int value represent if the node is leaf node


trie_node* Trie::get_node()
{
trie_node *pNode = new trie_node();
  if (pNode)
    {
      pNode->value = 0;
      pNode->children[0] = NULL;
      pNode->children[1] = NULL;
    }
  return pNode;
}

// Determine the node whether is leaf node
// If the value = 0, it is not leaf node
// If the value != 0, it is leaf node
bool Trie::is_rule_node(trie_node *pNode)
{
  if(pNode->value == 0)
    {
      return false;
    }
  else
    {
      return true;
    }
// If the value != 0, it is a rule ending node
}

// Determine whether a node has children or not
// If the node has no children, then it is an independent node
// If the node has children, it is not
bool Trie::is_independent_node(trie_node *pNode)
{
  if( (pNode->children[0] == NULL) && (pNode->children[1] == NULL) )
    {
      return true;
    }
  else
    {
      return false;
    }
}


void Trie::init_trie(Trie *pTrie)
{
  pTrie->root = get_node();
  pTrie->count = 0;
}


void Trie::convert_rule(vector<uint8_t>& rulesTable, Rule& rule)
{
    // Store the wildcard postion into vector maskPosion
    // Check the mask field from the lower bit

    vector<uint8_t> maskPosition;
    for(uint8_t i = 0; i < 8; i++) {
        if((rule.mask << i) & 1 == 1) {
            maskPosition.push_back(i);
        }
    }
    uint8_t num = maskPosition.size(); // k is the number of wildcard
    uint8_t base = rule.value & (~rule.mask);
    for(uint8_t i = 0; i < pow(2,num); i++) {
        uint8_t newRule = base;
        for(uint8_t j = 0; j < num; j++) {
            if((1 << j) & i == 0) {
                newRule = newRule + (1 << maskPosition.at(j));
            }
        }
          rulesTable.push_back(newRule);
    }
}


void Trie::insert_rule(Trie *pTrie, uint8_t rule)
{
  int level;
  int index;
  trie_node *pRule;

  pTrie->count++;
  pRule = pTrie->root;

  for (level=0; level<8; level++)
    {
      index = (rule >> (7-level)) & 1;

      // if the key is not present in the trie, insert a new node
      if ( !pRule->children[index] )
	{
	  pRule->children[index] = get_node();
	}
      pRule = pRule->children[index];

    }

  pRule->value = pTrie->count; // If the value is not 0, the node is leaf node

}


bool Trie::search_rule(Trie *pTrie, uint8_t key)
    {
      int level;
      int index;  // The index of children, here just has 2 children
      trie_node *pRule;

      pRule = pTrie->root;

      for (level=0; level<8; level++)
        {
          index = (key >> (7-level)) & 1;

          if ( !pRule->children[index] )
          return 0;
          pRule = pRule->children[index];

        }

      // If return 0, match miss
      // If return 1, match hit
      return (0 != pRule && pRule->value);

    }


bool Trie::remove(trie_node *pNode, uint8_t rule, int level, int len)
{
  //cout << pNode->value << (pNode->children[0] == NULL) << (pNode->children[1] == NULL) << endl;
  int index;
  if(pNode)
    {
      if(level == len)
	{
	  pNode->value = 0; // Unmark leaf node
	  // If this node has no children, can be deleted
	  if(Trie::is_independent_node(pNode))
	      return true;
	  else
	      return false;
	}
      else
	{
      index = (rule >> (7-level)) & 1;
          if(remove(pNode->children[index], rule, level+1, len))
	    {
	      // cout << 1 << endl;
	      delete pNode->children[index]; // Last node marked, delete it
	      pNode->children[index] = NULL;
	      // Determine whether the upper nodes should be deleted, parent node
	      return(!Trie::is_rule_node(pNode) && Trie::is_independent_node(pNode));

	    }
	}
    }
  return false;
}


void Trie::delete_rule(Trie *pTrie, uint8_t rule)
{
  int len = 8;
  if( len > 0 )
    {
      remove(pTrie->root, rule, 0, len);
    }
}
 
