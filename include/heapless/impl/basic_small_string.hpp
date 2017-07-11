#ifndef BASIC_SMALL_STRING
#define BASIC_SMALL_STRING

#include <cstddef>
#include <string>

namespace heapless
{
	namespace _impl
	{
		//will use the last char to hold the size of the string
		template <class CHAR_T
			, std::size_t SIZE
			, class Traits = std::char_traits<CHAR_T>>
			class basic_small_string
		{

		};
	}
}

#endif //!BASIC_SMALL_STRING