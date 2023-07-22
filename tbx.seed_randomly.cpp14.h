#ifndef TBX_SEED_RANDOMLY_CPP14_H
#define TBX_SEED_RANDOMLY_CPP14_H
//================================================================================
// "MIT License"
//================================================================================
// Copyright 2023 Michael J. Mannon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//================================================================================

//======================================================================
// tbx.seed_randomly.cpp14.h
// 
// This header provides tools for randomly seeding random number 
// engines in the C++ Standard Library.
// 
// They work with any random number engine that can be seeded using 
// a seed sequence, including all the engines in the C++ Standard 
// Library, and many others, as well.
// 
// The tools use only the features of C++14, nothing later.
//======================================================================
// seed_randomly
// 
// This function takes a random number engine as argument, and seeds 
// it with random seeds generated from std::random_device.
// 
// Example: Seed mt19937 with random seeds from std::random_device.
//    std::mt19937 mt;
//    tbx::seed_randomly( mt );
// 
// Example: Seed pcg32, one of the PCG engines by Melissa O'Neill.
//    pcg32 e;
//    tbx::seed_randomly(e);
// 
//======================================================================
// class seed_seq_rd
// 
// This class mimics the interface of std::seed_seq, but uses 
// std::random_device to generate seeds. 
// 
// Objects of this type are seed sequences that can be used as 
// arguments to member function seed in a random number engine.
// 
// Example: Seed mt19937 with random seeds from std::random_device.
//    std::mt19937 mt;
//    tbx::seed_seq_rd s;
//    mt.seed(s);
// 
// Example: Seed pcg32, one of the PCG engines by Melissa O'Neill.
//    pcg32 e;
//    e.seed(s);  // seed_seq_rd object can be reused.
// 
//======================================================================

#include <array>             // array, cbegin, cend
#include <cstddef>           // size_t
#include <cstdint>           // uint_least32_t
#include <initializer_list>  // initializer_list
#include <iterator>          // iterator_traits
#include <random>            // random_device
#include <type_traits>       // is_integral_v, is_unsigned_v
namespace tbx
{
    class seed_seq_rd
    {
        // This class mimics the interface of std::seed_seq, but 
        // uses std::random_device to generate seeds. 
        //
        // It performs only basic checks of its template arguments. 
        // Other than that, it complies with all requirements of a seed 
        // sequence as defined in the C++ standard.
    public:
        using result_type = typename std::random_device::result_type;
    private:
        // No matter what ctor you use, all you get is this array 
        // with one element. Best practice, therefore, is to use the 
        // default ctor.
        enum : std::size_t { zero, one };
        std::array<result_type, one> seeds{};
    public:
        seed_seq_rd() noexcept
            = default;
        template< typename InputIt >
        seed_seq_rd(InputIt begin, InputIt end) {
            using value_type = typename std::iterator_traits<InputIt>::value_type;
            static_assert(std::is_integral_v<value_type>, "");
        }
        template< typename T >
        seed_seq_rd(std::initializer_list<T> li) {
            using value_type = typename std::initializer_list<T>::value_type;
            static_assert(std::is_integral_v<value_type>, "");
        }
        seed_seq_rd(seed_seq_rd const&)
            = delete;
        seed_seq_rd& operator=(seed_seq_rd const&)
            = delete;
        template< typename RandomIt >
        void generate(RandomIt begin, RandomIt end) {
            using value_type = typename std::iterator_traits<RandomIt>::value_type;
            static_assert(std::is_integral_v<value_type>, "");
            static_assert(std::is_unsigned_v<value_type>, "");
            static_assert(sizeof(value_type) >= sizeof(std::uint_least32_t), "");
            std::random_device rd;
            while (begin != end)
                *begin++ = rd();
        }
        template <typename OutputIt>
        void param(OutputIt dest) const {
            *dest = seeds.front();
        }
        auto size() const noexcept {
            return seeds.size();
        }
    };

    template <typename RandomEngine>
    void seed_randomly(RandomEngine& e) {
        tbx::seed_seq_rd s;
        e.seed(s);
    }
}
#endif  // TBX_SEED_RANDOMLY_CPP14_H
// end file: tbx.seed_randomly.cpp14.h
