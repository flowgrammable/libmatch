/////////////////////////////////////

// File Name : wildcard matching

//////////////////////////////////////
#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>

#include "linear_arbitrary.hpp"


using namespace std;

/////////// Description ///////////////////////////////////////////////////////////////////////////////////////////////
// Program maps the arbitrary wildcard rules (includes '0', '1', and '*') into two different parts: bitMask, and wildMask.
// rule.value: parse the '*' and '0' into '0', the '1' into '1'.
// rule.mask: parse the '*' into '1', the '0' and '1' into '0'.
// ruleMatch: Bool (Incoming key && (~rule.mask) == rule.value), if true, then match, if false, don't match
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////// Input File Format ////////////////////////////////////////////////////////////////////////////////////////////
/* Rules  */
// *11*0110 drop
// **1010** port1
// 0011*110 port2
// 0011010* port3
// 10101100 port4

/* Incoming key */
// 01110110
// 10001010

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pass by reference, any changed made to the reference
// are passed through to the argument

void linearTable::insert_rule( Rule& rule )
{

  // Search table to see if rule exists:

  // Guarantee the value of rule is correct, didn't mess up by mask
  rule.value = rule.value & (~rule.mask);
  for (int i = 0; i < ruleTable.size(); i++) {
    if ( rule.value == ruleTable[i].value && rule.mask == ruleTable[i].mask ) {
      // Do something if it existed..
      return;
    }
  }

  /* Push the new rule into rules table  */

  ruleTable.push_back(rule);

}


uint32_t linearTable::search_rule( uint32_t key )
{
  for (int i = 0; i < ruleTable.size(); i++) {
    if (ruleTable[i].value == ( key & ( ~ruleTable[i].mask ) ) ) {
      return ruleTable[i].priority;
    }
  }

  // If no rules match:
  return 0;
}


void linearTable::delete_rule( Rule& rule )
{
  // Search table to see if rule exists:

  // Guarantee the value of rule is correct, didn't mess up by mask
  rule.value = rule.value & (~rule.mask);
  for (int i = 0; i < ruleTable.size(); i++) {
    if ( rule.value == ruleTable[i].value && rule.mask == ruleTable[i].mask ) {
      // delete the rule if it existed
      ruleTable.erase(ruleTable.begin()+i);
    }
  }

  // If didn't find the rule, return
  return;
}



