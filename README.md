# Random numbers in C++
`tbx.rand.h` provides feature-rich replacements for `rand()` and `srand(seed)` from the standard library header `<cstdlib>`.
## Simple user interface
- Without template arguments, `tbx::rand()` and `tbx::srand(seed)` behave the same as the corresponding functions in `<cstdlib>`.

- Adding a template argument let's you specify the type of generated numbers, for example: `tbx::rand<short>()`, `tbx::rand<double>()`, and `tbx::rand<std::int64>()`.

- Adding function arguments restricts the range of generated numbers. For example, `tbx::rand(1, 6)` gets you a dice roll of type `int`, and `tbx::rand<double>(-180.0, 180.0)` generates a random angle between +/- 180 degrees.

- Support for `param_type`
```cpp
// param_type is defined in std::uniform_int_distribution and std::uniform_real_distribution.
// tbx::param_type simplifies its usage.
tbx::param_type<double> p(-180.0, 180.0);
tbx::srand<double>();  // randomly seed the engine used for doubles
tbx::rand<double>(p);  // slightly faster than tbx::rand<double>(-180.0, 180.0);
```

## Expanded seeding options
There are three ways to seed the `std::mt19937` random number engine used by `tbx.rand.h`.
- `tbx::srand(seed)` uses an `unsigned int` to seed the engine.
- `tbx::srand()` randomly seeds all 624 state variables using `std::random_device`.
- `tbx::srand(seed_seq)` uses a `std::seed_seq` to seed all 624 state variables.

**Important note:** Be sure to include the appropriate template argument when calling one of the seeding functions. Each data type has its own random number engine, so if you omit the the template argument, you will probably end up seeding the wrong one.

For example, `tbx::srand()` randomly seeds the engine that generates `int` values, while `tbx::srand<double>(42u)` seeds the engine that generates `double` values.

## A variety of result types
`rand()` works with any of the types accepted by `std::uniform_int_distribution` or `std::uniform_real_distribution`, as well as their many type aliases from `<cstdint>`.
```cpp
tbx::rand();  // same as tbx::rand<int>()

tbx::rand<short>();
tbx::rand<int>();
tbx::rand<long>();
tbx::rand<long long>();

tbx::rand<unsigned short>();
tbx::rand<unsigned int>();
tbx::rand<unsigned long>();
tbx::rand<unsigned long long>();

tbx::rand<float>();
tbx::rand<double>();
tbx::rand<long double>();

tbx::rand<std::int16_t>();
tbx::rand<std::int32_t>();
tbx::rand<std::int64_t>();

tbx::rand<std::uint16_t>();
tbx::rand<std::uint32_t>();
tbx::rand<std::uint64_t>();

// etc.
```

## Thread safe
All functions are "thread_local," meaning that each thread where `rand()` is called has a random number engine and distribution of its own. Threads that call functions in the `rand()` family do not contend with other threads that use `rand()`, so no locking is necessary. 

## Easy installation
Just copy the header file `tbx.rand.h` to your project folder.

## Comprehensive test routines
A comprehensive set of unit tests systematically vary the result type, testing each overload of `rand()`, `srand()`, and `rand_max()` for every one. A small set of use tests demonstrates the functions in action.
