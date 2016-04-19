#ifndef LINEAR_ARBITRARY_H
#define LINEAR_ARBITRARY_H


#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;


struct Rule
{
  uint32_t value;
  uint32_t mask;
  uint32_t priority;
  Rule()
  {
    value = 0;
    mask = 0;
    priority = 0;
  }

  Rule(uint32_t x, uint32_t y, uint32_t z)
  {
    value = x;
    mask = y;
    priority = z;
  }

};

class linearTable
{
private:
  vector<Rule> ruleTable;   // Empty vector of Rule struct
  int count; // The number of rules in the linearTable

public:
  // linearTable constructor
  linearTable()
  {
    count = 0;
  }

  ~linearTable()
  {
    // delete constructor
  }

  void insert_rule( Rule& rule );

  uint32_t search_rule( uint32_t key );

  void delete_rule( Rule& rule );


};





#endif // LINEAR_ARBITRARY_H
