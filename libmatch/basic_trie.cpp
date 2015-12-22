#include <iostream>
#include <vector>
#include <string>
 
using namespace std;

// Trie node which contains two pointers: left and right
// int value represent if the node is leaf node

struct trie_node
{
  int value; // Represent the leaf node information
  trie_node *children[2]; // Trie stride = 1, has two pointer, '0' and '1'

  // trie_node constructor

  trie_node()
  {
    value = 0;
    children[0] = NULL;
    children[1] = NULL;
  }

};


class Trie
{
public:
  trie_node *root;
  int count; // THe number of rules in a trie

  // Trie constructor

  Trie()
  {
    root = NULL;
    count = 0;
  }

  static trie_node* getNode();
  static void init_trie(Trie *pTrie); // Initilize a trie
  void del();
  static void insert_rule(Trie *pTrie, string key);
  static int search_rule(Trie *pTrie, string key);

};

trie_node* Trie::getNode()
{
trie_node *pNode = new trie_node();
  if (pNode)
    {
      pNode->value = 0;
      pNode->children[0] = NULL;
      pNode->children[1] = NULL;
    }
  return pNode;
}


void Trie::init_trie(Trie *pTrie)
{
  pTrie->root = getNode();
  pTrie->count = 0;
}

void Trie::insert_rule(Trie *pTrie, string key)
{
  int level;
  int index;
  trie_node *pRule;

  pTrie->count++;
  pRule = pTrie->root;

  for (level=0; level<key.length(); level++)
    {
      if (key.at(level) == '0')
	{
	  index = 0;
	}
      else if (key.at(level) == '1')
	{
	  index = 1;
	}
      if ( !pRule->children[index] )
	{
	  pRule->children[index] = getNode();
	}
      pRule = pRule->children[index];

    }
  pRule->value = pTrie->count;

}

int Trie::search_rule(Trie *pTrie, string key)
{
  int level;
  int index;
  trie_node *pRule;

  pRule = pTrie->root;

  for (level=0; level<key.length(); level++)
    {
      if (key.at(level) == '0')
	{
	  index = 0;
	}
      else if (key.at(level) == '1')
	{
	  index = 1;
	}
      if ( !pRule->children[index] )
	{
	  return 0;
	}
      pRule = pRule->children[index];

    }
  return (0 != pRule && pRule->value);
}

int main()
{
  vector<string> list = {"0010", "100", "00", "10101", "0", "110"};
  Trie trie;
  char output[][32] = {"Not present in trie", "Present in trie"};
  Trie::init_trie(&trie);
  for (int i=0; i<list.size(); i++)
    {
      Trie::insert_rule(&trie, list.at(i));
    }

  cout << output[Trie::search_rule(&trie, "10101")] << endl;
  cout << output[Trie::search_rule(&trie, "10111")] << endl;
  cout << output[Trie::search_rule(&trie, "100")] << endl;
  cout << output[Trie::search_rule(&trie, "1")] << endl;

  return 0;

}
