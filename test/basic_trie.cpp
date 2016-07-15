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

// Initilize a trie
Trie trie;

void expand_rule( Rule& rule )
{
  //cout << " The priority is " << " " << rule.priority << endl;
  int boundary = 0;
  for (int i=0; i<64; i++) {
    // Find the first bit "0" from the least significant bit
    // 10x1xx, so the mask is 001011
    if ( ((rule.mask >> i) & 1) == 0 ) {
      boundary = i;
      break;
      //cout << "boundary is" << " " << boundary << endl;
    }
    else {
      continue;
    }
  }
  vector<uint32_t> maskNewPosition;
  for (int j=(boundary+1); j<64; j++) {
    if ( ((rule.mask >> j) & 1) == 1 ) {
      maskNewPosition.push_back(j); // recored the positions that should be expanded (the bit is "1")
      continue; // record all the candidates in 64-bit
    }
    else {
      continue; // check all the 64-bit
    }
  }
  // Get all the "1" in the new rule, besides the prefix part
  // need to expand all the "1" part
  uint32_t new_num = maskNewPosition.size(); // num is the number of wildcard
  //cout << new_num << endl;

   // check the bad_alloc threshold
   // want to delete the bad rules
  if (new_num > 12) {
    // delete the rules in sumRulesTable (bad rules cause bad_alloc)
    //pingRulesTable.erase(pingRulesTable.at(k));
    cout << " note return " << endl;
    return;
  }

  Rule expandedRule;
  vector<Rule> expandedPingRulesTable;

  // From the priority = 40 rule, starts having wildcard, not the prifix rules,
  // need to be expanded

  //cout << " mask is " << " " << rule.mask << " value is " << " " << rule.value
  //<< " priority is " << " " << rule.priority << endl;
  uint64_t expand_count = 0;
  for(uint64_t i = 0; i < ( uint64_t(1) << new_num ); i++) {

    expandedRule.value = rule.value; // base value
    // Show the last expandedRule value
    //cout << expandedRule.value << endl;
    // expand into the number of rules: depending on the number of wildcard
    //cout << " number of wildcard: " << " " << new_num << endl;
    for(int j = 0; j < new_num; j++) {
      if(((1 << j) & i) == 0) {
        // get the expandedRule for ruleTables
        expandedRule.value |= 0 * (uint64_t(1) << maskNewPosition.at(j));
        expandedRule.mask = ( uint64_t(1) << boundary ) - 1; // mask value should be a prefix value after expanded
        expandedRule.priority = rule.priority; // priority keeps the same
        //cout << " check the priority is " << " " << expandedRule.priority << endl;
      }
      else {
        // ((1 << j) & i) != 0
        expandedRule.value |= 1 * (uint64_t(1) << maskNewPosition.at(j));
        expandedRule.mask = ( uint64_t(1) << boundary ) - 1; // mask value should be a prefix value after expanded
        expandedRule.priority = rule.priority; // priority keeps the same
      }
      expandedPingRulesTable.push_back(expandedRule);
    }

    //cout << expandedRule.value << " " << expandedRule.mask << " " << expandedRule.priority << endl;
    trie.insert_prefix_rule_priority(expandedRule);
    expand_count++;
  }
  cout << expandedPingRulesTable.size() << endl; // check the expanded rule size, should be the power of 2

  cout << "expanded count:" << " " << expand_count << endl;
  //cout << "expanded rule size is" << " " << expandedPingRulesTable.size() << endl;
}

/*
 * Check whether the new rule is a prefix rule
 * if yes, do the LPM insertion
 * if not, do the expand rule function
*/
void is_prefix(Rule& rule)
{
  int expandRule_num = 0;
  int insertRule_num = 0;
  // Store the wildcard postion into vector maskPosion
  vector<uint32_t> maskPosition;
  // Check the mask field from the lower bit
  for(int i = 0; i < 64; i++) {
    // if this: get the position whose bit is 1 (have wildcard)
    if((rule.mask >> i) & 1 == 1) {
      maskPosition.push_back(i);
    }
  }
  uint32_t num = maskPosition.size(); // num is the number of wildcard
  if (rule.mask == (1 << num)-1) {
    trie.insert_prefix_rule_priority(rule);
    insertRule_num ++;
  }
  else {
    expand_rule(rule);
    expandRule_num ++;
  }
  cout << "expand rule num is:" << " " << expandRule_num << endl;
  cout << "insert rule num is:" << " " << insertRule_num << endl;
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

  // Start to calculate the rearrangement configure time
  // including the delta vector time

  auto start_1 = get_time::now();

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
  //vector<Rule> newPingRulesTable; // for each bit of each rule
  vector<Rule> sumRulesTable; // for a new rule

  for (int i = 0; i < pingRulesTable.size(); i++) {
    Rule newRule;
    for (int j = 0; j < 64; j++) {
      Rule subRule;
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
      //cout << j << " " << newRule.mask << endl;
      newRule.priority = pingRulesTable[i].priority;
    }
    sumRulesTable.push_back(newRule);
  }

  auto end_1 = get_time::now();
  auto diff_1 = end_1 - start_1;
  cout<<"Rules rearrangement configure time is:  "<< chrono::duration_cast<ns>(diff_1).count()<<" ns "<<endl;

  /*
    // Check the rearranged new rules ( has the same size with the original rules = 131 )
  for (i = 0; i < sumRulesTable.size(); i++) {
    cout << sumRulesTable[i].value << " " << sumRulesTable[i].mask << " " << sumRulesTable[i].priority << endl;
  }
  */


  /*
   * Rearrange the keyTable according to the delta vector
   * because the rules are being reordered by the delta vector
  */

  vector<uint64_t> newKeyTable; // for a new rule

  // Calculate the key rearrangement configure time basing on the delta vector
  auto start_2 = get_time::now();

  for (int i = 0; i < keyTable.size(); i++) {
    uint64_t newKey = 0; // new key after reordering
    for (int j = 0; j < 64; j++) {
      uint64_t subKey = 0; // subKey is the single bit value of each key
      if (delta[j] < 0) {
        subKey = ( (( (keyTable[i]) & (uint64_t(1) << j) ) ) << (abs(delta[j])) );
      }
      else if (delta[j] > 0) {
        // if it is positive, do the right shift
        subKey = ( (( (keyTable[i]) & (uint64_t(1) << j) ) ) >> (abs(delta[j])) );
      }
      else if (delta[j] == 0) {
        // if it is "0", no change
        subKey = (( (keyTable[i]) & (uint64_t(1) << j) ) );
      }
      newKey |= subKey;
    }
    newKeyTable.push_back(newKey);
  }

  auto end_2 = get_time::now();
  auto diff_2 = end_2 - start_2;
  cout<<"keys rearrangement configure time is:  "<< chrono::duration_cast<ns>(diff_2).count()<<" ns "<<endl;

  //cout << newKeyTable.size() << endl;
  /*
for (int k = 0; k < newKeyTable.size(); k++) {
  cout << newKeyTable[k] << endl;
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



  // insert new modified rules into trie (after rearrangement algorithm)

  // Calculate the insertion configure time
  // Inculduing the expanded configure time
  auto start_3 = get_time::now();

  for (int k = 0; k < sumRulesTable.size(); k++) {
    cout << "k is" << " " << k << endl; // check which rule has been expanded
    is_prefix(sumRulesTable.at(k));
  }

  auto end_3 = get_time::now();
  auto diff_3 = end_3 - start_3;
  cout<<"Rules insertion configure time is:  "<< chrono::duration_cast<ns>(diff_3).count()<<" ns "<<endl;

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << newKeyTable.size() <<
          ", rules=" << sumRulesTable.size() << "):" << endl;

  uint32_t checksum = 0; // show the sum of matching priority
  uint32_t match = 0; // how many keys are being matched in these new rules

  //get time1
  auto start = get_time::now(); // use auto keyword to minimize typing strokes :)
  for (int j=0; j<newKeyTable.size(); j++) {
    uint32_t priority = trie.LPM1_search_rule(newKeyTable[j]);
    //cout << j << " " << present << endl;
    checksum += priority;
    match += (priority != 0); // when priority == 0, which means no matching
  }

  //get time2
  auto end = get_time::now();
  auto diff = end - start;
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  cout<<"Elapsed time is: "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;

  return 0;
}

