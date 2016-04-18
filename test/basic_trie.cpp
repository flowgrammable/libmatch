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

using namespace std;
using  ns = chrono::nanoseconds;
using get_time = chrono::steady_clock ;


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


  rule.value = stoul(substr1);
  rule.mask = stoul(substr2);

  return rule;
}


int main(int argc, char* argv[])
{
  string line;
  Rule rule;
  ifstream file (argv[1]);

  // Read the rules from txt file
  vector<Rule> pingRulesTable;
  if (file.is_open()) {
    while (!file.eof()) {
      // Read lines as long as the file is
      getline(file,line);
      if(!line.empty()) {
        rule = strTint(line);
        // Push the input file into ruleArray
        pingRulesTable.push_back(rule);
      }
    }
  }
  file.close();

  cout << pingRulesTable.size() << endl;

  // Get the vector interger vector<Rule>: pingRulesTable
  // With the format Rule(value, mask)
  // Need to use the convert function to convert the Rule(value, mask)
  // to vector<uint32_t>
/*
  vector<uint32_t> rulesTable;
  // Convert the Rule(value, mask) to integer type in trie
  for (int i=0; i<pingRulesTable.size(); i++) {
    convert_rule(rulesTable, pingRulesTable[i]);

    //cout << rulesTable.size() << endl;
  }
  //cout << rulesTable[0] << endl;

  cout << rulesTable.size() << endl;

*/
  string packet;
  uint32_t key;
  ifstream file1 (argv[2]);
  vector<uint32_t> keyTable;
  if (file1.is_open()) {
    while (!file1.eof()) {
      // Read lines as long as the file is
      getline(file1,packet);
      if(!packet.empty()) {
        key = stoul(packet);
        // Push the input file into ruleArray
        keyTable.push_back(key);
      }
    }
  }
  file1.close();
  cout << keyTable.size() << endl;

  // Initilize a trie
  Trie trie;

  // insert rules into trie

  for (int k = 0; k < pingRulesTable.size(); k++) {
    uint32_t value = pingRulesTable[k].value;
    uint32_t mask = pingRulesTable[k].mask;
    //trie.insert_rule(value, mask);
    trie.insert_prefix_rule(value, mask);
  }


  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << keyTable.size() <<
          ", rules=" << pingRulesTable.size() << "):" << endl;

  uint32_t sumPresent = 0;

  //get time1
  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  for (int j=0; j<keyTable.size(); j++) {
    bool present = trie.LPM1_search_rule(keyTable[j]);
    //cout << j << " " << present << endl;
    sumPresent += present;
    /*
    if (present) {
      sumPresent += 1;
    }
    */
  }

  //get time2
  auto end = get_time::now();
  auto diff = end - start;
  cout << "Total matches: " << sumPresent << endl;
  cout<<"Elapsed time is :  "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;



  return 0;
}

