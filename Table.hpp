/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Aleksandr Lyapunov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <tuple>

inline const char spaces[] = "                                ";

inline const char* get_spaces(size_t count)
{
	return spaces + (sizeof(spaces) - 1) - count;
}

template <size_t S, class T>
struct Cell {
	const T& value;
	friend std::ostream& operator<<(std::ostream& strm, const Cell& c)
	{
		static_assert(S < sizeof(spaces));
		std::stringstream ss;
		ss << c.value;
		if (ss.str().size() >= S)
			return strm << ss.str();
		size_t padding = S - ss.str().size();
		size_t l = (padding + 1) / 2, r = padding / 2;
		return strm << get_spaces(l) << ss.str() << get_spaces(r);
	}
};

template <size_t S, class T>
Cell<S, T> cell(const T& t)
{
	return {t};
}

constexpr size_t widths[] = {5, 15, 15, 15, 15};

template <class... T>
struct Row {
	static_assert(sizeof...(T) <= std::size(widths));
	std::tuple<const T&...> values;

	template <size_t... I>
	std::ostream& out(std::ostream& strm, std::index_sequence<I...>) const
	{
		strm << '|';
		return ((strm << cell<widths[I]>(std::get<I>(values)) << '|'), ...);
	}

	friend std::ostream& operator<<(std::ostream& strm, const Row& c)
	{
		return c.out(strm, std::make_index_sequence<sizeof...(T)>{});
	}
};

template <class... T>
Row<T...> row(const T&... t)
{
	return {{t...}};
}

template <size_t N>
struct Separator {
	static_assert(N <= std::size(widths));
	friend std::ostream& operator<<(std::ostream& strm, const Separator&)
	{
		strm << '|';
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < widths[i]; j++)
				strm << '-';
			strm << '|';
		}

		return strm;
	}
};

template <size_t N>
Separator<N> separator()
{
	return {};
}