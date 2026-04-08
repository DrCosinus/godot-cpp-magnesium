#pragma once

#include <cstddef>

template <std::size_t N>
struct StringLiteral
{
	const char value[N];
	static constexpr std::size_t size = N;

	constexpr StringLiteral(const char (&str)[N])
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			value[i] = str[i];
		}
	}
};

template <std::size_t N>
StringLiteral(const char (&str)[N]) -> StringLiteral<N>;
