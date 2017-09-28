#ifndef FIXED_VOID_T_COMPATIBILITY_HPP
#define FIXED_VOID_T_COMPATIBILITY_HPP

#include <type_traits>

namespace fixed
{
namespace astd
{
    template <class...> using void_t = void;

    template <class T> constexpr bool is_const_v = std::is_const<T>::value;
}

template <typename T, class = void> struct is_iterator : public std::false_type
{
};

template <typename T>
struct is_iterator<T,
    fixed::astd::void_t<typename std::iterator_traits<T>::iterator_category>>
    : public std::true_type
{
};
}

#endif //! FIXED_VOID_T_COMPATIBILITY_HPP