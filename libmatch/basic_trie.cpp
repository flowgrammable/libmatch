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

  pRule->value = pTrie->count;

}

int Trie::search_rule(Trie *pTrie, string key)
{
  int level; 
  int index;
  trie_node *pRule;

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
      if ( !pRule->children[index] )
	{
	  return 0;
	}
      pRule = pRule->children[index];

    }

  // if return 0, then the incoming packet is not in the trie
  return (0 != pRule && pRule->value);
}

int main()
{
  vector<string> list = {"0010", "100", "00", "10101", "0", "110"};
  Trie trie;
  char output[][32] = {"Not present in trie", "Present in trie"};
  Trie::init_trie(&trie);
  for (int i=0; i<list.size(); i++)
    {
      Trie::insert_rule(&trie, list.at(i));
    }

  cout << output[Trie::search_rule(&trie, "10101")] << endl;
  cout << output[Trie::search_rule(&trie, "10111")] << endl;
  cout << output[Trie::search_rule(&trie, "100")] << endl;
  cout << output[Trie::search_rule(&trie, "1")] << endl;

  return 0;

}
