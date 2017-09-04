#ifndef FIXED_DEFINITION_HPP
#define FIXED_DEFINITION_HPP

#include <cstddef>
#include <cstdint>

namespace fixed
{
	namespace _impl
	{
#ifdef SIGNED_INT_SIZE_TYPE
		using container_size_type = std::int32_t;
#else
		using container_size_type = std::size_t;
#endif
	}
}

#endif //!FIXED_DEFINITION_HPP