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
// *11*0110 drop
// **1010** port1
// 0011*110 port2
// 0011010* port3
// 10101100 port4

/* Incoming packets  */
// 01110110
// 10001010

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/* Test whether the incoming packet match the rule table  */
vector<int> ruleMatches( string packet,  vector< vector<int> > bitMask,  /* return to a int value  */
                   vector< vector<int> > wildMask)        /* 2-dim vector  */
{	
	vector<int> result;
        
	for(int j = 0; j < bitMask.size(); j++)     /* when using vector, don't need to declare the size of vector, use .size() */
		{
			int match = 1;
        	for (int i = 0; i < packet.size(); ++i)
			{
                if(((packet[i]-'0') & wildMask[j][i]) != bitMask[j][i]){
                       match = 0;
                }
			}
		if(match) 
			result.push_back(j);
        }
	if(result.empty()) 
		result.push_back(-1);
        
		return result;
}
	

/* Parse the rules into wildmask.  */
/* retrun int vector.  */

vector< vector<int> > parseWildmask(vector<string> &ruleArray)
{
	vector< vector<int> > wildMask(ruleArray.size(), vector<int>(ruleArray[0].size()));
	for(int i = 0; i < ruleArray.size(); i++)
	{		
		for (int j = 0; j < ruleArray[0].size(); j++)
		{
			 if (ruleArray[i][j] == '*') 
                                wildMask[i][j] = 0; 
                        else 
                                wildMask[i][j] = 1;
		}
	
	}
return wildMask;
}

/* Parse the rules into bitmask.  */
/* retrun int vector.  */

vector< vector<int> > parseBitmask(vector<string> &ruleArray)
{
	vector< vector<int> > bitMask(ruleArray.size(), vector<int>(ruleArray[0].size()));
	for(int i = 0; i < ruleArray.size(); i++)
	{		
		for (int j = 0; j < ruleArray[0].size(); j++)
		{
			  if (ruleArray[i][j] == '0' || ruleArray[i][j]=='*') 
                                        bitMask[i][j] = 0;
                        if (ruleArray[i][j] == '1')
                                        bitMask[i][j] = 1;
		}
	}
return bitMask;
}


/* Main function  */


int main(int argc, char* argv[])
{

/* Read the rules from an input file  */
	string line;
	ifstream file ("ping_test.txt");
	
	vector<string> ruleArray;

	int i = 0;
 	if(file.is_open()) {
	
      	while(!file.eof()) {
			getline(file, line);  /*  Read lines as long as the file is */
			if(!line.empty())			
			ruleArray.push_back(line);   /* Push the input file into ruleArray  */
		}

	}
	
	file.close();
	
	for(int j = 0; j < ruleArray.size(); j++) {
		cout << ruleArray[j] << endl;  /* get the ruleArray table */
	}

	
	vector< vector<int> > wildMask = parseWildmask(ruleArray);    /* get the wildmask rule table  */

	vector< vector<int> >  bitMask = parseBitmask(ruleArray);     /* get the bitmask rule table  */

	cout <<endl;

/* Output the wildmask rule table.  */

	for(int i = 0; i < wildMask.size(); i++) {
		for(int j = 0; j < wildMask[0].size(); j++)
			cout << wildMask[i][j];
			cout << endl;
		}
	
		cout <<endl;

/* Output the bitmask rule table.  */

	for(int i = 0; i < bitMask.size(); i++) {
		for(int j = 0; j < bitMask[0].size(); j++)
			cout <<bitMask[i][j];
			cout <<endl;
		}
	

	string packet = "00110110";      /* Incoming packet  */

	vector<int> result;
	result = ruleMatches(packet, bitMask, wildMask);
	for(int i = 0; i < result.size(); i++)
	cout << result[i] << endl;

	
	return 0;	
}


   







        








