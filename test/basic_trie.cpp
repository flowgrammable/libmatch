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

  //cout << pingRulesTable.size() << endl;

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
  //cout << keyTable.size() << endl;

  /*
   * Generate the two dimensional array (generate delta array) from the pingRulesTable array
   * With all the bits in each rule, including "0" and "1"
   * Caculate the score--the sum of column
  */
  vector<uint64_t> sumColumn;
  for (int j = 0; j < 64; j++) {
    uint32_t score = 0;
    for (i = 0; i < pingRulesTable.size(); i++) {
      //cout << "mask score is" << " " << ((((pingRulesTable.at(i)).mask) >> j) & 1) << endl;
      score += ((((pingRulesTable.at(i)).mask) >> j) & 1);
    }
    //cout << "score is" << " " << score << endl;
    sumColumn.push_back(score);
    //cout << sumColumn[j] << endl;
  }
   //cout << sumColumn.size() << endl;

  // Copy the sumCOlumn vector to a new vector
  vector<uint64_t> newSumColumn(sumColumn);
  /*
   * Checked the newSumColumn vector is the same with the sumColumn vector
  cout << newSumColumn.size() << endl;

  for (i = 0; i < newSumColumn.size(); i++) {
    cout << newSumColumn[i] << endl;
  }
  */

  // Sort the newSumColumn vector in descending order
  std::sort(newSumColumn.begin(), newSumColumn.end(), std::greater<uint64_t>());
  /*
   * Checked the descending order is correct or not
  for (i = 0; i < newSumColumn.size(); i++) {
    cout << newSumColumn[i] << endl;
  }
  */

  // Construct the delta(): the rearrangement operation {left shift, right shift, and no change}
  // the element in delta vector, can be negative, positive and "0"
  vector<int> delta;
  // checkpoint is the index/subscript of the newSumColumn has the same value with the sumColumn
  uint32_t checkpoint = 0;
  int gap = 0; // Gap is the difference between the original and new sumcolumn vectors
  for (int i = 0; i < sumColumn.size(); i++) {
    //cout << "mark1" << " " << sumColumn[i] << endl;
    for (int j = 0; j < newSumColumn.size(); j++) {
      //cout << newSumColumn[j] << endl;
      // Check the first equal element, if it is true, then return the checkpoint
      if (newSumColumn[j] != sumColumn[i]) {
        continue;
      }
      else if (newSumColumn[j] == sumColumn[i]) {
        checkpoint = j;
        newSumColumn[j] = 132; // make the matched column invalid
        break;
      }
      else {
        // Search all the 64 values, still didn't find the same value
        cout << "Error occurs" << endl;
      }
    }
    // Get the difference between the original vector data and the sorted vector data
    // Create the rearrangement operation
    gap = i - checkpoint;
    //cout << "checkpoint is" << " " << checkpoint << endl;
    //cout << "gap is" << " " << gap << endl;
    delta.push_back(gap);
  }

  /*
   *Checked the delta vector

  for (i = 0; i < delta.size(); i++) {
    cout << "i=" << i  << " " << delta[i] << endl;
  }
  */


  /*
   * Generate the new rule after the delta operations
   * if the element value of delta vector is negative, which means left shift
   * if the element value of delta vector is positive, which means right shift
   * if the element value of delta vector is "0", which means no change
  */
  // Create a new pingRulesTable for the new rearrangement rules
  vector<Rule> newPingRulesTable; // for each bit of each rule
  vector<Rule> sumRulesTable; // for a new rule
  Rule subRule; // for each bit of each rule
  Rule newRule; // for a new rule

  for (int i = 0; i < pingRulesTable.size(); i++) {
    for (int j = 0; j < 64; j++) {
      if (delta[j] < 0) {
        // if it is negative, do the left shift
        // from the lsb position, do the correct operations
        // Note: because here is 64 bit, if we just use 1 to do left shift, it will occur overflow
        // since "1" is 32bit by default
        subRule.value = ( (( (pingRulesTable[i].value) & (uint64_t(1) << j) ) ) << (abs(delta[j])) );
        subRule.mask = ( (( (pingRulesTable[i].mask) & (uint64_t(1) << j) ) ) << (abs(delta[j])) );
      }
      else if (delta[j] > 0) {
        // if it is positive, do the right shift
        subRule.value = ( (( (pingRulesTable[i].value) & (uint64_t(1) << j) ) ) >> (abs(delta[j])) );
        subRule.mask = ( (( (pingRulesTable[i].mask) & (uint64_t(1) << j) ) ) >> (abs(delta[j])) );
      }
      else if (delta[j] == 0) {
        // if it is "0", no change
        subRule.value = (( (pingRulesTable[i].value) & (uint64_t(1) << j) ) );
        subRule.mask = (( (pingRulesTable[i].mask) & (uint64_t(1) << j) ) );
      }
      newRule.value |= subRule.value;
      newRule.mask |= subRule.mask;
      newRule.priority = pingRulesTable[i].priority;
    }
    sumRulesTable.push_back(newRule);
  }

  /*
   * // Check the rearranged new rules ( has the same size with the original rules = 131 )
  for (i = 0; i < sumRulesTable.size(); i++) {
    cout << sumRulesTable[i].value << " " << sumRulesTable[i].mask << " " << sumRulesTable[i].priority << endl;
  }
  */


  /*
   * We get the new rearrangement rules table here, named sumRulesTabel
   * Next, we will do the rules insertion
   * Here, we just insert prefix rules, follow the LPM insertion function
   * So we need to check whether each new rules is prefix rule
   * If it is, then do the insertion
   * if not, do the expansion algorithm to make it is prefix rule
  */

  // Initilize a trie
  Trie trie;

  // insert new modified rules into trie (after rearrangement algorithm)

  for (int k = 0; k < sumRulesTable.size(); k++) {
    trie.is_prefix(sumRulesTable.at(k));
  }

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << keyTable.size() <<
          ", rules=" << sumRulesTable.size() << "):" << endl;

  uint32_t checksum = 0; // show the sum of matching priority
  uint32_t match = 0; // how many keys are being matched in these new rules

  //get time1
  auto start = get_time::now(); // use auto keyword to minimize typing strokes :)
  for (int j=0; j<keyTable.size(); j++) {
    uint32_t priority = trie.LPM1_search_rule(keyTable[j]);
    //cout << j << " " << present << endl;
    checksum += priority;
    match += (priority != 0); // when priority == 0, which means no matching
  }

  //get time2
  auto end = get_time::now();
  auto diff = end - start;
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  cout<<"Elapsed time is :  "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;

  return 0;
}

