// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#include "../libmatch/linear_arbitrary.cpp"

#include <iostream>
#include <vector>
#include <string>

// Matches integer type


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
