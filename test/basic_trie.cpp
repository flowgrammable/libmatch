// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>

// Matches integer type


int main()
{
  vector<uint8_t> list = {28, 30, 128, 200, 243};
  Trie trie;

  // Initilize the trie
  Trie::init_trie(&trie);

  // Insert all the rules in the list
  for (int i=0; i<list.size(); i++)
    {
      Trie::insert_rule(&trie, list.at(i));
    }

  char output[][32] = {"Not present in trie", "Present in trie"};

  // Search the rules
  cout << output[Trie::search_rule(&trie, 28)] << endl;
  cout << output[Trie::search_rule(&trie, 30)] << endl;
  cout << output[Trie::search_rule(&trie, 35)] << endl;
  cout << output[Trie::search_rule(&trie, 243)] << endl;


  // Delete rules
  // When deleting nodes, needs to satisfy two conditions
  // 1. the deleting node is not the end node of a rule ( is_rule_node() )
  // 2. the deleting node does not have childrens ( is_independent_node() )

  Trie::delete_rule(&trie, list.at(1));
  //Trie::delete_rule(&trie, list.at(2));

  cout << output[Trie::search_rule(&trie, 30)] << endl;
  cout << output[Trie::search_rule(&trie, 128)] << endl;


  //cout << output[Trie::search_rule(&trie, "1001")] << endl;
  //cout << output[Trie::search_rule(&trie, "100")] << endl;

  return 0;

}
