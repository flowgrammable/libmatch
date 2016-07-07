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
using  ns = chrono::nanoseconds;
using get_time = chrono::steady_clock ;

typedef std::pair<int,int> mypair;
/*
bool comparator ( const mypair& l, const mypair& r)
   { return l.first < r.first; }
// forgetting the syntax here but intent is clear enough
*/
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

/*
 * Detect the wildcard position in mask part
 * Detect bit "1" in mask
 * void detect_wildcard_mask(Rule& rule)
{
  vector<uint64_t> maskPositionArray; // store all the "1" bit position, start from 0
  // Check the mask field from the lower bit, total is 64bit, two fields
  for(int i = 0; i < 64; i++) {
    // Get an array with 0 and 1
    if((rule.mask >> i) & 1 == 1) {
      maskPositionArray.push_back(1);
    }
    else {
      maskPositionArray.push_back(0);
    }
  }
}
*/


/*
 * Find the intersection of each rule in rulesTable
 * FInd the maximal overlapping wildcard in mask
 * Return an array, a set of bit "1" position {4, 5, 6}
 * void findIntersection(vector<uint64_t>& maskArray)
{

}
*/



int main(int argc, char* argv[])
{
  ifstream file (argv[1]);

  // Read the rules from txt file
  vector<Rule> pingRulesTable;
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
        pingRulesTable.push_back(rule);
      }
    }
  }
  file.close();

  cout << pingRulesTable.size() << endl;

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
  cout << keyTable.size() << endl;

  /*
   * Generate the two dimensional array (generate delta array) from the pingRulesTable array
   * With all the bits in each rule, including "0" and "1"
   * Caculate the score--the sum of column
  */
  vector<uint64_t> sumColumn;
  for (int j = 0; j < 64; j++) {
    for (i = 0; i < pingRulesTable.size(); i++) {
      sumColumn[j] = sumColumn[j] + ((pingRulesTable[i].mask >> j) & 1);
    }
  }
  // Copy the sumCOlumn vector to a new vector
  vector<uint64_t> newSumColumn(sumColumn);

  // Sort the newSumColumn vector in descending order
  std::sort(newSumColumn.begin(), newSumColumn.end(), std::greater<uint64_t>());

  // Construct the delta(): the rearrangement operation {left shift, right shift, and no change}
  // the element in delta vector, can be negative, positive and "0"
  vector<int> delta;
  // checkpoint is the index/subscript of the newSumColumn has the same value with the sumColumn
  int checkpoint = 0;
  for (int i = 0; i < sumColumn.size(); i++) {
    for (int j = 0; j < newSumColumn.size(); j++) {
      // Check the first equal element, if it is true, then return the checkpoint
      if (newSumColumn[j] == sumColumn[i]) {
        checkpoint = j;
      }
      else {
        cout << "Error occurs" << endl;
      }
    }
    // Get the difference between the original vector data and the sorted vector data
    // Create the rearrangement operation
    delta[i] = i - checkpoint;
  }

  /*
   * Generate the new rule after the delta operations
   * if the element value of delta vector is negative, which means left shift
   * if the element value of delta vector is positive, which means right shift
   * if the element value of delta vector is "0", which means no change
  */
  // Create a new pingRulesTable for the new rearrangement rules
  vector<Rule> newPingRulesTable;
  vector<Rule> sumRule;
  for (int i = 0; i < pingRulesTable.size(); i++) {
    for (int j = 0; j < 64; j++) {
      if (delta[j] < 0) {
        // if it is negative, do the left shift
        // from the lsb position, do the correct operations
        newPingRulesTable[i].mask = ( ( pingRulesTable[i].mask & (1 << i) ) << abs(delta[j]) );
        newPingRulesTable[i].value = ( ( pingRulesTable[i].value & (1 << i) ) << abs(delta[j]) );
      }
      else if (delta[j] > 0) {
        // if it is positive, do the right shift
        newPingRulesTable[i].mask = ( ( pingRulesTable[i].mask & (1 << i) ) >> abs(delta[j]) );
        newPingRulesTable[i].value = ( ( pingRulesTable[i].value & (1 << i) ) >> abs(delta[j]) );
      }
      else if (delta[j] == 0) {
        // if it is "0", no change
        newPingRulesTable[i].mask = pingRulesTable[i].mask;
        newPingRulesTable[i].value = pingRulesTable[i].value;
      }
      sumRule[i].mask =+ newPingRulesTable[i].mask;
      sumRule[i].value =+ newPingRulesTable[i].value;
    }
  }


  // Initilize a trie
  Trie trie;

  // insert rules into trie

  for (int k = 0; k < pingRulesTable.size(); k++) {
    trie.insert_rule(pingRulesTable.at(k));
    //trie.insert_prefix_rule_priority(pingRulesTable.at(k));

  }

  //void Trie::insert_rule(uint32_t value, uint32_t mask)

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << keyTable.size() <<
          ", rules=" << pingRulesTable.size() << "):" << endl;

  uint32_t checksum = 0;
  uint32_t match = 0;

  //get time1
  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  for (int j=0; j<keyTable.size(); j++) {
    uint32_t priority = trie.LPM1_search_rule(keyTable[j]);
    //cout << j << " " << present << endl;
    checksum += priority;
    match += (priority != 0);

  }

  //get time2
  auto end = get_time::now();
  auto diff = end - start;
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  cout<<"Elapsed time is :  "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;



  return 0;
}

