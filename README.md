# Random numbers in C++
`tbx.rand.h` provides feature-rich replacements for `rand()` and `srand(seed)` from the standard library header `<cstdlib>`.
## Simple user interface
- Without template arguments, `tbx::rand()` and `tbx::srand(seed)` behave the same as the corresponding functions in `<cstdlib>`.

- Adding a template argument let's you specify the type of generated numbers, for example: `tbx::rand<short>()`, `tbx::rand<double>()`, and `tbx::rand<std::int64>()`.

- Adding function arguments restricts the range of generated numbers. For example, `tbx::rand(1, 6)` gets you a dice roll of type `int`, and `tbx::rand<double>(-180.0, 180.0)` generates a random angle between +/- 180 degrees.

## Expanded seeding options
There are three ways to seed the `std::mt19937` random number engine used by `tbx.rand.h`.
- `tbx::srand(seed)` uses an `unsigned int` to seed the engine.
- `tbx::srand()` randomly seeds all 624 state variables using `std::random_device`.
- `tbx::srand(seed_seq)` uses a `std::seed_seq` to seed all 624 state variables.

_Important note:_ Be sure to include the appropriate template argument when calling one of the seeding functions. Each data type has its own random number engine, so if you omit the the template argument, you will probably end up seeding the wrong one.

For example, `tbx::srand()` randomly seeds the engine that generates `int` values, while `tbx::srand<double>(42u)` seeds the engine that generates `double` values.

## Thread safe
All functions are "thread_local," meaning that each thread where `rand()` is called has a random number engine and distribution of its own. Threads that call functions in the `rand()` family do not contend with other threads that use `rand()`, so no locking is necessary. 

## Easy installation
Just copy the header file `tbx.rand.h` to your project folder.

## Comprehensive Test Routines
A comprehensive set of unit tests systematically vary the result type, testing each overload of `rand()`, `srand()`, and `rand_max()` for every one. A small set of use tests demonstrates the functions in action.
