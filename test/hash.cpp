#include "libmatch/hash.hpp"

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
  return 0;
}