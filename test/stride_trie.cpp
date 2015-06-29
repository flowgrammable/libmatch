#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility> 
#include <memory.h>
#include "../libmatch/stride_trie.hpp"

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
// If insertion operation, 
// line contains: <Op type: 1> <RULE DATA(HEXA FORMAT)> 
//                <PREFIX LENGTH> <NEXT_HOP INFO> 
// If search operation,
// line contains: <Op type: 2> <INPUT DATA(HEXA FORMAT)>  
// If update operation,
// line contains: <Op type: 4> <RULE DATA(HEXA FORMAT)>
//		  <PREFIX LENGTH> <NEXT_HOP INFO> 
// If delete operation,
// line contains: <Op type: 3> <RULE DATA(HEXA FORMAT)> <PREFIX LENGTH>  
int main(int argc, char* argv[])
{
	trie_node<int,ul,std::string>* root= new trie_node<int,ul,std::string>(); 	
	root->init(3);
	// setting maximum number of bits a rule can contain
	max_prefix_length = 32;	

	int T;
	std::cin>>T;

	while (T--)
	{
		int type;
		std::cin>>type;
		if ((OPR)type == INSERTION)
		{
			ul prefix; int prefix_length; std::string next_hop;
			std::cin>>std::hex>>prefix;
			std::cin>>prefix_length;
			std::cin>>next_hop;
			root->insertData(root, prefix, prefix_length, 
			next_hop);
		}
		else if ((OPR)type == SEARCH)
		{
			ul item_to_be_searched;
			prefix_node<std::string>* BMP=NULL;
			std::cin>>std::hex>>item_to_be_searched;
			root->searchData(root, item_to_be_searched, BMP);	
			if(BMP)
				std::cout<<"NEXT HOP found: "<<BMP->getHop()<<"\n";
			else
				std::cout<<"No Matching Rule Found!!\n";
		}
		else if ((OPR)type == UPDATE)
		{
			ul prefix; int prefix_length; std::string next_hop;
			std::cin>>std::hex>>prefix;
			std::cin>>prefix_length;
			std::cin>>next_hop;
			root->updateData(root, prefix, prefix_length, next_hop);
		}
		else if ((OPR)type == DELETION)
		{
			ul prefix; int prefix_length;
			std::cin>>std::hex>>prefix;
			std::cin>>prefix_length;
			root->deleteData(root, prefix, prefix_length);
		}
	}
	return 0;
}


