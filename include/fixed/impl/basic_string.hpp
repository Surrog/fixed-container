#ifndef BASIC_HEAPLESS_STRING_HPP
#define BASIC_HEAPLESS_STRING_HPP

#include <cstddef>
#include <string>

#include "fixed/vector.hpp"

namespace fixed
{
	namespace _impl
	{
		template <class CHAR_T
			, std::size_t SIZE
			, class Traits = std::char_traits<CHAR_T>>
			class basic_string
		{
			fixed::vector<CHAR_T, SIZE> _data;

		};
	}
}

#endif //!HEAPLESS_STRING_HPP