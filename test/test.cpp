#include <iostream>
#include <cmath>

typedef unsigned long ul;

template<typename T> 
struct bitmap 
{
	
	ul* bfield;
	bitmap() { bfield = NULL; } 
	~bitmap() { if(bfield != NULL) delete bfield; }
	void init(size_t);

};




template<typename T> 
void
bitmap<T>::init(size_t size)  // indicate the valid or invalid, like 0 or 1
{
	size_t mem_cells =
	ceil((double)size/(double)sizeof(ul)) ; 
	bfield = new ul[mem_cells];
	memset(bfield, 0, sizeof(ul)*mem_cells);  // initialize the array of bfield
}

void main()
{

std::cout << (double)size/(double)sizeof(ul) << std::endl;
}
