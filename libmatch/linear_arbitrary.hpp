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
    uint8_t value;
    uint8_t mask;
    Rule()
    {
        value = 0;
        mask = 0;
    }

    Rule(uint8_t x, uint8_t y)
    {
        value = x;
        mask = y;
    }

};

class linearTable
{
public:
    vector<Rule> rulesTable;   // Empty vector of Rule struct
    int count; // The number of rules in the linearTable

    // linearTable constructor
    linearTable();  // constructor
    ~linearTable(); // delete structure

    static void insert_rule( vector<Rule>& rulesTable, Rule& rule );

    static bool search_rule( vector<Rule>& rulesTable, uint8_t key );

    static void delete_rule( vector<Rule>& rulesTable, Rule& rule );


};





#endif // LINEAR_ARBITRARY_H
