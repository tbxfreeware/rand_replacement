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

#include <algorithm>    // equal
#include <array>        // array
#include <cassert>      // assert
#include <cstdint>      // int16_t, int32_t, int64_t, uint16_t, uint32_t, uint64_t
#include <iomanip>      // setw
#include <iostream>     // basic_ostream
#include <string>       // char_traits, string
#include <thread>       // thread
#include <vector>       // cbegin, cend, vector

#include "tbx.rand.h"

//======================================================================
// display_coin_tosses
//======================================================================
namespace
{
    template <typename charT, typename traits>
    bool display_coin_tosses
    (
        std::basic_ostream<charT, traits>& ost
        , int const n_tosses
        , bool const show_detail = false
    )
    {
        ost << n_tosses << " Coin Tosses:\n\n";
        enum : std::size_t
        {
            tails         // 0u
            , heads       // 1u
            , n_outcomes  // 2u
        };
        std::array<int, n_outcomes> count{};
        auto const n_trials{ n_tosses - n_tosses % 100 }
            , n_cols{ 25 }
            , n_rows{ n_trials / n_cols };
        tbx::srand<bool>();
        for (int i{ n_rows }; i--;)
        {
            for (int j{ n_cols }; j--;)
            {
                auto const is_heads = tbx::rand<bool>();
                if (show_detail)
                    ost << (is_heads ? "heads " : "tails ");
                ++count[static_cast<std::size_t>(is_heads)];
            }
            if (show_detail)
                ost.put('\n');
        }
        if (show_detail)
            ost.put('\n');
        ost << "Counts:\n"
            << "  heads : " << count[heads] << '\n'
            << "  tails : " << count[tails] << '\n'
            << "Expected : " << n_trials / n_outcomes << " each\n\n";
        return true;
    }
}

//======================================================================
// display_dice_rolls
//======================================================================
namespace
{
    template <typename charT, typename traits>
    bool display_dice_rolls
    (
        std::basic_ostream<charT, traits>& ost
        , int const n_rolls
        , bool const show_detail = false
    )
    {
        ost << n_rolls << " Dice Rolls:\n\n";
        enum : std::size_t { zero, one, n_outcomes = 6u };
        std::array<int, n_outcomes> count{};
        auto const p{ tbx::param_type<int>(1, 6) };
        tbx::srand();
        for (auto i{ n_rolls }; i--;)
        {
            auto const r = tbx::rand(p);
            if (show_detail)
                ost << r << ' ';
            ++count[r - one];
        }
        if (show_detail)
            ost << "\n\n";
        ost << "Counts:\n";
        auto die{ 0 };
        for (auto const& c : count)
        {
            ost << "  " << ++die << " : " << c << "\n";
        }
        ost << "Expected : " << n_rolls / n_outcomes << " each\n\n";
        return true;
    }
}

//======================================================================
// display_random_ascii
//======================================================================
namespace
{
    template <typename result_type>
    auto printable_ascii_string(int const length)
    {
        std::string s;  // expecting NVRO
        tbx::param_type<int> p('!', '~');  // same as p(0x21, 0x7e)
        tbx::srand<result_type>();
        for (auto i{ length }; i--;)
            s.push_back(tbx::rand<result_type>(p));
        return s;
    }
    template <typename charT, typename traits, typename result_type>
    bool display_random_ascii
    (
        std::basic_ostream<charT, traits>& ost,
        std::string const& name_result_type
    )
    {
        ost << "Display Random ASCII - display_random_ascii<"
            << name_result_type
            << ">(ost, \""
            << name_result_type
            << "\") \n\n";
        enum : int
        {
            string_size = 20
            , gutter_width = 3
            , col_width = string_size + gutter_width
            , line_length = 166
            , n_rows = 3
            , n_cols = line_length / col_width
        };
        for (int i{ n_rows }; i--;)
        {
            for (int j{ n_cols }; j--;)
                ost << printable_ascii_string<result_type>(string_size)
                    << std::setw(gutter_width) << ' ';
            ost.put('\n');
        }
        ost.put('\n');
        return true;
    }
    template <typename charT, typename traits>
    bool display_random_ascii(std::basic_ostream<charT, traits>& ost)
    {
        auto pass{ true };
        pass = ::display_random_ascii<charT, traits, char>(ost, "char") && pass;
        pass = ::display_random_ascii<charT, traits, signed char>(ost, "signed char") && pass;
        pass = ::display_random_ascii<charT, traits, unsigned char>(ost, "unsigned char") && pass;
        return pass;
    }
}

//======================================================================
// display_variates
//======================================================================
namespace
{
    template <typename charT, typename traits, typename ResultType>
    bool display_variates
    (
        std::basic_ostream<charT, traits>& ost
        , std::string const& heading
        , std::vector<ResultType> const& variates
        , int const row_count
        , int const col_count
        , int const col_width = sizeof(ResultType) > 4 ? 21 : 11
    )
    {
        assert(variates.size() == row_count * col_count);
        ost << heading << '\n';
        auto v{ std::cbegin(variates) };
        for (auto i{ row_count }; i--;)
        {
            for (auto j{ col_count }; j--;)
                ost << std::setw(col_width) << +*v++ << ' ';
            ost.put('\n');
        }
        ost.put('\n');
        return true;
    }
    //------------------------------------------------------------------
    template <typename charT, typename traits, typename ResultType>
    bool display_variates
    (
        std::basic_ostream<charT, traits>& ost,
        std::string const& name_result_type
    )
    {
        auto pass{ true };
        ost << "Display variates - ResultType = "
            << name_result_type
            << "\n\n";
        std::string const name_rand{ "tbx::rand<" + name_result_type + ">()" };
        std::string const name_srand{ "tbx::srand<" + name_result_type + ">()" };
        std::string const name_srand_one{ "tbx::srand<" + name_result_type + ">(1u)" };
        std::string heading;
        int constexpr const row_count{ 2 };
        int constexpr const col_count{ sizeof(ResultType) > 4 ? 7 : 13 };
        int constexpr const count{ row_count * col_count };
        std::vector<ResultType> variates1(count), variates2(count), variates3(count), variates4(count);
        std::thread rand_thread([&]()
            {
                for (auto& v : variates1)
                    v = tbx::rand<ResultType>();

                tbx::srand<ResultType>(1u);
                for (auto& v : variates2)
                    v = tbx::rand<ResultType>();

                tbx::srand<ResultType>();
                for (auto& v : variates3)
                    v = tbx::rand<ResultType>();

                tbx::srand<ResultType>();
                for (auto& v : variates4)
                    v = tbx::rand<ResultType>();
            }
        );
        rand_thread.join();

        heading = name_rand + "   First use in new thread";
        pass = display_variates<charT, traits, ResultType>
            (ost, heading, variates1, row_count, col_count) && pass;

        heading = name_rand + "   Seeded with: " + name_srand_one;
        pass = display_variates<charT, traits, ResultType>
            (ost, heading, variates2, row_count, col_count) && pass;

        heading = name_rand + "   Seeded with: " + name_srand;
        pass = display_variates<charT, traits, ResultType>
            (ost, heading, variates3, row_count, col_count) && pass;

        heading = name_rand + "   Seeded with: " + name_srand;
        pass = display_variates<charT, traits, ResultType>
            (ost, heading, variates4, row_count, col_count) && pass;

        pass = std::equal(
            std::cbegin(variates1), std::cend(variates1),
            std::cbegin(variates2), std::cend(variates2)
        ) && pass;
        assert(pass);
        return pass;
    }
}
//======================================================================
// Driver routines
//======================================================================
namespace
{
    template <typename charT, typename traits, typename ResultType>
    bool test_result_type
    (
        std::basic_ostream<charT, traits>& ost, 
        std::string const& name_result_type
    )
    {
        auto pass{ true };
        pass = ::display_variates<charT, traits, ResultType>
            (ost, name_result_type) && pass;
        return pass;
    }
    //------------------------------------------------------------------
    template <typename charT, typename traits>
    bool vary_result_type(std::basic_ostream<charT, traits>& ost)
    {
        auto pass{ true };
        pass = ::test_result_type<charT, traits, std::int8_t   >(ost, "std::int8_t" ) && pass;
        pass = ::test_result_type<charT, traits, std::int16_t  >(ost, "std::int16_t") && pass;
        pass = ::test_result_type<charT, traits, std::int32_t  >(ost, "std::int32_t") && pass;
        pass = ::test_result_type<charT, traits, std::int64_t  >(ost, "std::int64_t") && pass;

        pass = ::test_result_type<charT, traits, std::uint8_t  >(ost, "std::uint8_t" ) && pass;
        pass = ::test_result_type<charT, traits, std::uint16_t >(ost, "std::uint16_t") && pass;
        pass = ::test_result_type<charT, traits, std::uint32_t >(ost, "std::uint32_t") && pass;
        pass = ::test_result_type<charT, traits, std::uint64_t >(ost, "std::uint64_t") && pass;

        pass = ::test_result_type<charT, traits, float         >(ost, "float") && pass;
        pass = ::test_result_type<charT, traits, double        >(ost, "double") && pass;
        pass = ::test_result_type<charT, traits, long double   >(ost, "long double") && pass;
        return pass;
    }
}
//----------------------------------------------------------------------
namespace tbx
{
    template <typename charT, typename traits>
    bool rand_use_tests(std::basic_ostream<charT, traits>& ost)
    {
        ost << "Use Tests - tbx.rand.h \n\n";
        auto pass{ true };
        pass = ::vary_result_type(ost) && pass;
        pass = ::display_random_ascii(ost) && pass;
        enum : bool { summary, detail };
        pass = ::display_coin_tosses(ost, 200, detail) && pass;
        pass = ::display_coin_tosses(ost, 20'000, summary) && pass;
        pass = ::display_dice_rolls(ost, 600, detail) && pass;
        pass = ::display_dice_rolls(ost, 60'000, summary) && pass;
        return pass;
    }
}
//----------------------------------------------------------------------
template
bool tbx::rand_use_tests<char, std::char_traits<char>>
(std::basic_ostream<char, std::char_traits<char>>&);
//----------------------------------------------------------------------
// end file: tbx.rand.use_tests.cpp
