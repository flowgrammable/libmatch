#ifndef LIBMATCH_TRIE_HPP
#define LIBMATCH_TRIE_HPP

#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <utility> 
#include <cstring>

// FIXME: Don't use 'using namespace std' We may have conflicting types
// with the standard library so we want to make sure we know what the
// fully qualified name is.
using namespace std;

// FIXME: For C, this would be a typedef, not a define since you are
// defining a type. For C++ the preferred way to do this is a 'using'
// directive. (ex. using ul = unsigned long int;)
#define UL unsigned long int
//#define MAX_PREFIX_LENGTH 32
// FIXME: Do not to use all caps for variable names.
int MAX_PREFIX_LENGTH;

// FIXME: Try to keep the column width at '70'

/////////////////////////////////////////////////////// Description //////////////////////////////////////////////////////////////////
// Program uses a tree bitmap scheme for achieving optimum storage and search speed for storing prefixes, along with fast insertions.  
// In this scheme, there are 2 bitmaps stored per trie node, one for all internally stored prefixes and one for the external pointers.
// The trie node also contains 2 pointers, pointing to an internal prefix array and a successor trie node array respectively. Bitmaps
// indicate valid positions within the internal prefix array and the successor trie node array. Nodes within the internal prefix array
// contain the next Hop information associated with a particular prefix rule. For details, refer: Network Algorithmics: Section- 11.8 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FIXME: Don't use all caps for data structure names. Also, keeping
// with the style of the other parts of this library move the definitions
// of functions outside the data structure, unless its trivial.

// Bitmap where 1 bit indicate valid/invalid status. 
template<typename T>
struct BITMAP
{
	// FIXME: this is not a type, refrain from using the postfix _t.
	// Maybe try 'using bitfield_t = unsigned long;' then have this
	// member look like 'bitfield_t* bitfield;' You can then also use
	// the bitfield_t instead of writing out 'unsigned long' everywhere.
	unsigned long* bfield_t;


	// NOTE: Use two spaces between functions, and leave the return type
	// hanging above the function declaration

	// FIXME: Make this a constructor
	void 
	init(size_t size)
	{
		size_t mem_cells = ceil((double)size/(double)sizeof(unsigned long)) ; 
		bfield_t = new unsigned long[mem_cells];
		memset(bfield_t, 0, sizeof(unsigned long)*mem_cells);
	}

	// FIXME: use a destructor to handle memory cleanup

	T 
	get(T const& index)
	{
		return 1 & (bfield_t[index/(8*sizeof(unsigned long))]>> (index % (8 * sizeof(unsigned long) )));
	}


	void 
	set(T const& index)
	{
		bfield_t[index/(8*sizeof(unsigned long))] |= (1<< (index % (8 * sizeof(unsigned long) )));
	}
	

	void 
	reset(T const& index)
	{
		bfield_t[index/(8*sizeof(unsigned long))] &= ~(1<< index % (8 * sizeof(unsigned long) ));	
	}

	
	bool 
	checkIfEmpty(T const& size) const
	{
		T mem_cells = ceil((double)size/(double)sizeof(unsigned long)) ;
		T i;
		for(i=0; i< mem_cells; i++)
		{
			if(bfield_t[i]!=0)
				return false;
		}
		return true;
	}
};

// Represents next hop information associated with each prefix rule.
template <typename V>
class prefix_node
{
public:
	V next_hop;

	// FIXME: Use a constructor instead of this 'init' function	
	void init(V data)
	{
		next_hop = data;
	}


	V 
	getHop()
	{
		return next_hop;
	}
};

// Represents Trie node which contains 2 pointers to an array of child trie nodes and prefix nodes. Also contains 2 bitmaps representing 
// valid/invalid status of each child pointer.
template <typename T, typename G, typename V>
class trie_node
{
	T stride;
	BITMAP<T> triePointers;
	BITMAP<T> prefixPointers;
	trie_node<T,G,V>* trieNodes;
	prefix_node<V>* prefixNodes;
	
public:
	// FIXME: use a constructor
	// FIXME: use a destructor to handle memory cleanup
	void init(G);	
	bool checkIfEmpty();
	T getStride();

	void insertData(trie_node<T,G,V>*, G,T,V);
	void searchData(trie_node<T,G,V>*, G,prefix_node<V>*&);
	void updateData(trie_node<T,G,V>*, G, T, V);
	bool deleteData(trie_node<T,G,V>*, G, T);
};

template <typename T, typename G, typename V>
void trie_node<T,G,V>::init(G strd)
{
	stride = strd;
	trieNodes = new trie_node[(T)pow(2,stride)];
	prefixNodes = new prefix_node<V>[(T)pow(2,stride)*2-1];
	triePointers.init((T)pow(2,stride));
	prefixPointers.init((T)pow(2,stride)*2-1);
}

template <typename T, typename G, typename V>
bool trie_node<T,G,V>::checkIfEmpty()
{
	// FIXME: put a space between 'for', 'while', and 'if' and the first '('
	if (triePointers.checkIfEmpty((G)pow(2,stride)) && prefixPointers.checkIfEmpty((G)pow(2,stride)*2-1))	
	{
		delete[] trieNodes;
		delete[] prefixNodes;
		return true;
	}
	else
		return false;
}

template <typename T, typename G, typename V>
T trie_node<T,G,V>::getStride()
{	
	return stride;
}

// Insert data represented by prefix, next_hop into the trie data structure.
template <typename T, typename G, typename V>
void trie_node<T,G,V>::insertData(trie_node<T,G,V>* node, G prefix, T prefix_length, V next_hop)
{
	while (1)
	{
		G mask = ~0 << (MAX_PREFIX_LENGTH - min(node->stride,prefix_length));
		G node_prefix = (prefix&mask)>>(MAX_PREFIX_LENGTH - min(node->stride, prefix_length));	

		// update prefixes in the corresponding trie positions
		if (prefix_length > node->stride)	
		{
			trie_node<T,G,V>* tn = node->trieNodes + node_prefix;
			// create trie node pointer	
			if (!node->triePointers.get(node_prefix))
			{
				cout<<"Initializing trie node: "<<node<<"\n";
				tn->init(node->stride);
				node->triePointers.set(node_prefix);
			}
			prefix_length -= node->stride;
			prefix = prefix<<node->stride;
			node = tn;
			//tn.insertPrefix(prefix<<node->stride, prefix_length, next_hop);
		}
		else
		{
			// insert prefix pointer
			T prefixPointerIndex = pow(2, prefix_length)-1 + node_prefix;
			prefix_node<V>* pn = node->prefixNodes + prefixPointerIndex;
			cout<<"Initializing prefix node: "<<pn<<" with data: "<<next_hop<<"\n";
			pn->init(next_hop);
			node->prefixPointers.set(prefixPointerIndex);
			break;
		}
	}
}

// search next hop info associated the longest matching rule in the trie. 
template <typename T, typename G, typename V>
void trie_node<T,G,V>::searchData(trie_node<T,G,V>* node, G item, prefix_node<V>*& BMP)
{
	while (1)
	{
		cout<<"Traversing trie node: "<<node<<"\n";
		G mask = ~0 << (MAX_PREFIX_LENGTH - node->stride);
		G node_prefix = (item&mask) >>  (MAX_PREFIX_LENGTH - stride);
		T cur_stride= stride;
		// update BMP
		while (cur_stride > 0)
		{
			T prefixNodeIndex = (pow(2, cur_stride)-1) + (node_prefix >> (stride - cur_stride));
			if (prefixPointers.get(prefixNodeIndex))
			{
				cout<<"Setting Prefix Node: "<<prefixNodes + prefixNodeIndex<<" index: "<<prefixNodeIndex<<"\n";
				BMP = prefixNodes + prefixNodeIndex;
				break;
			}	
			cur_stride--;
		}
		// traverse successor trie node if it exists
		// create trie node pointer	
		if (triePointers.get(node_prefix))
		{
			item = item<<node->stride;
			node = trieNodes + node_prefix;
		}
		else
			break;
	}
	//cout<<BMP->next_hop;
}

// update next hop info associated with a rule inserted in the trie.
template <typename T, typename G, typename V>
void trie_node<T,G,V>::updateData(trie_node<T,G,V>* node, G prefix, T prefix_length, V next_hop)
{
	while (1)
	{
		G mask = ~0 << (MAX_PREFIX_LENGTH - min(node->stride,prefix_length));
		G node_prefix = (prefix&mask)>>(MAX_PREFIX_LENGTH - min(node->stride, prefix_length));	

		if (prefix_length > node->getStride())	
		{
			trie_node<T,G,V>* tn = node->trieNodes + node_prefix;
			// create trie node pointer	
			if (!node->triePointers.get(node_prefix))
			{
				cout<<"Error !!! Prefix Not Found\n";
				break;
			}
			else
			{
				prefix_length -= node->stride;
				prefix = prefix << node->stride;
				node = tn;
			}
		}
		else
		{
			// update prefix pointer
			T prefixPointerIndex = pow(2, prefix_length) -1 + node_prefix;
			prefix_node<V>* pn = node->prefixNodes + prefixPointerIndex;
			if (node->prefixPointers.get(prefixPointerIndex))
			{
				cout<<"Updating Prefix node: "<<pn<<" with next hop info: "<<next_hop<<"\n";
				pn->init(next_hop);
			}
			else
				cout<<"Error !! Prefix Not Found\n";
			break;
		}
	}
}

// Delete rule specified by prefix, prefix_length from the trie.
template <typename T, typename G, typename V>
bool trie_node<T,G,V>::deleteData(trie_node<T,G,V>* node, G prefix, T prefix_length)
{
	UL mask = ~0 << (MAX_PREFIX_LENGTH - min(node->stride,prefix_length));
	UL node_prefix = (prefix&mask)>>(MAX_PREFIX_LENGTH - min(node->stride, prefix_length));	

	// update prefixes in the corresponding trie positions after padding
	if(prefix_length > node->getStride())	
	{
		// access trie node pointer	
		if(!node->triePointers.get(node_prefix))
			return false;

		trie_node<T,G,V>* tn = node->trieNodes + node_prefix;
		prefix_length -= node->stride;
		if(deleteData(tn, (prefix<<node->stride), prefix_length))
		{
			node->triePointers.reset(node_prefix);
			return node->checkIfEmpty();
		}	
	}
	else
	{
		// access and reset prefix pointer bitmap
		int prefixPointerIndex = pow(2, prefix_length) -1  + node_prefix;
		prefix_node<V>* pn = node->prefixNodes + prefixPointerIndex;
		node->prefixPointers.reset(prefixPointerIndex);
		return node->checkIfEmpty();
	}
}

#endif