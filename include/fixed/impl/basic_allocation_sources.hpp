#ifndef FIXED_BASIC_ALLOCATION_SOURCES
#define FIXED_BASIC_ALLOCATION_SOURCES

#include "fixed_type_traits.hpp"
#include "fixed_def.hpp"
#include <type_traits>

namespace fixed
{
namespace _impl
{
    struct empty_source
    {
        typedef allocator_source_tag allocator_source;
    };
}
}

#endif // FIXED_BASIC_ALLOCATION_SOURCES