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

void Trie::insert_rule(Trie *pTrie, string key)
{
  int level;
  int index;
  trie_node *pRule;

  pTrie->count++;
  pRule = pTrie->root;

  for (level=0; level<key.length(); level++)
    {
      if (key.at(level) == '0')
	{
	  index = 0;
	}
      else if (key.at(level) == '1')
	{
	  index = 1;
	}

      // if the key is not present in the trie, insert a new node
      if ( !pRule->children[index] )
	{
	  pRule->children[index] = get_node();
	}
      pRule = pRule->children[index];

    }

  pRule->value = pTrie->count; // If the value is not 0, the node is leaf node

}

bool Trie::search_rule(Trie *pTrie, string packet)
{
  int level; 
  int index;  // The index of children, here just has 2 children
  trie_node *pRule;

  pRule = pTrie->root;

  for (level=0; level<packet.length(); level++)
    {
      if (packet.at(level) == '0')
	{
	  index = 0;
	}
      else if (packet.at(level) == '1')
	{
	  index = 1;
	}
      if ( !pRule->children[index] )
	{
	  return 0;
	}
      pRule = pRule->children[index];

    }
 
  // If return 0, match miss
  // If return 1, match hit
  return (0 != pRule && pRule->value);

}

bool Trie::remove(trie_node *pNode, string key, int level, int len)
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
	    {
	      return true;
	    }
	  else
	    {
	      return false;
	    }
	}
      else
	{
	  if(key.at(level) == '0')
	    {
	      index = 0;
	    }
	  else if(key.at(level) == '1')
	    {
	      index = 1;
	    }
	  if(remove(pNode->children[index], key, level+1, len))
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


void Trie::delete_rule(Trie *pTrie, string key)
{
  int len = key.length();
  if( len > 0 )
    {
      remove(pTrie->root, key, 0, len);
    }
}
 
