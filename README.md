# seed_randomly
Header `tbx.seed_randomly.cpp14.h` provides tools for randomly seeding random number engines in the C++ Standard Library. They work with any random number engine that can be seeded using a seed sequence, including all the engines in the C++ Standard Library, and many others, as well.

The tools use only the features of C++14, nothing later.

## seed_randomly
This function takes a random number engine as argument, and seeds it with random seeds generated from `std::random_device`.
````cpp
// Example: Seed mt19937 with random seeds from std::random_device.
   std::mt19937 mt;
   tbx::seed_randomly( mt );

// Example: Seed pcg32, one of the PCG engines by Melissa O'Neill.
   pcg32 e;
   tbx::seed_randomly( e );
````

## class seed_seq_rd
This class mimics the interface of `std::seed_seq`, but uses `std::random_device` to generate seeds. Objects of this type are seed sequences that can be used as arguments to member function `seed` in a random number engine.

Function `seed_randomly` is a trivial wrapper around a `seed_seq_rd` object.
````cpp
// Example: Seed mt19937 with random seeds from std::random_device.
   std::mt19937 mt;
   tbx::seed_seq_rd s;
   mt.seed( s );

// Example: Seed pcg32, one of the PCG engines by Melissa O'Neill.
   pcg32 e;
   e.seed( s );  // seed_seq_rd object can be reused.

// Implementation of seed_randomly:
   template< typename RandomNumberEngine >
   void seed_randomly( RandomNumberEngine& e);
   {
        tbx::seed_seq_rd s;
        e.seed(s);
   }
````