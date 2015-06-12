#include "../libmatch/hash.hpp"

#include <cassert>
#include <iostream>
#include <ctime>


// Simple stopwatch class to get timings in milliseconds.
struct stop_watch
{
  // Data members
  std::clock_t initial; // Inital time
  std::clock_t final;   // Final time


  // Sets the inital time stamp.
  inline void 
  start()
  {
    initial = std::clock();
  }

  // Sets the final time stamp.
  inline void
  stop()
  {
    final = std::clock();
  }


  // Calculates the duration from the inital to the final time stamps
  // in milliseconds.
  inline double
  duration() const
  {
    return (final - initial) / (double) (CLOCKS_PER_SEC / 1000);
  }
};

void
basic_bucket_test()
{
  using hash_table::data_store::basic_bucket;
  
  basic_bucket<size_t> initially_empty;
  basic_bucket<size_t> initially_full(20);
  std::cout << "Begin 'basic_bucket' test...\n";
  
  // Check that the buckets were constructed correctly
  assert(initially_empty.is_empty());
  assert(initially_full.is_full());
  std::cout << "Constructors are good\n";
  
  // Check that the correct value is contained
  assert(initially_full.get() == 20);
  std::cout << "Get is good.\n";
  
  // Try inserting into the empty one
  initially_empty.insert(16);
  assert(initially_empty.get() == 16);
  std::cout << "Insert is good.\n";
  
  // Try erasing the wrong value
  initially_full.erase(1);
  assert(initially_full.get() == 20);
  std::cout << "Bad erase is good.\n";
  
  // Try erasing the correct value
  initially_full.erase(20);
  assert(initially_full.is_empty());
  std::cout << "Good erase is good.\n";
  
  // Try clearing
  initially_empty.clear();
  assert(initially_empty.is_empty());
  std::cout << "Clear is good.\n";
  std::cout << "End 'basic_bucket' test\n";
}


// Base test to make sure the table actually works
void
base_test()
{

}


// Check the runtimes for inserting into tables with varying
// table sizes and load sizes.
void
insert_test()
{

}


// Check the runtimes for finding values in tables with varying
// table sizes and load sizes.
void
find_test()
{

}


// Check the runtimes for resizing tables with varying table sizes
// and load sizes.
void
resize_test()
{

}


int 
main(void)
{ 
  std::cout << "Beginning hash test...\n";
  basic_bucket_test();
  
  std::cout << "Done.\n";
  return 0;
}
