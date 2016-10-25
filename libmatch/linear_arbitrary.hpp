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
  uint64_t value;
  uint64_t mask;
  uint64_t priority;
  uint32_t action;
  Rule()
  {
    value = 0;
    mask = 0;
    priority = 0;
    action = 0;
  }

  Rule(uint64_t x, uint64_t y, uint64_t z, uint32_t w)
  {
    value = x;
    mask = y;
    priority = z;
    action = w;
  }

};

struct trie_result
{
  uint64_t priority;
  uint32_t action;

  trie_result()
  {
    priority = 0;
    action = 0;
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

  trie_result search_rule( uint64_t key );

  void delete_rule( Rule& rule );


};





#endif // LINEAR_ARBITRARY_H
