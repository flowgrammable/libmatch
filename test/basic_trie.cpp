// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>

// Matches integer type


int main()
{
    vector<Rule> list = {Rule(102,144), Rule(40,195), Rule(54,8), Rule(54,1), Rule(172,0)};
    vector<uint8_t> rulesTable;
    // Convert the Rule(value, mask) to integer type in trie
    for (int i=0; i<list.size(); i++) {
        Trie::convert_rule(vector<uint8_t> rulesTable, list.at(i));
    }

    Trie trie;

    // Initilize the trie
    Trie::init_trie(&trie);

    // Insert all the rules in the list
    for (int i=0; i<rulesTable.size(); i++)
    {
        Trie::insert_rule(&trie, rulesTable.at(i));
    }

    char output[][32] = {"Not present in trie", "Present in trie"};

    // Search the rules
    cout << output[Trie::search_rule(&trie, 230)] << endl;
    cout << output[Trie::search_rule(&trie, 202)] << endl;
    cout << output[Trie::search_rule(&trie, 172)] << endl;
    cout << output[Trie::search_rule(&trie, 192)] << endl;


    // Delete rules

    Trie::delete_rule(&trie, rulesTable.at(1));


    cout << output[Trie::search_rule(&trie, 30)] << endl;
    cout << output[Trie::search_rule(&trie, 128)] << endl;


    return 0;

}
