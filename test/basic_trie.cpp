// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>

// Matches string type 


int main()
{
  vector<string> list = {"0010", "100", "1001", "00", "10101", "0", "110"};
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
  cout << output[Trie::search_rule(&trie, "1001")] << endl;
  cout << output[Trie::search_rule(&trie, "10101")] << endl;
  cout << output[Trie::search_rule(&trie, "10111")] << endl;
  cout << output[Trie::search_rule(&trie, "100")] << endl;
  cout << output[Trie::search_rule(&trie, "1")] << endl;
  cout << output[Trie::search_rule(&trie, "0010")] << endl;

  // Delete rules
  // When deleting nodes, needs to satisfy two conditions
  // 1. the deleting node is not the end node of a rule ( is_rule_node() )
  // 2. the deleting node does not have childrens ( is_independent_node() )

  Trie::delete_rule(&trie, list.at(1));
  //Trie::delete_rule(&trie, list.at(2));

  cout << output[Trie::search_rule(&trie, "1001")] << endl;
  cout << output[Trie::search_rule(&trie, "100")] << endl;


  //cout << output[Trie::search_rule(&trie, "1001")] << endl;
  //cout << output[Trie::search_rule(&trie, "100")] << endl;

  return 0;

}
