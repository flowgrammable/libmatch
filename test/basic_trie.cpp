// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>

// Matches integer type


int main()
{
    vector<Rule> list = {Rule(102,144), Rule(172,0)};
    vector<uint8_t> rulesTable;
    // Convert the Rule(value, mask) to integer type in trie
    for (int i=0; i<list.size(); i++) {
        convert_rule(rulesTable, list.at(i));
    }

    // Initilize a trie
    Trie trie;

    // Insert all the rules in the rulesTable
    for (int i=0; i<rulesTable.size(); i++) {
        trie.insert_rule(rulesTable.at(i));
    }

    char output[][32] = {"Not present in trie", "Present in trie"};

    // Search the rules
    cout << output[trie.search_rule(230)] << endl;
    cout << output[trie.search_rule(202)] << endl;
    cout << output[trie.search_rule(172)] << endl;
    cout << output[trie.search_rule(118)] << endl;

    // Delete rules
    // object.member function
    trie.delete_rule(rulesTable.at(1));

    cout << output[trie.search_rule(230)] << endl;
    cout << output[trie.search_rule(128)] << endl;

    return 0;

}
