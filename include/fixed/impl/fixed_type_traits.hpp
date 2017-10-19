#ifndef FIXED_VOID_T_COMPATIBILITY_HPP
#define FIXED_VOID_T_COMPATIBILITY_HPP

#include <type_traits>
#include <iterator>

namespace fixed
{
namespace astd
{
    template <class...> using void_t = void;

    template <class T> constexpr bool is_const_v = std::is_const<T>::value;

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

    template <typename T, class = void>
    struct is_iterator : public std::false_type
    {
    };

    template <typename T>
    struct is_iterator<T,
        fixed::astd::void_t<
            typename std::iterator_traits<T>::iterator_category>>
        : public std::true_type
    {
    };

    template <typename T> constexpr bool is_iterator_v = is_iterator<T>::value;
}
}

#endif //! FIXED_VOID_T_COMPATIBILITY_HPP
