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
using  ms = chrono::microseconds;
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

// Sorting the rules in an asscending order
bool wayToSort(Rule aa, Rule bb)
{
  return (aa.mask < bb.mask);
}

bool wayToSort1(Rule aaa, Rule bbb)
{
  return (aaa.value < bbb.value);
}

/*
 * Sorting the prefix format rules into asscending order, denpending on the prefix length
 * using the std::sort function
*/
vector<Rule> sort_rules(vector<Rule>& ruleList)
{
  std::sort(ruleList.begin(), ruleList.end(), wayToSort);
  /*
  cout << "mark" << "===============" << endl;
  for (int k = 0; k < ruleList.size(); k ++) {
    cout << ruleList[k].value << ", " << ruleList[k].mask << endl;
  }
  */
  vector<Rule> sortTable;
  vector<Rule> sortTotalTable;
  // Determine the size of combined table
  sortTotalTable.reserve(ruleList.size());
  for (int i = 0; i < ruleList.size(); i ++) {
    if (i != ruleList.size() - 1) {
      if (ruleList.at(i).mask == ruleList.at(i+1).mask) {
        // if the mask value is the same, push into the same vector
        //cout << "test" << endl;
        sortTable.push_back(ruleList.at(i));
        continue;
      }
      else {
        sortTable.push_back(ruleList.at(i));
        //cout << "i = " << i << endl;
        std::sort(sortTable.begin(), sortTable.end(), wayToSort1);
        sortTotalTable.insert( sortTotalTable.end(), sortTable.begin(), sortTable.end() );
        /*
        for (int k = 0; k < sortTotalTable.size(); k ++) {
          cout << sortTotalTable[k].value << ", " << sortTotalTable[k].mask << endl;
        }
        cout << "sortTotalTable size = " << sortTotalTable.size() << endl;
        */
        // Delete the current contents, clear the memory
        sortTable.clear();
        //cout << "sortTable size = " << sortTable.size() << endl;
        continue;
      }
    }
    else {
      // for the last element in the vector
      // for avoiding the over-range of the vector
      if (ruleList.at(i).mask == ruleList.at(i-1).mask) {
        sortTable.push_back(ruleList.at(i));
        //cout << "i = " << i << endl;
        std::sort(sortTable.begin(), sortTable.end(), wayToSort1);
        sortTotalTable.insert( sortTotalTable.end(), sortTable.begin(), sortTable.end() );
      }
      else {
        std::sort(sortTable.begin(), sortTable.end(), wayToSort1);
        sortTotalTable.insert( sortTotalTable.end(), sortTable.begin(), sortTable.end() );
        sortTable.clear();
        sortTable.push_back(ruleList.at(i));
        sortTotalTable.insert( sortTotalTable.end(), sortTable.begin(), sortTable.end() );
      }
    }
  }
  return sortTotalTable;
}


int main(int argc, char* argv[])
{
  string line;
  Rule rule;
  ifstream file (argv[1]);

  // Read in rules from file:
  vector<Rule> oldinputRules;
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
        rule.action = rand() % 50 + 1; // randomly choose from [1,50]
        // Push the input file into ruleArray
        oldinputRules.push_back(rule);
      }
    }
  }
  file.close();

 for (int k = 0; k < oldinputRules.size(); k++) {
   cout << oldinputRules[k].action << endl;
 }

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




  return 0;
}



