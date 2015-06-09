# libmatch

Data structures and algorithms for key/value matching.

TODO: Write me.


## Building libmatch

`libmatch` is written in in the C++11 programming language. The following
compilers are known to support the feature set required to build the
`libmatch` library:

- GCC 4.8
- Clang 3.3

There are several external dependencies for `libmatch`.

- Boost C++ Libraries vesion 1.55.0 or greater
- ...

If building against a custom deployment of Boost, set BOOST_ROOT
to the top-level source directory of that deployment.

`libmatch` is built using CMake. 

```
> mkdir build
> cd build
> cmake ..
> make
```

## Testing

In order to run the test suite, simply run the following
command after building the program.

```
make test
```

The unit test framework is based on CTest.


## Benchmarking

To run the performance tests and benchmark comparisons...


## Installing

After building `libmatch`, run the following:

```
make install
```

Note that installation prefix can be set in configuration changing
the CMAKE_INSTALL_PREFIX variable.


## Using `libmatch`

TODO: Write me.

