#ifndef FIXED_TEST_FUNCTIONS_ITERATORS_HPP
#define FIXED_TEST_FUNCTIONS_ITERATORS_HPP

#include "fixed/impl/fixed_type_traits.hpp"

template <typename ITERATOR_TYPE> void test_not_const()
{
    static_assert(fixed::is_iterator<ITERATOR_TYPE>::value,
        "supposed to be an iterator");
    static_assert(!std::is_const<typename ITERATOR_TYPE::value_type>::value,
        "supposed to be not const");
    static_assert(!std::is_const<typename ITERATOR_TYPE::pointer>::value,
        "supposed to be not const");
    static_assert(!std::is_const<typename ITERATOR_TYPE::reference>::value,
        "supposed to be not const");
}

template <typename ITERATOR_TYPE> void test_is_const()
{
    static_assert(fixed::is_iterator<ITERATOR_TYPE>::value,
        "supposed to be an iterator");
    static_assert(!std::is_const<typename ITERATOR_TYPE::value_type>::value,
        "supposed to be const");
    static_assert(std::is_const<typename ITERATOR_TYPE::pointer>::value,
        "supposed to be const");
    static_assert(std::is_const<typename ITERATOR_TYPE::reference>::value,
        "supposed to be const");
}

template <typename LIT_T, typename RIT_T> void test_assign()
{
    RIT_T rval;
    LIT_T lval(rval);
    LIT_T lval2(RIT_T{});
}

#endif //! FIXED_TEST_FUNCTIONS_ITERATORS_HPP