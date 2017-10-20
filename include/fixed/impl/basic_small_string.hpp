#ifndef BASIC_SMALL_STRING
#define BASIC_SMALL_STRING

#include "fixed_def.hpp"
#include <string>

namespace fixed
{
namespace _impl
{
    // will use the last char to hold the size of the string
    template <class CHAR_T, size_t SIZE,
        class Traits = std::char_traits<CHAR_T>>
    class basic_small_string
    {
    };
}
}

#endif //! BASIC_SMALL_STRING