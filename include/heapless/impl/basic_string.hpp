#ifndef BASIC_HEAPLESS_STRING_HPP
#define BASIC_HEAPLESS_STRING_HPP

#include <cstddef>
#include <string>

#include "heapless/vector.hpp"

namespace heapless
{
	namespace _impl
	{
		template <class CHAR_T
			, std::size_t SIZE
			, class Traits = std::char_traits<CHAR_T>>
			class basic_string
		{
			heapless::vector<CHAR_T, SIZE> _data;

		};
	}
}

#endif //!HEAPLESS_STRING_HPP