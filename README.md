# Random numbers in C++14
`tbx.rand.h` provides feature-rich replacements for `rand()` and `srand(seed)` from the standard library header `<cstdlib>`.


## Simple user interface
- Without template arguments, `tbx::rand()` and `tbx::srand(seed)` behave the same as the corresponding functions in `<cstdlib>`.

- Adding a template argument let's you specify the type of generated values, for example: `tbx::rand<bool>()`, `tbx::rand<char>()`,`tbx::rand<short>()`, `tbx::rand<double>()`, and `tbx::rand<std::int64>()`.

- Adding function arguments restricts the range of generated values. For example, `tbx::rand(1, 6)` gets you a dice roll of type `int`, and `tbx::rand<double>(-180.0, 180.0)` generates a random angle between +/- 180 degrees.

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

For example, `tbx::srand(42u)` seeds the engine that generates `int` values, while `tbx::srand<double>(42u)` seeds the engine that generates `double` values.


## A variety of result types
`tbx.rand.h` works with any `result_type` accepted by `std::uniform_int_distribution` or `std::uniform_real_distribution`, as well as their many type aliases from `<cstdint>`. It also works with `bool` and `char` types.
```cpp
// Note that the argument to srand(seed) always has type unsigned, 
// even though its template argument may be different.

tbx::srand(42u);                     tbx::rand();         // same as tbx::rand<int>();
tbx::srand<bool>(42);                tbx::rand<bool>();
tbx::srand<char>(42);                tbx::rand<char>();

tbx::srand<signed char>(42);         tbx::rand<signed char>();
tbx::srand<short>(42u);              tbx::rand<short>();
tbx::srand<int>(42u);                tbx::rand<int>();
tbx::srand<long>(42u);               tbx::rand<long>();
tbx::srand<long long>(42u);          tbx::rand<long long>();

tbx::srand<unsigned char>(42);       tbx::rand<unsigned char>();
tbx::srand<unsigned short>(42u);     tbx::rand<unsigned short>();
tbx::srand<unsigned int>(42u);       tbx::rand<unsigned int>();
tbx::srand<unsigned long>(42u);      tbx::rand<unsigned long>();
tbx::srand<unsigned long long>(42u); tbx::rand<unsigned long long>();

tbx::srand<float>(42u);              tbx::rand<float>();
tbx::srand<double>(42u);             tbx::rand<double>();
tbx::srand<long double>(42u);        tbx::rand<long double>();

tbx::srand<std::int8_t>(42u);        tbx::rand<std::int8_t>();
tbx::srand<std::int16_t>(42u);       tbx::rand<std::int16_t>();
tbx::srand<std::int32_t>(42u);       tbx::rand<std::int32_t>();
tbx::srand<std::int64_t>(42u);       tbx::rand<std::int64_t>();

tbx::srand<std::uint8_t>(42u);       tbx::rand<std::uint8_t>();
tbx::srand<std::uint16_t>(42u);      tbx::rand<std::uint16_t>();
tbx::srand<std::uint32_t>(42u);      tbx::rand<std::uint32_t>();
tbx::srand<std::uint64_t>(42u);      tbx::rand<std::uint64_t>();

// etc.
```


## Thread safe
All functions are "thread_local," meaning that each thread where `rand()` is called has a random number engine and distribution of its own. Threads that call functions in the `rand()` family do not contend with other threads that use `rand()`, so no locking is necessary. 


## Easy installation
Just copy the header file `tbx.rand.h` to your project folder.


## Comprehensive test routines
A comprehensive set of unit tests systematically vary the result type, testing each overload of `rand()`, `srand()`, and `rand_max()` for every one. A small set of use tests demonstrates the functions in action.
