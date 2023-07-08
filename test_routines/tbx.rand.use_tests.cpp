#pragma once
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
// Test routines
//======================================================================
namespace
{
	template <typename charT, typename traits>
	bool display_dice_rolls
	(
		std::basic_ostream<charT, traits>& ost
		, int const n_rolls = 60'000
		, bool show_every_roll = false
	)
	{
		ost << n_rolls << " Dice Rolls:\n";
		std::array<int, 6> count{};
		for (auto i{ n_rolls }; i--;)
		{
			auto const r = tbx::rand(1, 6);
			if (show_every_roll)
				ost << r << ' ';
			++count[r - 1];
		}
		ost << (show_every_roll ? "\n\n" : "") << "Counts:\n";
		auto die{ 0 };
		for (auto const& c : count)
		{
			ost << ++die << " : " << c << "\n";
		}
		ost << "Expected : " << n_rolls / 6.0 << " each\n\n";
		return true;
	}
}
//----------------------------------------------------------------------
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
				ost << std::setw(col_width) << *v++ << ' ';
			ost.put('\n');
		}
		ost.put('\n');
		return true;
	}
	//------------------------------------------------------------------
	template <typename charT, typename traits, typename ResultType>
	bool display_variates
	(
		std::basic_ostream<charT, traits>& ost
		, std::string const& name_result_type
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
			});
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
		std::basic_ostream<charT, traits>& ost
		, std::string const& name_result_type
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
		pass = ::test_result_type<charT, traits, std::int16_t  >(ost, "std::int16_t") && pass;
		pass = ::test_result_type<charT, traits, std::int32_t  >(ost, "std::int32_t") && pass;
		pass = ::test_result_type<charT, traits, std::int64_t  >(ost, "std::int64_t") && pass;

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
		pass = ::display_dice_rolls(ost, 600, true) && pass;
		pass = ::display_dice_rolls(ost, 60'000, false) && pass;
		return pass;
	}
}
//----------------------------------------------------------------------
template
bool tbx::rand_use_tests<char, std::char_traits<char>>
(std::basic_ostream<char, std::char_traits<char>>&);
//----------------------------------------------------------------------
// end file: tbx.rand.use_tests.cpp
