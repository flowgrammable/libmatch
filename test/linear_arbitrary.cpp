// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/linear_arbitrary.cpp"

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


  rule.value = stoull(substr1);
  rule.mask = stoull(substr2);
  rule.priority = 0;

  return rule;
}


int main(int argc, char* argv[])
{
  string line;
  Rule rule;
  ifstream file (argv[1]);

  // Read in rules from file:
  vector<Rule> inputRules;
  int i = 0;
  if (file.is_open()) {
    while (!file.eof()) {
      // Read lines as long as the file is
      getline(file,line);
      if(!line.empty()) {
        rule = strTint(line);
        // Add the priority feature when generating rules
        // Priority is in-order of generating
        rule.priority = ++i;

        // Push the input file into ruleArray
        inputRules.push_back(rule);
      }
    }
  }
  file.close();

  // Read in keys from file:
  string packet;
  uint64_t key;
  ifstream file1 (argv[2]);
  vector<uint64_t> keyTable;
  if (file1.is_open()) {
    while (!file1.eof()) {
      // Read lines as long as the file is
      getline(file1,packet);
      if(!packet.empty()) {
        key = stoull(packet);
        // Push the input file into ruleArray
        keyTable.push_back(key);
      }
    }
  }
  file1.close();

  // Insert rules into linear arbitrary table:
  linearTable table;
  for (int i=0; i<inputRules.size(); i++) {
    table.insert_rule(inputRules.at(i));
  }

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << keyTable.size() <<
          ", rules=" << inputRules.size() << "):" << endl;

  uint64_t checksum = 0;
  uint64_t match = 0;


  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  //get time1
  for (int j=0; j<keyTable.size(); j++) {
    uint64_t priority = table.search_rule(keyTable[j]);
    //cout << present << endl;
    checksum += priority;
    match += (priority != 0);
  }
  auto end = get_time::now();
  auto diff = end - start;
  //get time2
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  cout<<"Search time is :  "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;



  return 0;
}


