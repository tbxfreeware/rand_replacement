# Random numbers in C++
`tbx.rand.h` provides feature-rich replacements for `rand()` and `srand(seed)` from the standard library header `<cstdlib>`.

The functions in this header require only the features of C++14, nothing later is used.

&nbsp;
## Simple user interface
- Without template arguments, `tbx::rand()` and `tbx::srand(seed)` behave the same as the corresponding functions in `<cstdlib>`.

- Adding a template argument let's you specify the type of generated values, for example: `tbx::rand<bool>()`, `tbx::rand<char>()`,`tbx::rand<short>()`, `tbx::rand<double>()`, and `tbx::rand<std::int64>()`.

- Adding function arguments restricts the range of generated values. For example, `tbx::rand(1, 6)` gets you a dice roll of type `int`, and `tbx::rand<double>(-180.0, 180.0)` generates a random angle between +/- 180 degrees.

&nbsp;
## Expanded seeding options
There are three ways to seed the `std::mt19937` random number engine used by `tbx.rand.h`.
- `tbx::srand(seed)` uses an `unsigned int` to seed the engine.
- `tbx::srand()` randomly seeds all 624 state variables using `std::random_device`.
- `tbx::srand(seed_seq)` uses a `std::seed_seq` to seed all 624 state variables.

**Important note:** Be sure to include the appropriate template argument when calling one of the seeding functions. Each data type has its own random number engine, so if you omit the the template argument, you will probably end up seeding the wrong one.

For example, `tbx::srand(42u)` seeds the engine that generates `int` values, while `tbx::srand<double>(42u)` seeds the engine that generates `double` values.

&nbsp;
## A variety of result types
`tbx.rand.h` works with any `result_type` accepted by `std::uniform_int_distribution` or `std::uniform_real_distribution`, as well as their many type aliases from `<cstdint>`. It also works with `bool` and `char` types.
```cpp
// Note that the argument to srand(seed) always has type unsigned, 
// even though its template argument may be different.

tbx::srand(42u);                     tbx::rand();         // same as tbx::rand<int>();
tbx::srand<bool>(42u);               tbx::rand<bool>();
tbx::srand<char>(42u);               tbx::rand<char>();

tbx::srand<signed char>(42u);        tbx::rand<signed char>();
tbx::srand<short>(42u);              tbx::rand<short>();
tbx::srand<int>(42u);                tbx::rand<int>();
tbx::srand<long>(42u);               tbx::rand<long>();
tbx::srand<long long>(42u);          tbx::rand<long long>();

tbx::srand<unsigned char>(42u);      tbx::rand<unsigned char>();
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

&nbsp;
## Support for `param_type`
`param_type` is defined in `std::uniform_int_distribution` and `std::uniform_real_distribution`. `tbx::param_type` simplifies its usage. 

When you call `tbx::rand(a, b)`, a new `param` object must be constructed from `a` and `b`. That step is bypassed when you call `tbx::rand(param)`.
```cpp
tbx::param_type<double> param(-180.0, 180.0);
tbx::srand<double>();      // randomly seed the engine used for doubles
tbx::rand<double>(param);  // slightly faster than tbx::rand<double>(-180.0, 180.0);
```

&nbsp;
## Thread safe
All functions are "thread_local," meaning that each thread where `rand()` is called has a random number engine and distribution of its own. Threads that call functions in the `rand()` family do not contend with other threads that use `rand()`, so no locking is necessary. 

&nbsp;
## Implicit seeding
If one of the overloads of `rand()` is called in a given thread before it has been seeded in that thread, it behaves as if it had been seeded with `seed(1u)`. This mimics the behavior of `rand()` in the C++ Standard Library.
````cpp
tbx::rand()                // implicitly seeded with srand(1u);
tbx::rand<double>()        // implicitly seeded with srand<double>(1u);
tbx::rand<std::uint64_t>   // implicitly seeded with srand<std::uint64_t>(1u);
// etc.
````

&nbsp;
## Easy installation
Just copy the header file `tbx.rand.h` to your project folder.

&nbsp;
## Comprehensive test routines
A comprehensive set of unit tests systematically vary the result type, testing each overload of `rand()`, `srand()`, and `rand_max()` for every one. A small set of use tests demonstrates the functions in action.

&nbsp;
## Check `random_device`
Before using function `srand()`, the overload that uses `std::random_device`, you should satisfy yourself that `std::random_device` is a good source of entropy on your system. Sometimes, it is not.

[Microsoft Visual C++](https://learn.microsoft.com/en-us/cpp/standard-library/random-device-class?view=msvc-170), for instance, generates "non-deterministic and cryptographically secure" values, and never blocks, which is excellent. Prior to version 9.2, however, [MinGW distributions of GCC](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85494) used `std::mt19937` with a fixed seed! Those systems generated the same sequence every time. (Newer versions purport to have fixed the problem, but I have not checked.) [Unix-like systems](https://en.wikipedia.org/wiki//dev/random) often use `/dev/random` (which can block) or `/dev/urandom`. Both have their advantages.

So, do your homework.

In case you decide not use function `seed()`, you can still use one of the other seeding functions. To seed `std::mt19937` or `std::mt19937_64`, your best bet would then be to create a `std::seed_seq` with 624 seeds of type `unsigned`, and do your seeding with that. 
