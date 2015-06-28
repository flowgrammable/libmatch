#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility> 
#include <memory.h>
#include "../libmatch/stride_trie.hpp"
using namespace std;

enum OPR
{
	INSERTION=1,
	SEARCH=2,
	DELETION=3,
	UPDATE=4,
};
// Testing data read from: trie_input.txt
// Format: 
// 1st line indicates T(number of operations)
// Followed by T lines
// If insertion operation, line contains: <Op type: 1> <RULE DATA(HEXA FORMAT)> <PREFIX LENGTH> <NEXT_HOP INFO> 
// If search operation, line contains: <Op type: 2> <INPUT DATA(HEXA FORMAT)>  
// If update operation, line contains: <Op type: 4> <RULE DATA(HEXA FORMAT)> <PREFIX LENGTH> <NEXT_HOP INFO> 
// If delete operation, line contains: <Op type: 3> <RULE DATA(HEXA FORMAT)> <PREFIX LENGTH>  
int main(int argc, char* argv[])
{
	trie_node<int,UL,string>* root= new trie_node<int,UL,string>(); 	
	root->init(3);
	MAX_PREFIX_LENGTH = 32;	

	int T;
	cin>>T;

	while(T--)
	{
		int type;
		cin>>type;
		if((OPR)type == INSERTION)
		{
			UL prefix; int prefix_length; string next_hop;
			cin>>hex>>prefix;
			cin>>prefix_length;
			cin>>next_hop;
			root->insertData(root, prefix, prefix_length, next_hop);
		}
		else if((OPR)type == SEARCH)
		{
			UL item_to_be_searched;
			prefix_node<string>* BMP=NULL;
			cin>>hex>>item_to_be_searched;
			root->searchData(root, item_to_be_searched, BMP);	
			if(BMP)
				cout<<"NEXT HOP found: "<<BMP->getHop()<<"\n";
			else
				cout<<"No Matching Rule Found!!\n";
		}
		else if((OPR)type == UPDATE)
		{
			UL prefix; int prefix_length; string next_hop;
			cin>>hex>>prefix;
			cin>>prefix_length;
			cin>>next_hop;
			root->updateData(root, prefix, prefix_length, next_hop);
		}
		else if((OPR)type == DELETION)
		{
			UL prefix; int prefix_length;;
			cin>>hex>>prefix;
			cin>>prefix_length;
			root->deleteData(root, prefix, prefix_length);
		}
	}
	return 0;
}
