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


  rule.value = stoul(substr1);
  rule.mask = stoul(substr2);

  return rule;
}


int main(int argc, char* argv[])
{
  string line;
  Rule rule;
  ifstream file (argv[1]);

  // Create a new vector
  vector<Rule> rulesTable;
  if (file.is_open()) {
    while (!file.eof()) {
      // Read lines as long as the file is
      getline(file,line);
      if(!line.empty()) {
        rule = strTint(line);
        // Push the input file into ruleArray
        rulesTable.push_back(rule);
      }
    }
  }
  file.close();

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

  for (int i=0; i<rulesTable.size(); i++) {
    linearTable::insert_rule(rulesTable, rulesTable.at(i));
  }

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << "Begin test (keys=" << keyTable.size() <<
          ", rules=" << rulesTable.size() << "):" << endl;

  int sumPresent = 0;
  auto start = get_time::now(); //use auto keyword to minimize typing strokes :)
  //get time1
  for (int j=0; j<keyTable.size(); j++) {
    bool present = linearTable::search_rule(rulesTable, keyTable[j]);
    //cout << output[] << endl;
    sumPresent += present;
  }
  auto end = get_time::now();
  auto diff = end - start;
  //get time2
  cout << "Total matches: " << sumPresent << endl;
  cout<<"Elapsed time is :  "<< chrono::duration_cast<ns>(diff).count()<<" ns "<<endl;



  return 0;
}

/*
int main()
{
  vector<Rule> list = {Rule(102,144), Rule(40,195), Rule(54,8), Rule(54,1), Rule(172,0)};
  vector<Rule> rulesTable;

  // Insert all the rules in the list
  for (int i=0; i<list.size(); i++) {
    linearTable::insert_rule(rulesTable, list.at(i));
  }

  char output[][32] = {"Not present in rulesTable", "Present in rulesTable"};

  // Search the rules
  cout << output[linearTable::search_rule(rulesTable, 230)] << endl;
  cout << output[linearTable::search_rule(rulesTable, 202)] << endl;
  cout << output[linearTable::search_rule(rulesTable, 172)] << endl;
  cout << output[linearTable::search_rule(rulesTable, 192)] << endl;

  // Delete rules
  linearTable::delete_rule(rulesTable, list.at(4));

  cout << output[linearTable::search_rule(rulesTable, 172)] << endl;

  return 0;

}
*/
