#ifndef FIXED_DEFINITION_HPP
#define FIXED_DEFINITION_HPP

#include <cstddef>
#include <cstdint>
#include <exception>

namespace fixed
{
namespace _impl
{
#ifdef SIGNED_INT_SIZE_TYPE
    using container_size_type = std::int32_t;
#else
    using container_size_type = std::size_t;
#endif

#if !defined(NDEBUG) || defined(FIXED_CONTAINER_CHECK_BOUND)
#define FIXED_CHECK_INBOUND(ex)                                                \
    (!(ex)) ? throw std::out_of_range("Iterator out of range") : ((void)0)
#define FIXED_CHECK_BADRANGE(ex)                                               \
    (!(ex)) ? throw std::out_of_range("Bad iterator range") : ((void)0)
#define FIXED_CHECK_FULL(ex)                                                   \
    (!(ex)) ? throw std::out_of_range("Container full") : ((void)0)
#define FIXED_CHECK_EMPTY(ex)                                                  \
    (!(ex)) ? throw std::out_of_range("Empty container") : ((void)0)
#define FIXED_CHECK_CUSTOM(ex, msg)                                            \
    (!(ex)) ? throw std::out_of_range(msg) : ((void)0)

#else //! defined(NDEBUG) || defined(FIXED_CONTAINER_CHECK_BOUND)
#define FIXED_CHECK(ex) ((void)0)
#endif // defined(NDEBUG) && !defined(FIXED_CONTAINER_CHECK_BOUND)

    template <bool val> struct constexpr_if
    {
        template <typename CLOSURE_T> constexpr constexpr_if(const CLOSURE_T&)
        {
        }

        template <typename CLOSURE_T> constexpr void _else(const CLOSURE_T& f)
        {
            f();
        }
    };

    template <> struct constexpr_if<true>
    {
        template <typename CLOSURE_T> constexpr constexpr_if(const CLOSURE_T& f)
        {
            f();
        }

        template <typename CLOSURE_T> constexpr void _else(const CLOSURE_T&) {}
    };
}
}

#endif //! FIXED_DEFINITION_HPP