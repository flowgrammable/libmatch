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



/*
 * Check whether the neighbouring rules are cross pattern (has intersection)
 * if yes, continue the rules
 * if not, spearate the group
*/
bool is_cross_pattern(Rule const& a, Rule const& b)
{
  if (a.mask && b.mask == 0) {
    return false;
  }
  else {
    return true;
  }
}


/*
 * Check whether the new rule is a prefix rule
 * if yes, do the LPM insertion
 * if not, do the expand rule function
*/
bool is_prefix(Rule& rule)
{
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
    return true;
  }
  else {
    return false;
  }
}

/*
 * Generate the two dimensional array (generate delta array) from the pingRulesTable array
 * With all the bits in each rule, including "0" and "1"
 * Caculate the score--the sum of column
*/
vector<int> generate_delta(vector<Rule>& ruleList)
{
  vector<uint64_t> sumColumn;
  for (int j = 0; j < 64; j++) {
    uint32_t score = 0;
    for (int i = 0; i < ruleList.size(); i++) {
      score += ((((ruleList.at(i)).mask) >> j) & 1);
    }
    sumColumn.push_back(score);
  }

  // Copy the sumColumn vector to a new vector
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
  return delta;
  /*
   *Checked the delta vector
  for (i = 0; i < delta.size(); i++) {
    cout << "i=" << i  << " " << delta[i] << endl;
  }
  */
}

/*
 * Generate the new rule after the delta operations
 * if the element value of delta vector is negative, which means left shift
 * if the element value of delta vector is positive, which means right shift
 * if the element value of delta vector is "0", which means no change
*/
// Create a new pingRulesTable for the new rearrangement rules
//vector<Rule> newPingRulesTable; // for each bit of each rule
vector<Rule> rules_rearrange(vector<Rule>& oldRuleList, vector<int> delta_array)
{
  vector<Rule> sumRulesTable; // for a new rule

  for (int i = 0; i < oldRuleList.size(); i++) {
    Rule newRule;
    for (int j = 0; j < 64; j++) {
      Rule subRule;
      if (delta_array[j] < 0) {
        // if it is negative, do the left shift
        // from the lsb position, do the correct operations
        // Note: because here is 64 bit, if we just use 1 to do left shift, it will occur overflow
        // since "1" is 32bit by default
        subRule.value = ( (( (oldRuleList[i].value) & (uint64_t(1) << j) ) ) << (abs(delta_array[j])) );
        subRule.mask = ( (( (oldRuleList[i].mask) & (uint64_t(1) << j) ) ) << (abs(delta_array[j])) );
      }
      else if (delta_array[j] > 0) {
        // if it is positive, do the right shift
        subRule.value = ( (( (oldRuleList[i].value) & (uint64_t(1) << j) ) ) >> (abs(delta_array[j])) );
        subRule.mask = ( (( (oldRuleList[i].mask) & (uint64_t(1) << j) ) ) >> (abs(delta_array[j])) );
      }
      else if (delta_array[j] == 0) {
        // if it is "0", no change
        subRule.value = (( (oldRuleList[i].value) & (uint64_t(1) << j) ) );
        subRule.mask = (( (oldRuleList[i].mask) & (uint64_t(1) << j) ) );
      }
      newRule.value |= subRule.value;
      newRule.mask |= subRule.mask;
      //cout << j << " " << newRule.mask << endl;
      newRule.priority = oldRuleList[i].priority;
    }
    sumRulesTable.push_back(newRule);
  }
  return sumRulesTable;
}

/*
 * Rearrange each key in the keyTable according to the delta vector
 * because the rules are being reordered by the delta vector
*/

uint64_t keys_rearrange(uint64_t key, vector<int> delta_array)
{
  // Calculate the key rearrangement configure time basing on the delta vector

  uint64_t newKey = 0; // new key after reordering
  for (int j = 0; j < 64; j++) {
    uint64_t subKey = 0; // subKey is the single bit value of each key
    if (delta_array[j] < 0) {
      subKey = ( (( key & (uint64_t(1) << j) ) ) << (abs(delta_array[j])) );
    }
    else if (delta_array[j] > 0) {
      // if it is positive, do the right shift
      subKey = ( (( key & (uint64_t(1) << j) ) ) >> (abs(delta_array[j])) );
    }
    else if (delta_array[j] == 0) {
      // if it is "0", no change
      subKey = (( key & (uint64_t(1) << j) ) );
    }
    newKey |= subKey;
  }
  return newKey;
  //cout << newKey << endl;
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
   * Grouping algorithm
   * Use the is_cross_pattern function to check the grouping number
  */


  vector<uint32_t> groupVector;

  for (int i = 0; i < pingRulesTable.size(); i++) {
    if (is_cross_pattern( pingRulesTable[i], pingRulesTable[i+1] )) {
      continue;
    }
    else {
      groupVector.push_back(i);
      continue;
    }
  }
  cout << "Group num is:" << " " << groupVector.size() << endl;

  /* Create all the subgroups
   * The big array is called bigArray
  */
  vector< vector<Rule> > bigArray;
  // Create a new sub group by copying the related rules
  for (int j = 0; j < groupVector.size(); j++) {
    bigArray.push_back(vector<Rule> ());
  }

  for (int j = 0; j < groupVector.size(); j++) {
    if (j == 0) {
      for (int i = 0; i < (groupVector[j] + 1); i++) {
        bigArray[j].push_back(pingRulesTable.at(i));
      }
      continue;
    }
    else {
      for (int k = (groupVector[j-1] + 1); k < (groupVector[j] + 1); k++) {
        bigArray[j].push_back(pingRulesTable.at(k));
      }
      continue;
    }
  }

  for (int j = 0; j < groupVector.size(); j++) {
   cout << bigArray[j].size() << " " << groupVector[j] + 1 << " " << "check is the same or not" << endl;
  }

  // Start to calculate the rearrangement configure time
  // including the delta vector time

  //auto start_1 = get_time::now();


  // Start to build the newRules in each group
  /*
   * We get the new rearrangement rules table here, named sumRulesTabel
   * Next, we will do the rules insertion
   * Here, we just insert prefix rules, follow the LPM insertion function
   * So we need to check whether each new rules is prefix rule
   * If it is, then do the insertion
   * if not, do the expansion algorithm to make it is prefix rule
  */

  int expandRule_num = 0;
  int insertRule_num = 0;
  int deleteRule_num = 0;

  uint64_t checksum = 0; // show the sum of matching priority
  uint64_t match = 0; // how many keys are being matched in these new rules

  uint64_t sum_trie_expand_count = 0;
  uint64_t sum_trie_count = 0;
  uint64_t sum_trie_node_count = 0;

  //get time1
  auto start = get_time::now(); // use auto keyword to minimize typing strokes :)


  // Start to search each key here
  for (int j = 0; j < groupVector.size(); j++) {
    // Initilize a trie
    // Each group is a seperate trie
    Trie trie;
    vector<int> delta_need = generate_delta(bigArray[j]);
    vector<Rule> newSumRuleTable = rules_rearrange(bigArray[j], delta_need);
    // Doing the rule insertion
    for (int k = 0; k < newSumRuleTable.size(); k++) {
      if ( is_prefix(newSumRuleTable.at(k)) ) {
        trie.insert_prefix_rule_priority(newSumRuleTable.at(k));
        insertRule_num ++;
      }
      else if ( trie.get_new_num(newSumRuleTable.at(k)) < 12 ) {
        trie.expand_rule(newSumRuleTable.at(k));
        expandRule_num ++;
      }
      else {
        pingRulesTable[k].value = 0;
        pingRulesTable[k].mask = 0;
        cout << pingRulesTable[k].value << "test" << endl;
        deleteRule_num ++;
        continue;
      }
    }
    // Finished the rearranged rule insertion for each subtrie
    // Doing the rule searching
    char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

    // Search the rules
    cout << "Begin test (keys=" << keyTable.size() <<
            ", rules=" << pingRulesTable.size() << "):" << endl;

    for (int i = 0; i < keyTable.size(); i++) {
      // Check each key
      keys_rearrange(keyTable[i], delta_need);
      uint64_t priority = trie.LPM1_search_rule(keyTable[i]);
      match += (priority != 0); // when priority == 0, which means no matching
      if (priority == 0) {
        continue;
      }
      else {
        checksum += priority;
        // Delete the matched keys from the keyTable
        // Avoid the duplication searching
        // Delete the element keyTable[i]
        keyTable.erase(keyTable.begin() + i);
        continue;
      }
    }
    sum_trie_expand_count += trie.expand_count;
    sum_trie_count += trie.count;
    sum_trie_node_count += trie.node_count;
  }

  //get time2
  auto end = get_time::now();
  auto diff = end - start;



  //auto end_1 = get_time::now();
  //auto diff_1 = end_1 - start_1;
  //cout<<"Rules rearrangement configure time is:  "<< chrono::duration_cast<ns>(diff_1).count()<<" ns "<<endl;


  // auto end_3 = get_time::now();
  //auto diff_3 = end_3 - start_3;
  //cout<<"Rules insertion configure time is:  "<< chrono::duration_cast<ns>(diff_3).count()<<" ns "<<endl;
  cout << "Total expanded count is:" << " " << sum_trie_expand_count << endl;
  cout << "Expand rule num is:" << " " << expandRule_num << endl;
  cout << "Insert rule num is:" << " " << insertRule_num << endl;
  cout << "Delete rule num is:" << " " << deleteRule_num << endl;
  cout << "Total insert rule num is:" << " " << sum_trie_count << endl;
  cout << "Total insert trie_node count is:" << " " << sum_trie_node_count << endl;
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  cout <<"Search time is: "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;

  return 0;
}

