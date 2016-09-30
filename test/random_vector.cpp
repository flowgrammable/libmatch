// Copyright (c) 2015 Flowgrammable.org
// All rights reserved
/*
#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <algorithm>
#include <functional>

using namespace std;


// Matches integer type
Rule strTint(string rulestr)
{
  Rule rule;
  rule.mask = 0;
  rule.value = 0;
  // Find the position of ".", which spilit IP address to 4 parts
  size_t p1 = rulestr.find_first_of(" ");

  string substr1 = rulestr.substr(0,p1);
  string substr2 = rulestr.substr((p1 + 1));


  rule.value = stoull(substr1); // unsigned long long 64bit
  rule.mask = stoull(substr2);

  return rule;
}

int main(int argc, char* argv[])
{
  ifstream file (argv[1]);
  // Read the rules from txt file
  vector<Rule> oldpingRulesTable;
  int i = 0;
  if (file.is_open()) {
    while (!file.eof()) {
      // Read lines as long as the file is
      string line;
      getline(file,line);
      if(!line.empty()) {
        Rule rule = strTint(line);
        // Add the priority feature when generating rules
        // Priority is in-order of generating
        rule.priority = ++i;
        // Push the input file into ruleArray
        oldpingRulesTable.push_back(rule);
      }
    }
  }
  file.close();
  // Get the original table
  vector<Rule> pingRulesTable;

  for (int i = 1682; i >= 0; i --) {
    // Get the random index from the table
    int rand_index = rand() % oldpingRulesTable.size();
    // Choose the random index rule and push into the new smaller vector
    pingRulesTable.push_back(oldpingRulesTable[rand_index]);
    // Make sure not choose the same element
    oldpingRulesTable.erase( oldpingRulesTable.begin() + rand_index );
  }

  cout << "Selected total size = " << pingRulesTable.size() << endl;
  //vector<Rule> pingRulesTable = merge_rules(oldpingRulesTable);
  //cout << "Merged total size = " << pingRulesTable.size() << endl;

  for (int k = 0; k < pingRulesTable.size(); k++) {
    cout << pingRulesTable[k].value << " " << pingRulesTable[k].mask << endl;
  }

  // Read in keys from file:
  ifstream file1 (argv[2]);
  vector<uint64_t> keyTable;
  if (file1.is_open()) {
    while (!file1.eof()) {
      // Read lines as long as the file is
      string packet;
      getline(file1,packet);
      if(!packet.empty()) {
        uint64_t key = stoull(packet);
        // Push the input file into ruleArray
        keyTable.push_back(key);
      }
    }
  }
  file1.close();
}
*/

// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/basic_trie.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <algorithm>
#include <functional>

using namespace std;


// Matches integer type
Rule strTint(string rulestr)
{
  Rule rule;
  rule.mask = 0;
  rule.value = 0;
  // Find the position of ".", which spilit IP address to 4 parts
  size_t p1 = rulestr.find_first_of(" ");

  string substr1 = rulestr.substr(0,p1);
  string substr2 = rulestr.substr((p1 + 1));


  rule.value = stoull(substr1); // unsigned long long 64bit
  rule.mask = stoull(substr2);

  return rule;
}

int main(int argc, char* argv[])
{
  ifstream file (argv[1]);
  // Read the rules from txt file
  vector<Rule> oldpingRulesTable;
  int i = 0;
  if (file.is_open()) {
    while (!file.eof()) {
      // Read lines as long as the file is
      string line;
      getline(file,line);
      if(!line.empty()) {
        Rule rule = strTint(line);
        // Add the priority feature when generating rules
        // Priority is in-order of generating
        rule.priority = ++i;
        // Push the input file into ruleArray
        oldpingRulesTable.push_back(rule);
      }
    }
  }
  file.close();

  // Read in keys from file:
  ifstream file1 (argv[2]);
  vector<uint64_t> keyTable;
  if (file1.is_open()) {
    while (!file1.eof()) {
      // Read lines as long as the file is
      string packet;
      getline(file1,packet);
      if(!packet.empty()) {
        uint64_t key = stoull(packet);
        // Push the input file into ruleArray
        keyTable.push_back(key);
      }
    }
  }
  file1.close();

  // Start to generate different size of keys
  vector<uint64_t> keyTable1;
  vector<uint64_t> keyTable2;
  vector<uint64_t> keyTable3;
  for (int i = 0; i < 176420; i++) {
    keyTable1.push_back(keyTable[i]);
    // Delete the key one by one
    //keyTable.erase(keyTable.begin());
  }
  cout << "keyTable1 size: " << keyTable1.size() << endl;

  for (int j = 0; j < 1764200; j++) {
    keyTable2.push_back(keyTable[j]);
  }
  cout << "keyTable2 size: " << keyTable2.size() << endl;

  for (int k = 0; k < 17642000; k++) {
    keyTable3.push_back(keyTable[k]);
  }
  cout << "keyTable2 size: " << keyTable2.size() << endl;
}
