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
   * Avoid the expanding number is too large
   * how to improve the grouping algorithm??
   * add the threshold, to adjust the grouping seperation
  */

  // For the grouped rule table
  vector<uint32_t> groupVector;
  vector<Rule> newList;

  // avoid the bad allocation memory

  uint32_t list_count = 0;
  for ( int i = 0; i < pingRulesTable.size(); i++ ) {
    if (i < (pingRulesTable.size()-1)) {
      newList.push_back(pingRulesTable[i]);
      vector<int> new_generated_delta = generate_delta(newList);
      // Create the rearranged rule set
      vector<Rule> new_table_list = rules_rearrange(
            newList, new_generated_delta );
      for ( int k = 0; k < new_table_list.size(); k++ ) {
        // check each rule in new group
        // guarantee there has no rules expanding too much and cause bad_alloc
        // 20: if bigger than 20, occur bad allocation, did test
        // related to the i value and k value
        Trie trie1; // for caculating the trie1.new_num
        /*
        cout << " size of each group:" << " " << newList.size() << endl;
        cout << list_count << "check the rules in each group" << endl;
        if (trie1.get_new_num(new_table_list.at(k)) >20) {
          cout << "k is:" << k << " " << "first Num of wildcard is:" << " " << trie1.get_new_num(new_table_list.at(k)) << endl;
        }
        */
        if (trie1.get_new_num( new_table_list.at (k) ) < 20) {
          continue;
        }

        else if (new_table_list.size() == 1) {
          groupVector.push_back(list_count);
          list_count += 1;
          newList.clear();
          break;
        }
        else if ((new_table_list.size() > 1)) {
          //cout << "push back" << list_count + (k-1) << endl;
          if (k == 0) {
            groupVector.push_back(list_count);
            list_count += 1;
            newList.clear();
            break;
          }
          else {
            groupVector.push_back(list_count + (k-1));
            // clear the newList vector, becasue this is a seperated group
            list_count += k;
            newList.clear();
            break;
          }
        }
      }
    }
    else {
      groupVector.push_back(i);
    }
  }

  cout << "Num of groups is:" << " " << groupVector.size() << endl;




  /* Create all the subgroups
   * The big array is called bigArray
  */
  vector< vector<Rule> > bigArray;
  // Create a new sub group by copying the related rules

  for (int m = 0; m < groupVector.size(); m++) {
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
    cout << "Group" << j+1 << " " <<  bigArray[j].size() << endl;
  }


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
  //int deleteRule_num = 0;

  uint64_t checksum = 0; // show the sum of matching priority
  uint64_t match = 0; // how many keys are being matched in these new rules

  uint64_t sum_trie_expand_count = 0;
  uint64_t sum_trie_count = 0;
  uint64_t sum_trie_node_count = 0;
  uint64_t sum_rule_rearrange_time = 0;
  uint64_t sum_rule_insertion_time = 0;
  uint64_t sum_key_rearrange_time = 0;
  uint64_t sum_key_search_time = 0;
  //get time1
  //auto start = get_time::now(); // use auto keyword to minimize typing strokes :)


  // Start to search each key here
  for (int j = 0; j < groupVector.size(); j++) {
    // Initilize a trie
    // Each group is a seperate trie
    Trie trie;
    auto start1 = get_time::now();
    vector<int> delta_need = generate_delta(bigArray[j]);
    cout << bigArray[j].size() << "size of each array" << endl;
    vector<Rule> newSumRuleTable = rules_rearrange(bigArray[j], delta_need);
    auto end1 = get_time::now();
    auto diff1 = end1 - start1;
    sum_rule_rearrange_time += chrono::duration_cast<ns>(diff1).count();
    /*
     * Checked the num of groups: new rules
    for (int k = 0; k < newSumRuleTable.size(); k++) {
      cout << newSumRuleTable[k].value << " " << newSumRuleTable[k].mask
           << " " << newSumRuleTable[k].priority << endl;
    }
    */
    // Doing the rule insertion
    auto start2 = get_time::now();
    for (int k = 0; k < newSumRuleTable.size(); k++) {
      cout << newSumRuleTable.size() << " " << "size" << endl;
      //cout << "Num of wildcard is:" << " " << trie.get_new_num(newSumRuleTable.at(k)) << endl;
      if ( is_prefix(newSumRuleTable.at(k)) ) {
        trie.insert_prefix_rule_priority(newSumRuleTable.at(k));
        insertRule_num ++;
        //cout << "test" << endl;
      }
      else {
        cout << "test test" << endl;
        cout << "j=" << j << " "<< "k=" << k << " " << trie.get_new_num(newSumRuleTable.at(k)) << endl;
        trie.expand_rule(newSumRuleTable.at(k));

        expandRule_num ++;
      }
    }
    auto end2 = get_time::now();
    auto diff2 = end2 - start2;
    sum_rule_insertion_time += chrono::duration_cast<ns>(diff2).count();
    // Finished the rearranged rule insertion for each subtrie
    // Doing the rule searching
    char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

    // Search the rules
    /*
    cout << "Begin test (keys=" << keyTable.size() <<
            ", sub rules=" << newSumRuleTable.size() << "):" << endl;
    */
    for (int i = 0; i < keyTable.size(); i++) {
      // Check each key
      auto start3 = get_time::now();
      uint64_t newGenKey = keys_rearrange(keyTable[i], delta_need);
      auto end3 = get_time::now();
      auto diff3 = end3 - start3;
      sum_key_rearrange_time += chrono::duration_cast<ns>(diff3).count();
      auto start4 = get_time::now();
      uint64_t priority = trie.LPM1_search_rule(newGenKey);
      auto end4 = get_time::now();
      auto diff4 = end4 - start4;
      sum_key_search_time += chrono::duration_cast<ns>(diff4).count();
      match += (priority != 0); // when priority == 0, which means no matching
      if (priority == 0) {
        // not matching
        // Go to check the next keys through this current group of rules
        continue;
      }
      else {
        checksum += priority;
        // Delete the matched keys from the keyTable
        // Avoid the duplication searching
        // Delete the element keyTable[i]
        keyTable.erase(keyTable.begin() + i);
        // Make sure the index is correct
        // Because we delete one key, so need to decrease index too
        i = i - 1;
        continue;
      }
    }
    sum_trie_expand_count += trie.expand_count;
    sum_trie_count += trie.count;
    sum_trie_node_count += trie.node_count;
  }

  //get time2
  //auto end = get_time::now();
  //auto diff = end - start;

  cout << "Total rules rearrange configure time is:" << sum_rule_rearrange_time << " ns " << endl;
  cout << "Total rules insertion configure time is:" << sum_rule_insertion_time << " ns " << endl;
  cout << "Total keys rearrange configure time is:" << sum_key_rearrange_time << " ns " << endl;
  cout << "Total keys search time is:" << sum_key_search_time << " ns " << endl;
  cout << "Total expanded count is:" << " " << sum_trie_expand_count << endl;
  cout << "Expand rule num is:" << " " << expandRule_num << endl;
  cout << "Insert rule num is:" << " " << insertRule_num << endl;
  //cout << "Delete rule num is:" << " " << deleteRule_num << endl;
  cout << "Total insert rule num is:" << " " << sum_trie_count << endl;
  cout << "Total insert trie_node count is:" << " " << sum_trie_node_count << endl;
  cout << "Checksum: " << checksum << endl;
  cout << "Total matches: " << match << endl;
  //cout << "Total runtime is: "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;

  return 0;
}

