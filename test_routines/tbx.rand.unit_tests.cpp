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
#include <array>        // array
#include <cassert>      // assert
#include <cstddef>      // size_t
#include <cstdint>      // int16_t, int32_t, int64_t, uint16_t, uint32_t, uint64_t
#include <iostream>     // basic_ostream
#include <limits>       // numeric_limits
#include <random>       // mt19937, seed_seq, uniform_int_distribution
#include <string>       // char_traits, string
#include <thread>       // thread
#include <type_traits>  // is_floating_point_v, is_same_v, is_unsigned_v

#include "tbx.rand.h"

//======================================================================
// Test Routines - ResultType bool
//======================================================================
namespace
{
    bool bool_result_type__test_rand()
    {
        static_assert(std::is_same_v<bool, decltype(tbx::rand<bool>())>, "");
        auto const r{ tbx::rand<bool>() };
        enum : int { n_trials = 10'000 };
        auto output_varies{ false };
        for (int i{ n_trials }; i--;)
            if (tbx::rand<bool>() != r)
            {
                output_varies = true;
                break;
            }
        assert(output_varies);
        return output_varies;
    }
    //------------------------------------------------------------------
    bool bool_result_type__test_rand__a_b(bool const a, bool const b)
    {
        static_assert(std::is_same_v<bool, decltype(tbx::rand<bool>(a, b))>, "");

        auto pass{ true };
        auto const r{ tbx::rand<bool>() };
        if (a == b)
        {
            pass = (r == tbx::rand<bool>(a, b)) && pass;
            assert(pass);
        }
        else
        {
            auto output_varies{ false };
            enum : int { n_trials = 10'000 };
            for (int i{ n_trials }; i--;)
                if (tbx::rand<bool>() != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    bool bool_result_type__test_rand__a_b()
    {
        auto pass{ true };
        pass = bool_result_type__test_rand__a_b(false, true) && pass;
        pass = bool_result_type__test_rand__a_b(false, false) && pass;
        pass = bool_result_type__test_rand__a_b(true, true) && pass;
        return pass;
    }
    //------------------------------------------------------------------
    bool bool_result_type__test_rand__param(bool const a, bool const b)
    {
        int const aa{ static_cast<int>(a) }, bb{ static_cast<int>(b) };
        tbx::param_type<int> p(aa, bb);
        static_assert(std::is_same_v<bool, decltype(tbx::rand<bool>(p))>, "");

        auto pass{ true };
        auto const r{ tbx::rand<bool>(p) };
        if (a == b)
        {
            pass = (r == tbx::rand<bool>(p)) && pass;
            assert(pass);
        }
        else
        {
            auto output_varies{ false };
            enum : int { n_trials = 10'000 };
            for (int i{ n_trials }; i--;)
                if (tbx::rand<bool>() != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    bool bool_result_type__test_rand__param()
    {
        auto pass{ true };
        pass = bool_result_type__test_rand__param(false, true) && pass;
        pass = bool_result_type__test_rand__param(false, false) && pass;
        pass = bool_result_type__test_rand__param(true, true) && pass;
        return pass;
    }
    //------------------------------------------------------------------
    bool bool_result_type__test_rand_max()
    {
        static_assert(std::is_same_v<bool, decltype(tbx::rand_max<bool>())>, "");
        auto const pass1{ tbx::rand_max<bool>() == true };
        assert(pass1);
        auto const pass2{ tbx::rand_max<bool>() == std::numeric_limits<bool>::max() };
        assert(pass2);
        return pass1 && pass2;
    }
}
//======================================================================
// Test Routines - Omitted ResultType
//======================================================================
namespace
{
    bool omit_result_type__test_rand()
    {
        using ResultType = int;
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand())>, "");
        enum : int { n_trials = 1000, failure_threshold = n_trials / 4 };

        auto pass1{ true };
        auto const r1 = tbx::rand();
        int repeat_count{}, zero_count{};
        for (int i{ n_trials }; i--;)
        {
            auto const r2{ tbx::rand() };
            if (r2 == r1) ++repeat_count;
            if (r2 == 0) ++zero_count;

            auto const lower_bound_checks{ 0 <= r2 };
            assert(lower_bound_checks);
            pass1 = lower_bound_checks && pass1;

            auto const upper_bound_checks{ r2 <= tbx::rand_max() };
            assert(upper_bound_checks);
            pass1 = upper_bound_checks && pass1;
        }
        // Admittedly, (n_trials/4) is not much of a threshold. 
        // Nevertheless, these tests demonstrate that output from rand() 
        // does vary.
        auto const pass2{ repeat_count < failure_threshold };
        assert(pass2);
        auto const pass3{ zero_count < failure_threshold };
        assert(pass3);
        return pass1 && pass2 && pass3;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_rand__a_b(int const a, int const b)
    {
        using ResultType = int;
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand(a, b))>, "");
        enum : int { n_trials = 1000 };

        auto const aa{ b < a ? b : a };
        auto const bb{ a < b ? b : a };
        assert(aa <= bb);

        auto pass{ true };
        for (int i{ n_trials }; i--;)
        {
            auto const r{ tbx::rand(a, b) };

            auto const lower_bound_checks{ aa <= r };
            assert(lower_bound_checks);
            pass = lower_bound_checks && pass;

            auto const upper_bound_checks{ r <= bb };
            assert(upper_bound_checks);
            pass = upper_bound_checks && pass;
        }
        if (a != b)
        {
            auto output_varies{ false };
            auto const r{ tbx::rand(a, b) };
            for (int i{ n_trials }; i--;)
                if (tbx::rand(a, b) != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_rand__a_b()
    {
        using ResultType = int;
        auto pass{ true };
        pass = ::omit_result_type__test_rand__a_b(1, 6) && pass;    // one "normal" case
        pass = ::omit_result_type__test_rand__a_b(-1, +1) && pass;  // another fairly "normal" case
        pass = ::omit_result_type__test_rand__a_b(                  // all the rest are "edge" cases
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::max()) && pass;
        pass = ::omit_result_type__test_rand__a_b(
            std::numeric_limits<ResultType>::max(),
            std::numeric_limits<ResultType>::min()) && pass;
        pass = ::omit_result_type__test_rand__a_b(
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::min()) && pass;
        pass = ::omit_result_type__test_rand__a_b(
            tbx::rand_max(),
            tbx::rand_max()) && pass;
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_rand__param(int const a, int const b)
    {
        using ResultType = int;
        tbx::param_type<ResultType> p{ a, b };
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand(p))>, "");
        enum : int { n_trials = 1000 };

        auto pass{ true };
        for (int i{ n_trials }; i--;)
        {
            auto const r{ tbx::rand(p) };

            auto const lower_bound_checks{ a <= r };
            assert(lower_bound_checks);
            pass = lower_bound_checks && pass;

            auto const upper_bound_checks{ r <= b };
            assert(upper_bound_checks);
            pass = upper_bound_checks && pass;
        }
        if (a != b)
        {
            auto output_varies{ false };
            auto const r{ tbx::rand(p) };
            for (int i{ n_trials }; i--;)
                if (tbx::rand(p) != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_rand__param()
    {
        using ResultType = int;
        auto pass{ true };
        pass = ::omit_result_type__test_rand__param(1, 6) && pass;    // a couple of "normal" cases
        pass = ::omit_result_type__test_rand__param(-1, +1) && pass;
        pass = ::omit_result_type__test_rand__param(                  // the  rest are "edge" cases
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::max()) && pass;
        pass = ::omit_result_type__test_rand__param(
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::min()) && pass;
        pass = ::omit_result_type__test_rand__param(
            tbx::rand_max(),
            tbx::rand_max()) && pass;
        return true;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_rand_max()
    {
        using Resulttype = int;
        static_assert(std::is_same_v<Resulttype, decltype(tbx::rand_max())>, "");
        auto const pass1{ tbx::rand_max() == std::uniform_int_distribution<Resulttype>{}.max() };
        assert(pass1);
        auto const pass2{ tbx::rand_max() == std::numeric_limits<Resulttype>::max() };
        assert(pass2);
        return pass1 && pass2;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_srand()
    {
        static_assert(std::is_same_v<void, decltype(tbx::srand())>, "");
        tbx::srand();
        auto const r{ tbx::rand() };
        enum : int { n_trials = 1000 };
        for (int i{ n_trials }; i--;)
        {
            tbx::srand();
            if (r != tbx::rand())
                return true;
        }
        // We called srand() to chose a random seed 1000 times. 
        // Each time, however, the first call to rand() thereafter 
        // returned the same value. We assume, therefore, that the 
        // seeding is not random, and we failed this test.
        assert(false);
        return false;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_srand__seed()
    {
        using ResultType = int;
        enum : std::size_t { n_trials = 42u };
        std::array<ResultType, n_trials> values;

        using seed_type = typename std::mt19937::result_type;
        seed_type const arbitrary_seed{ std::random_device{}() };
        static_assert(std::is_same_v<void, decltype(tbx::srand(arbitrary_seed))>, "");

        tbx::srand(arbitrary_seed);
        for (auto& v : values)
            v = tbx::rand();

        auto pass{ true };
        tbx::srand(arbitrary_seed);
        for (auto const& v : values)
            if (v != tbx::rand())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_srand__seed_seq()
    {
        using ResultType = int;
        enum : std::size_t { n_trials = 42u };
        std::array<ResultType, n_trials> values;

        std::seed_seq const arbitrary_seed_seq{ 1, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
        static_assert(std::is_same_v<void, decltype(tbx::srand(arbitrary_seed_seq))>, "");

        tbx::srand(arbitrary_seed_seq);
        for (auto& v : values)
            v = tbx::rand();

        auto pass{ true };
        tbx::srand(arbitrary_seed_seq);
        for (auto const& v : values)
            if (v != tbx::rand())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_thread_local()
    {
        using ResultType = int;
        enum : std::size_t { n_values = 42u };
        std::array<ResultType, n_values> values;

        using seed_type = typename std::mt19937::result_type;
        seed_type const arbitrary_seed{ std::random_device{}() };
        tbx::srand(arbitrary_seed);  // Nothing in thread_A should disturb this seeding.

        std::thread thread_A([&]()
            {
                tbx::srand(arbitrary_seed);
                for (auto& v : values)
                    v = tbx::rand();
            }
        );
        thread_A.join();

        auto pass{ true };
        for (auto const& v : values)
            if (v != tbx::rand())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    bool omit_result_type__test_unseeded_first_use()
    {
        // Verify that rand() is implicitly seeded as if srand(1u) had 
        // been called. This test generates some values using an unseed 
        // rand(), and then verifies that they match what you get after 
        // seeding with srand(1u).
        auto pass{ true };
        std::thread rand_thread([&]()
            {
                using ResultType = int;
                enum : std::size_t { n_trials = 42u };
                std::array<ResultType, n_trials> values;
                for (auto& v : values)
                    v = tbx::rand();
                using seed_type = typename std::mt19937::result_type;
                enum : seed_type { zero, one, default_seed = one };
                tbx::srand(default_seed);
                for (auto const& v : values)
                    if (v != tbx::rand())
                    {
                        pass = false;
                        break;
                    }
            }
        );
        rand_thread.join();
        assert(pass);
        return pass;
    }
}
//======================================================================
// Test Routines - Variable ResultType
//======================================================================
namespace
{
    template <typename ResultType>
    bool vary_result_type__test_rand()
    {
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand<ResultType>())>, "");
        enum : int { n_trials = 1000, failure_threshold = n_trials / 4 };

        auto pass1{ true };
        auto const r1 = tbx::rand<ResultType>();
        int r1_count{}, zero_count{};
        for (int i{ n_trials }; i--;)
        {
            auto const r2{ tbx::rand<ResultType>() };
            if (r2 == r1) ++r1_count;
            if (r2 == 0) ++zero_count;

            auto const lower_bound_checks{ ResultType{} <= r2 };
            assert(lower_bound_checks);
            pass1 = lower_bound_checks && pass1;

            auto const upper_bound_checks
            {
                std::is_floating_point_v<ResultType>
                ? r2 < tbx::rand_max<ResultType>()
                : r2 <= tbx::rand_max<ResultType>()
            };
            assert(upper_bound_checks);
            pass1 = upper_bound_checks && pass1;
        }
        // Admittedly, (n_trials/4) is not much of a threshold. 
        // Nevertheless, these tests demonstrate that output from rand() 
        // does vary.
        auto const pass2{ r1_count < failure_threshold };
        assert(pass2);
        auto const pass3{ zero_count < failure_threshold };
        assert(pass3);
        return pass1 && pass2 && pass3;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_rand__a_b(ResultType const a, ResultType const b)
    {
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand<ResultType>(a, b))>, "");
        enum : int { n_trials = 1000 };

        auto const aa{ b < a ? b : a };
        auto const bb{ a < b ? b : a };
        assert(aa <= bb);

        auto pass{ true };
        for (int i{ n_trials }; i--;)
        {
            auto const r{ tbx::rand<ResultType>(a, b) };

            auto const lower_bound_checks{ aa <= r };
            assert(lower_bound_checks);
            pass = lower_bound_checks && pass;

            auto const upper_bound_checks
            {
                std::is_floating_point_v<ResultType>
                ? r < bb
                : r <= bb
            };
            assert(upper_bound_checks);
            pass = upper_bound_checks && pass;
        }
        if (a != b)
        {
            auto output_varies{ false };
            auto const r{ tbx::rand<ResultType>(a, b) };
            for (int i{ n_trials }; i--;)
                if (tbx::rand<ResultType>(a, b) != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_rand__a_b()
    {
        auto pass{ true };
        pass = ::vary_result_type__test_rand__a_b<ResultType>(  // a few "normal" cases
            static_cast<ResultType>(1),
            static_cast<ResultType>(6)) && pass;
        pass = ::vary_result_type__test_rand__a_b<ResultType>(
            static_cast<ResultType>('a'),
            static_cast<ResultType>('z')) && pass;
        if /* constexpr */ (!std::is_unsigned_v<ResultType>)
        {
            pass = ::vary_result_type__test_rand__a_b<ResultType>(
                static_cast<ResultType>(-1),
                static_cast<ResultType>(+1)) && pass;
        }
        pass = ::vary_result_type__test_rand__a_b<ResultType>(  // all the rest are "edge" cases
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::max()) && pass;
        pass = ::vary_result_type__test_rand__a_b<ResultType>(
            std::numeric_limits<ResultType>::max(),
            std::numeric_limits<ResultType>::min()) && pass;
        if /* constexpr */ (!std::is_floating_point_v<ResultType>)
        {
            pass = ::vary_result_type__test_rand__a_b<ResultType>(
                std::numeric_limits<ResultType>::max(),
                std::numeric_limits<ResultType>::max()) && pass;
            pass = ::vary_result_type__test_rand__a_b<ResultType>(
                tbx::rand_max<ResultType>(),
                tbx::rand_max<ResultType>()) && pass;
        }
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_rand__param(ResultType const a, ResultType const b)
    {
        tbx::param_type<ResultType> p{ a, b };
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand<ResultType>(p))>, "");
        enum : int { n_trials = 1000 };

        auto pass{ true };
        for (int i{ n_trials }; i--;)
        {
            auto const r{ tbx::rand<ResultType>(p) };

            auto const lower_bound_checks{ a <= r };
            assert(lower_bound_checks);
            pass = lower_bound_checks && pass;

            auto const upper_bound_checks{ r <= b };
            assert(upper_bound_checks);
            pass = upper_bound_checks && pass;
        }
        if (a != b)
        {
            auto output_varies{ false };
            auto const r{ tbx::rand<ResultType>(p) };
            for (int i{ n_trials }; i--;)
                if (tbx::rand<ResultType>(p) != r)
                {
                    output_varies = true;
                    break;
                }
            assert(output_varies);
            pass = pass && output_varies;
        }
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_rand__param()
    {
        auto pass{ true };
        pass = ::vary_result_type__test_rand__param<ResultType>(      // a couple of "normal" cases, ...
            static_cast<ResultType>(1),
            static_cast<ResultType>(6)) && pass;
        if /* constexpr */ (!std::is_unsigned_v<ResultType>)
        {
            pass = ::vary_result_type__test_rand__param<ResultType>(
                static_cast<ResultType>(-1),
                static_cast<ResultType>(+1)) && pass;
        }
        pass = ::vary_result_type__test_rand__param<ResultType>(      // and the the rest are all "edge" cases
            std::numeric_limits<ResultType>::min(),
            std::numeric_limits<ResultType>::max()) && pass;
        if /* constexpr */ (!std::is_floating_point_v<ResultType>)
        {
            pass = ::vary_result_type__test_rand__param<ResultType>(
                std::numeric_limits<ResultType>::max(),
                std::numeric_limits<ResultType>::max()) && pass;
            pass = ::vary_result_type__test_rand__param<ResultType>(
                tbx::rand_max<ResultType>(),
                tbx::rand_max<ResultType>()) && pass;
        }
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_rand_max()
    {
        static_assert(std::is_same_v<ResultType, decltype(tbx::rand_max<ResultType>())>, "");
        auto const pass1
        {
            tbx::is_bool_or_char_v<ResultType>
            ? true
            : tbx::rand_max<ResultType>() == typename tbx::param_type<ResultType>::distribution_type{}.max()
        };
        assert(pass1);
        auto const pass2
        {
            std::is_floating_point_v<ResultType>
            ? tbx::rand_max<ResultType>() == static_cast<ResultType>(1.0)
            : tbx::rand_max<ResultType>() == std::numeric_limits<ResultType>::max()
        };
        assert(pass2);
        return pass1 && pass2;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_srand()
    {
        static_assert(std::is_same_v<void, decltype(tbx::srand<ResultType>())>, "");
        tbx::srand<ResultType>();
        auto const r{ tbx::rand<ResultType>() };
        enum : int { n_trials = 1000 };
        for (int i{ n_trials }; i--;)
        {
            tbx::srand<ResultType>();
            if (r != tbx::rand<ResultType>())
                return true;
        }
        // We called srand() to chose a random seed 1000 times. 
        // Each time, however, the first call to rand() thereafter 
        // returned the same value. We assume, therefore, that the 
        // seeding is not random, and we failed this test.
        assert(false);
        return false;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_srand__seed()
    {
        using seed_type = typename std::mt19937::result_type;
        seed_type const arbitrary_seed{ std::random_device{}() };
        static_assert(std::is_same_v<void, decltype(tbx::srand<ResultType>(arbitrary_seed))>, "");

        enum : std::size_t { n_trials = 42u };
        std::array<ResultType, n_trials> values{};

        tbx::srand<ResultType>(arbitrary_seed);
        for (auto& v : values)
            v = tbx::rand<ResultType>();

        auto pass{ true };
        tbx::srand<ResultType>(arbitrary_seed);
        for (auto const& v : values)
            if (v != tbx::rand<ResultType>())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_srand__seed_seq()
    {
        std::seed_seq const arbitrary_seed_seq{ 1, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
        static_assert(std::is_same_v<void, decltype(tbx::srand<ResultType>(arbitrary_seed_seq))>, "");

        enum : std::size_t { n_trials = 42u };
        std::array<ResultType, n_trials> values{};

        tbx::srand<ResultType>(arbitrary_seed_seq);
        for (auto& v : values)
            v = tbx::rand<ResultType>();

        auto pass{ true };
        tbx::srand<ResultType>(arbitrary_seed_seq);
        for (auto const& v : values)
            if (v != tbx::rand<ResultType>())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_thread_local()
    {
        enum : std::size_t { n_values = 42u };
        std::array<ResultType, n_values> values;

        using seed_type = typename std::mt19937::result_type;
        seed_type const arbitrary_seed{ std::random_device{}() };
        tbx::srand<ResultType>(arbitrary_seed);  // Nothing in thread_A should disturb this seeding.

        std::thread thread_A([&]()
            {
                tbx::srand<ResultType>(arbitrary_seed);
                for (auto& v : values)
                    v = tbx::rand<ResultType>();
            }
        );
        thread_A.join();

        auto pass{ true };
        for (auto const& v : values)
            if (v != tbx::rand<ResultType>())
            {
                pass = false;
                break;
            }
        assert(pass);
        return pass;
    }
    //------------------------------------------------------------------
    template <typename ResultType>
    bool vary_result_type__test_unseeded_first_use()
    {
        // Verify that rand() is implicitly seeded as if srand(1u) had 
        // been called. This test generates some values using an unseed 
        // rand(), and then verifies that they match what you get after 
        // seeding with srand(1u).
        auto pass{ true };
        std::thread rand_thread([&]()
            {
                enum : std::size_t { n_trials = 42u };
                std::array<ResultType, n_trials> values;
                for (auto& v : values)
                    v = tbx::rand<ResultType>();
                using seed_type = typename std::mt19937::result_type;
                tbx::srand<ResultType>(seed_type{ 1u });
                for (auto const& v : values)
                    if (tbx::rand<ResultType>() != v)
                    {
                        pass = false;
                        assert(pass);
                        break;
                    }
            });
        rand_thread.join();
        return pass;
    }
}
//======================================================================
// Driver Routine - Boolean ResultType 
//======================================================================
namespace
{
    template <typename charT, typename traits>
    bool bool_result_type(std::basic_ostream<char, traits>& ost)
    {
        auto pass{ true };
        pass = ::bool_result_type__test_rand_max    () && pass;
        pass = ::bool_result_type__test_rand        () && pass;
        pass = ::bool_result_type__test_rand__a_b   () && pass;
        pass = ::bool_result_type__test_rand__param () && pass;

        pass = ::vary_result_type__test_srand              <bool>() && pass;
        pass = ::vary_result_type__test_srand__seed        <bool>() && pass;
        pass = ::vary_result_type__test_srand__seed_seq    <bool>() && pass;
        pass = ::vary_result_type__test_thread_local       <bool>() && pass;
        pass = ::vary_result_type__test_unseeded_first_use <bool>() && pass;
        ost << (pass ? "  pass : " : "  FAIL : ") << "bool \n";
        return pass;
    }
}
//======================================================================
// Driver Routine - Omitted ResultType 
//======================================================================
namespace
{
    template <typename charT, typename traits>
    bool omit_result_type(std::basic_ostream<charT, traits>& ost)
    {
        auto pass{ true };
        pass = ::omit_result_type__test_rand_max           () && pass;
        pass = ::omit_result_type__test_rand               () && pass;
        pass = ::omit_result_type__test_rand__a_b          () && pass;
        pass = ::omit_result_type__test_rand__param        () && pass;
        pass = ::omit_result_type__test_srand              () && pass;
        pass = ::omit_result_type__test_srand__seed        () && pass;
        pass = ::omit_result_type__test_srand__seed_seq    () && pass;
        pass = ::omit_result_type__test_thread_local       () && pass;
        pass = ::omit_result_type__test_unseeded_first_use () && pass;
        ost << (pass ? "  pass : " : "  FAIL : ") << "omitted result_type \n";
        return pass;
    }
}
//======================================================================
// Driver Routines - Variable ResultType
//======================================================================
namespace
{
    template <typename charT, typename traits, typename ResultType>
    bool vary_result_type(
        std::basic_ostream<charT, traits>& ost,
        std::string const& name_result_type)
    {
        auto pass{ true };
        pass = ::vary_result_type__test_rand_max           <ResultType>() && pass;
        pass = ::vary_result_type__test_rand               <ResultType>() && pass;
        pass = ::vary_result_type__test_rand__a_b          <ResultType>() && pass;
        pass = ::vary_result_type__test_rand__param        <ResultType>() && pass;
        pass = ::vary_result_type__test_srand              <ResultType>() && pass;
        pass = ::vary_result_type__test_srand__seed        <ResultType>() && pass;
        pass = ::vary_result_type__test_srand__seed_seq    <ResultType>() && pass;
        pass = ::vary_result_type__test_thread_local       <ResultType>() && pass;
        pass = ::vary_result_type__test_unseeded_first_use <ResultType>() && pass;
        ost << (pass ? "  pass : " : "  FAIL : ") << name_result_type << '\n';
        return pass;
    }
    //------------------------------------------------------------------
    template <typename charT, typename traits>
    bool vary_result_type(std::basic_ostream<charT, traits>& ost)
    {
        auto pass{ true };
        pass = ::vary_result_type<charT, traits, char               >(ost, "char"              ) && pass;
        ost.put('\n');
        pass = ::vary_result_type<charT, traits, signed char        >(ost, "signed char"       ) && pass;
        pass = ::vary_result_type<charT, traits, short              >(ost, "short"             ) && pass;
        pass = ::vary_result_type<charT, traits, int                >(ost, "int"               ) && pass;
        pass = ::vary_result_type<charT, traits, long               >(ost, "long"              ) && pass;
        pass = ::vary_result_type<charT, traits, long long          >(ost, "long long"         ) && pass;
        ost.put('\n');
        pass = ::vary_result_type<charT, traits, unsigned char      >(ost, "unsigned char"     ) && pass;
        pass = ::vary_result_type<charT, traits, unsigned short     >(ost, "unsigned short"    ) && pass;
        pass = ::vary_result_type<charT, traits, unsigned int       >(ost, "unsigned int"      ) && pass;
        pass = ::vary_result_type<charT, traits, unsigned long      >(ost, "unsigned long"     ) && pass;
        pass = ::vary_result_type<charT, traits, unsigned long long >(ost, "unsigned long long") && pass;
        ost.put('\n');
        pass = ::vary_result_type<charT, traits, float              >(ost, "float"             ) && pass;
        pass = ::vary_result_type<charT, traits, double             >(ost, "double"            ) && pass;
        pass = ::vary_result_type<charT, traits, long double        >(ost, "long double"       ) && pass;
        ost.put('\n');
        pass = ::vary_result_type<charT, traits, std::int8_t        >(ost, "std::int8_t"       ) && pass;
        pass = ::vary_result_type<charT, traits, std::int16_t       >(ost, "std::int16_t"      ) && pass;
        pass = ::vary_result_type<charT, traits, std::int32_t       >(ost, "std::int32_t"      ) && pass;
        pass = ::vary_result_type<charT, traits, std::int64_t       >(ost, "std::int64_t"      ) && pass;
        ost.put('\n');
        pass = ::vary_result_type<charT, traits, std::uint8_t       >(ost, "std::uint8_t"      ) && pass;
        pass = ::vary_result_type<charT, traits, std::uint16_t      >(ost, "std::uint16_t"     ) && pass;
        pass = ::vary_result_type<charT, traits, std::uint32_t      >(ost, "std::uint32_t"     ) && pass;
        pass = ::vary_result_type<charT, traits, std::uint64_t      >(ost, "std::uint64_t"     ) && pass;
        ost.put('\n');
        return pass;
    }
}
//======================================================================
// Driver Routine - All Tests
//======================================================================
namespace tbx
{
    template <typename charT, typename traits>
    bool rand_unit_tests
    (
        std::basic_ostream<charT, traits>& ost,
        bool run_all_tests
    )
    {
        auto pass{ true };
        ost << "Unit Tests - tbx.rand.h \n";
        if (run_all_tests)
        {
            pass = omit_result_type(ost) && pass;
            pass = bool_result_type<charT, traits>(ost) && pass;
            pass = vary_result_type(ost) && pass;
        }
        else
        {
            // During development, put isolated tests here.
            pass = vary_result_type<charT, traits, int>(ost, "int") && pass;
        }
        ost << (pass ? "  pass" : "  FAIL")
            << " : all tests\n\n";
        return pass;
    }
}
//----------------------------------------------------------------------
template
bool tbx::rand_unit_tests<char, std::char_traits<char>>
(std::basic_ostream<char, std::char_traits<char>>&, bool const);
//----------------------------------------------------------------------
// end file: tbx.rand.unit_tests.cpp
