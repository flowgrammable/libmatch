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


using namespace std;

/////////// Description ///////////////////////////////////////////////////////////////////////////////////////////////
// Program maps the arbitrary wildcard rules (includes '0', '1', and '*') into two different parts: bitMask, and wildMask.
// bitMask: parse the '*' and '0' into '0', the '1' into '1'.
// wildMask: parse the '*' into '0', the '0' and '1' into '1'.
// ruleMatch: Bool (Incoming packet & wildMask == bitMask), if true, then match, if false, don't match
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////// Input File Format ////////////////////////////////////////////////////////////////////////////////////////////
/* Rules  */
// *11*0110
// **1010**
// 0011*110
// 0011010*
// 10101100

/* Incoming packets  */
// 01110110
// 10001010

/* Test whether the incoming packet match the rule table  */
vector<int> ruleMatchesBit( const string &packet,  const vector< unsigned int > &bitMask,  /* return to a int value  */
                         const vector< unsigned int > &wildMask)        /* 2-dim vector  */
{
  vector<int> result;
  unsigned int packetBit = 0;
  for(int i = 0; i < packet.size(); i++) {
    packetBit <<= 1;
      if(packet[i] == '1')
        packetBit |= 1;
  }

  for(int i = 0; i < (wildMask.size() - 1); i++)     
  {
    bool match = true;
    if((packetBit & wildMask[i]) != bitMask[i])
      match = false;

    if(match)
      result.push_back(i);
   }

   return result;
}


/* Parse the rules into bitmask.  */
/* retrun int vector.  */
void parseBitmaskBit(string& s, vector<unsigned int> &rules)
{
  unsigned int rule = 0;
  for(int i = 0; i < s.length(); i++)
  {
    rule = rule << 1;
    if(s[i] == '1')
      rule = rule | 1;
    else if(s[i] == '0' || s[i] == '*')
      rule = rule | 0;
  }
  rules.push_back(rule);
}
/* Parse the rules into wildmask.  */
/* retrun int vector.  */

void parseWildmaskBit(string& s, vector<unsigned int> &rules)
{
  unsigned int rule = 0;
  for(int i = 0; i < s.length(); i++)
  {
    rule = rule << 1;
    if(s[i] == '1' || s[i] == '0')
      rule = rule | 1;
    else if(s[i] == '*')
      rule = rule | 0;
  }
  rules.push_back(rule);
}
/* Main function  */


int main(int argc, char* argv[])
{

/* Read the rules from an input file  */
  string line;
  ifstream file ("ping_test.txt");

  vector<string> ruleArray;
  vector<unsigned int> wildMask;
  vector<unsigned int> bitMask;
 
  int i = 0;
  if(file.is_open()) {
    while(!file.eof()) {
      getline(file, line);  /*  Read lines as long as the file is */

      std::cout << line << endl;

      if(!line.empty())
        ruleArray.push_back(line);   /* Push the input file into ruleArray  */
        parseWildmaskBit(line, wildMask);
        parseBitmaskBit(line, bitMask);
    }
  }

  file.close();

  cout << endl;
  string packet = "00110110";      /* Incoming packet  */
  cout << packet << endl;
  vector<int> result;
  result = ruleMatchesBit(packet, bitMask, wildMask);
  
  for(int i = 0; i < result.size(); i++)
    cout << result[i] << endl;

  return 0;
}












