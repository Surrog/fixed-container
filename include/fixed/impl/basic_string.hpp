#ifndef BASIC_HEAPLESS_STRING_HPP
#define BASIC_HEAPLESS_STRING_HPP

#include <string>

#include "fixed/vector.hpp"
#include "fixed/impl/fixed_def.hpp"

namespace fixed
{
	namespace _impl
	{
		template <class CHAR_T
			, size_type SIZE
			, class Traits = std::char_traits<CHAR_T>>
			class basic_string
		{
			fixed::vector<CHAR_T, SIZE> _data;

		};
	}
}

#endif //!HEAPLESS_STRING_HPP