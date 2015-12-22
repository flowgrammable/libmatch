#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>


using namespace std;
struct tuple
{
  unsigned int field1;
  unsigned int field2;
  // vector<unsigned int> fields;
  tuple ()
  { 
    field1 = 0;
    field2 = 0;
  }
  tuple(unsigned int x, unsigned int y)
  { 
    field1 = x;
    field2 = y;
  }
  tuple OR(const tuple& x)
  {
    tuple ret;
    ret.field1 = field1 |  x.field1;
    ret.field2 = field2 |  x.field2;
    return ret;
  }
  bool equals(const tuple& x)
  {
    if(field1 == x.field1 && field2 == x.field2)
	  return true;
    else
	  return false;
      
  }
};

struct Rule
{
  tuple value;
  tuple mask;
  Rule()
  {
  }
  Rule(tuple x, tuple y)
  {
    value = x;
    mask = y;
  }
  bool match(const tuple& packet)
  {
    tuple match1 = mask.OR(packet);
	tuple match2 = mask.OR(value);
    return match1.equals(match2); 
  }
};

vector<Rule> rules;

bool isMatch(tuple& packet, Rule& res)
{
  for(int i = 0; i < rules.size(); i++)
  {
    if(rules[i].match(packet))
	{
	  res =  rules[i];
	  cout << "MATCH SUCCESS\n";
	  return true;
	}
  }
  cout<<"MATCH FAILURE\n";
  return false;
}


int main()
{
  //unsigned int val2;
  //cin >> hex >> val2;

  //cout << val2 << "\n";
	
  tuple value(0,3);
  tuple mask(0,40);
  Rule rule1(value, mask);
  rules.push_back(rule1); 
  tuple packet(0,7);
  Rule res;
  isMatch(packet, res);

  return 0;
}
