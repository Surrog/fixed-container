
#include "catch.hpp"
#include "fixed/vector.hpp"
#include "test_function_unary.hpp"
#include "test_struct.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_constructor()
{
    {
        VECTOR_TYPE<test_emplace, 10, Alloc_pattern> test;
        std::vector<test_emplace> vec_ref;

        unsigned int array_test[] = {1u, 2u};
        test.push_back(test_emplace());
        vec_ref.push_back(test_emplace());
        test.push_back({30, 'b', array_test});
        vec_ref.push_back({30, 'b', array_test});

        VECTOR_TYPE<test_emplace, 10, Alloc_pattern> test2 = test;

        CHECK(std::equal(
            test.begin(), test.end(), vec_ref.begin(), vec_ref.end()));
        CHECK(std::equal(test.begin(), test.end(), test2.begin(), test2.end()));
    }

    {
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words1{
            "the", "frogurt", "is", "also", "cursed"};
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words1.begin(), words1.end(), expected.begin(),
                expected.end()));
        }

        // words2 == words1
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words2(
            words1.begin(), words1.end());
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words2.begin(), words2.end(), expected.begin(),
                expected.end()));
        }
    }

    {
        // c++11 initializer list syntax:
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words1{
            "the", "frogurt", "is", "also", "cursed"};
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words1.begin(), words1.end(), expected.begin(),
                expected.end()));
        }

        // words2 == words1
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words2(
            words1.begin(), words1.end());
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words2.begin(), words2.end(), expected.begin(),
                expected.end()));
        }

        // words3 == words1
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words3(words1);
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words3.begin(), words3.end(), expected.begin(),
                expected.end()));
        }
    }

    {
        // c++11 initializer list syntax:
        VECTOR_TYPE<std::string, 5, Alloc_pattern> words1{
            "the", "frogurt", "is", "also", "cursed"};
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words1.begin(), words1.end(), expected.begin(),
                expected.end()));
        }

        // words2 == words1
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words2(
            words1.begin(), words1.end());
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words2.begin(), words2.end(), expected.begin(),
                expected.end()));
        }

        // words3 == words1
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words3(words1);
        {
            auto expected = {"the", "frogurt", "is", "also", "cursed"};
            CHECK(std::equal(words3.begin(), words3.end(), expected.begin(),
                expected.end()));
        }

        // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
        VECTOR_TYPE<std::string, 10, Alloc_pattern> words4(5, "Mo");
        {
            auto expected = {"Mo", "Mo", "Mo", "Mo", "Mo"};
            CHECK(std::equal(words4.begin(), words4.end(), expected.begin(),
                expected.end()));
        }
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> nums1{3, 1, 4, 6, 5, 9};
        VECTOR_TYPE<int, 6, Alloc_pattern> nums2;
        VECTOR_TYPE<int, 20, Alloc_pattern> nums3;

        CHECK(nums1.size() == 6);
        CHECK(nums2.size() == 0);
        CHECK(nums3.size() == 0);
        // copy assignment copies data from nums1 to nums2
        nums2 = nums1;

        CHECK(nums1.size() == 6);
        CHECK(nums2.size() == 6);
        CHECK(nums3.size() == 0);
        // move assignment moves data from nums1 to nums3,
        // modifying both nums1 and nums3
        nums3 = std::move(nums1);

        CHECK(nums1.size() == 0);
        CHECK(nums2.size() == 6);
        CHECK(nums3.size() == 6);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_iterator()
{
    VECTOR_TYPE<int, 20, Alloc_pattern> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    using fiterator = typename VECTOR_TYPE<int, 20, Alloc_pattern>::iterator;
    using fciterator =
        typename VECTOR_TYPE<int, 20, Alloc_pattern>::const_iterator;

    {
        fiterator it;
        fciterator cit(it);

        CHECK(it == fiterator());
        CHECK(cit == fciterator());
    }

    {
        auto reverse = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        CHECK(std::equal(
            vec.rbegin(), vec.rend(), reverse.begin(), reverse.end()));
    }

    {
        CHECK(static_cast<std::size_t>(std::distance(vec.begin(), vec.end()))
            == vec.size());
    }

    {
        auto beg = vec.begin();
        auto end = vec.end();
        std::size_t i = 0;

        CHECK(beg != end);

        while(i < vec.size())
        {
            CHECK(beg < end);
            CHECK(*(beg + i) == vec[i]);
            CHECK((beg + i) != end);
            ++i;
        }
    }

    {
        auto beg = vec.begin();
        auto end = vec.end();

        CHECK(beg <= end);
        ++beg;
        CHECK(beg < end);
        beg++;
        CHECK(end > beg);
        beg = beg + 1;
        CHECK(end >= beg);
        CHECK(beg != end);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_modifier()
{
    {
        VECTOR_TYPE<int, 20, Alloc_pattern> vec(3, 100);
        std::vector<int> expected = {100, 100, 100};
        CHECK(vec == expected);

        auto it = vec.begin();
        it = vec.insert(it, 200);
        expected = {200, 100, 100, 100};
        CHECK(vec == expected);

        vec.insert(it, 2u, 300);
        expected = {300, 300, 200, 100, 100, 100};
        CHECK(vec == expected);

        it = vec.begin();
        std::vector<int> vec2(2, 400);
        vec.insert(it + 2, vec2.begin(), vec2.end());
        expected = {300, 300, 400, 400, 200, 100, 100, 100};
        CHECK(vec == expected);

        int arr[] = {501, 502, 503};
        CHECK_NOTHROW(vec.insert(vec.begin(), arr, arr + 3));
        expected = {501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100};
        CHECK(vec == expected);
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        c.erase(c.begin());
        expected.erase(expected.begin());

        CHECK(c == expected);

        expected.erase(expected.begin() + 2, expected.begin() + 5);
        c.erase(c.begin() + 2, c.begin() + 5);

        CHECK(c == expected);

        expected.erase(expected.begin() + (expected.size() - 1));
        c.erase(c.begin() + (c.size() - 1));
        CHECK(c.size() == expected.size());
        CHECK(c == expected);
    }

    {
        VECTOR_TYPE<char, 5, Alloc_pattern> characters;

        characters.assign(5, 'a');

        CHECK(characters.size() == 5);
        int iteration = 0;
        for(auto& c : characters)
        {
            CHECK(c == 'a');
            iteration++;
        }
        CHECK(iteration == 5);
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector;
        for(int i = 1; i <= 5; i++)
            myvector.push_back(i);

        int i = 1;
        for(typename VECTOR_TYPE<int, 10, Alloc_pattern>::iterator it
            = myvector.begin();
            it != myvector.end(); ++it)
        {
            CHECK(*it == i);
            i++;
        }
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector(5);

        typename VECTOR_TYPE<int, 10, Alloc_pattern>::reverse_iterator rbit
            = myvector.rbegin();
        typename VECTOR_TYPE<int, 10, Alloc_pattern>::reverse_iterator reit
            = myvector.rend();
        typename VECTOR_TYPE<int, 10, Alloc_pattern>::iterator it
            = myvector.begin() + (myvector.size() - 1);
        int i = 0;
        while(rbit != reit)
        {
            CHECK(&*it == &*rbit);
            --it;
            ++rbit;
            ++i;
        }
        CHECK(i == 5);
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector(
            5); // 5 default-constructed ints

        int i = 0;

        typename VECTOR_TYPE<int, 10, Alloc_pattern>::reverse_iterator rit
            = myvector.rbegin();
        for(; rit != myvector.rend(); ++rit)
            *rit = ++i;
        CHECK(i == 5);
        for(typename VECTOR_TYPE<int, 10, Alloc_pattern>::iterator it
            = myvector.begin();
            it != myvector.end(); ++it)
        {
            CHECK(*it == i);
            i--;
        }
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector = {10, 20, 30, 40, 50};

        auto expected = {10, 20, 30, 40, 50};
        int i = 0;
        for(auto it = myvector.cbegin(); it != myvector.cend(); ++it)
        {
            CHECK(*it == *(expected.begin() + i));
            i++;
        }
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector = {10, 20, 30, 40, 50};

        auto expected = {10, 20, 30, 40, 50};
        for(std::size_t i = 0; i < expected.size(); i++)
        {
            CHECK(myvector[i] == *(expected.begin() + i));
        }
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector = {10, 20, 30, 40, 50};

        auto expected = {10, 20, 30, 40, 50};
        for(std::size_t i = 0; i < expected.size(); i++)
        {
            CHECK(myvector.at(i) == *(expected.begin() + i));
        }

        CHECK(myvector.front() == 10);
        CHECK(myvector.back() == 50);
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector;

        CHECK(myvector.empty());
        myvector = {10, 20, 30, 40, 50};
        CHECK(!myvector.empty());
        CHECK(myvector.size() == 5);
        myvector.clear();
        CHECK(myvector.empty());
        CHECK(myvector.max_size() == 10);
        myvector.reserve(10);
        CHECK(myvector.capacity() == 10);
        myvector.shrink_to_fit();
    }

    {
        VECTOR_TYPE<int, 10, Alloc_pattern> myvector = {10, 20, 30, 40, 50};
        int* ptr = myvector.data();

        {
            auto beg = myvector.begin();
            auto end = myvector.end();
            typename VECTOR_TYPE<int, 10, Alloc_pattern>::size_type i = 0;

            while(beg != end)
            {
                CHECK(ptr + i == &*beg);
                ++i;
                ++beg;
            }
        }

        myvector.erase(myvector.begin() + 2);

        {
            auto beg = myvector.begin();
            auto end = myvector.end();
            typename VECTOR_TYPE<int, 10, Alloc_pattern>::size_type i = 0;

            while(beg != end)
            {
                CHECK(ptr + i == &*beg);
                ++i;
                ++beg;
            }
        }
    }

    test_erase<VECTOR_T, Alloc_pattern>();
    test_push_back<VECTOR_T, Alloc_pattern>();
    test_pop_back<VECTOR_T, Alloc_pattern>();
    test_emplace_back<VECTOR_T, Alloc_pattern>();
    test_resize<VECTOR_T, Alloc_pattern>();
}

TEST_CASE("testing basic_vector", "[linear]")
{
    test_vector_constructor<fixed::vector,
        fixed::_impl::basic_stack_allocator>();
    test_vector_modifier<fixed::vector, fixed::_impl::basic_stack_allocator>();
    test_vector_iterator<fixed::vector, fixed::_impl::basic_stack_allocator>();

    test_vector_constructor<fixed::vector,
        fixed::_impl::basic_heap_allocator>();
    test_vector_modifier<fixed::vector, fixed::_impl::basic_heap_allocator>();
    test_vector_iterator<fixed::vector, fixed::_impl::basic_heap_allocator>();
}