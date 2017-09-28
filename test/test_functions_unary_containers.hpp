#ifndef FIXED_TEST_FUNCTIONS_UNARY_CONTAINERS_HPP
#define FIXED_TEST_FUNCTIONS_UNARY_CONTAINERS_HPP

#include "test_struct.hpp"

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_assign()
{
    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        auto expected = {1, 1, 1, 1, 1};
        l.assign(5, 1);
        CHECK(l.size() == 5);
        CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        auto expected = {1, 1, 1, 1, 1};
        l.assign(expected.begin(), expected.end());
        CHECK(l.size() == 5);
        CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        auto expected = {1, 1, 1, 1, 1};
        l.assign(expected);
        CHECK(l.size() == 5);
        CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        auto expected = {1, 2, 3, 4, 5};
        l.assign(expected.begin(), expected.end());
        CHECK(l.size() == 5);
        CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
        CHECK(l.front() == 1);
        CHECK(l.back() == 5);
        auto reverse_expected = {5, 4, 3, 2, 1};
        CHECK(std::equal(l.rbegin(), l.rend(), reverse_expected.begin(),
            reverse_expected.end()));
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        CHECK(l.empty());
        auto expected = {1, 2, 3, 4, 5};
        l.assign(expected.begin(), expected.end());
        CHECK(!l.empty());
        l.clear();
        CHECK(l.empty());
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_capacity()
{
    {
        CONTAINER_T<int, 10, Alloc_pattern> c;

        CHECK(c.empty());
        c = {10, 20, 30, 40, 50};
        CHECK(!c.empty());
        CHECK(c.size() == 5);
        c.clear();
        CHECK(c.empty());
        CHECK(c.max_size() >= 10);
        CHECK_THROWS(c.resize(c.max_size() + 1));
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_capacity()
{
    CONTAINER_T<int, 10, Alloc_pattern> c;

    c.shrink_to_fit();
    CHECK(c.capacity() >= 10);
    c.reserve(10);
    CHECK(c.capacity() >= 10);
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_insert()
{
    {
        CONTAINER_T<int, 30, Alloc_pattern> l;
        auto expected = {1, 2, 3, 4, 5};
        l.assign(expected);

        auto result = l.insert(l.begin(), 0);
        auto exp1 = {0, 1, 2, 3, 4, 5};
        CHECK(l.size() == exp1.size());
        CHECK(std::equal(l.begin(), l.end(), exp1.begin(), exp1.end()));
        CHECK(*result == 0);

        auto result2 = l.insert(l.end(), 6);
        auto exp2 = {0, 1, 2, 3, 4, 5, 6};
        CHECK(l.size() == exp2.size());
        CHECK(std::equal(l.begin(), l.end(), exp2.begin(), exp2.end()));
        CHECK(*result2 == 6);

        auto result3 = l.insert(l.begin() + 1, 10);
        auto exp3 = {0, 10, 1, 2, 3, 4, 5, 6};
        CHECK(l.size() == exp3.size());
        CHECK(std::equal(l.begin(), l.end(), exp3.begin(), exp3.end()));
        CHECK(*result3 == 10);
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l = {1, 2, 3, 4, 5};

        auto to_insert = {10, 11, 12};
        auto result = l.insert(l.end(), to_insert.begin(), to_insert.end());
        auto exp = {1, 2, 3, 4, 5, 10, 11, 12};
        CHECK(l.size() == exp.size());
        CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        CHECK(*result == 10);
    }

    {
        CONTAINER_T<int, 30, Alloc_pattern> l = {1, 2, 3, 4, 5};

        auto to_insert = {10, 11, 12};
        auto result = l.insert(l.end(), to_insert);
        auto exp = {1, 2, 3, 4, 5, 10, 11, 12};
        CHECK(l.size() == exp.size());
        CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        CHECK(*result == 10);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_erase()
{
    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};

        CHECK_THROWS(v.erase(v.end()));
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin());
        auto exp = {1, 2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 1);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 1);
        auto exp = {0, 2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 2);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 2);
        auto exp = {0, 1, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 3);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 3);
        auto exp = {0, 1, 2, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 4);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 4);
        auto exp = {0, 1, 2, 3};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(it == v.end());
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 0, v.begin() + 2);
        auto exp = {2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 2);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 1, v.begin() + 3);
        auto exp = {0, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 3);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 2, v.begin() + 4);
        auto exp = {0, 1, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 4);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::aligned_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 3, v.begin() + 5);
        auto exp = {0, 1, 2};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(it == v.end());
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l{0, 1, 2, 3, 4};
        auto result = l.erase(l.begin());
        auto exp = {1, 2, 3, 4};
        CHECK(l.size() == exp.size());
        CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        CHECK(*result == 1);

        while(l.size())
        {
            l.erase(l.begin());
        }

        CHECK_THROWS(l.erase(l.end()));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l{0, 1, 2, 3, 4};
        l.erase(l.begin(), l.begin() + 5);
        CHECK(l.empty());
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l{0, 1, 2, 3, 4};
        l.erase(l.begin() + 1, l.begin() + 5);
        auto exp = {0};
        CHECK(l.size() == exp.size());
        CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_resize()
{
    {
        int v = 0;
        {
            CONTAINER_T<test_construct, 5, Alloc_pattern> l;
            for(std::size_t i = 0; i < 5u; i++)
            {
                l.resize(i, test_construct(v));
                CHECK(l.size() == i);
                CHECK(v == int(i));
            }
        }
        CHECK(v == 0);
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l;
        for(std::size_t i = 0; i < 5u; i++)
        {
            l.resize(i);
            CHECK(l.size() == i);
        }
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l;
        CHECK_THROWS(l.resize(l.max_size() + 1));
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_push_front()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> l;
        l.push_front(0);
        {
            auto exp = {0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_front(1);
        {
            auto exp = {1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_front(2);
        {
            auto exp = {2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_front(3);
        {
            auto exp = {3, 2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_front(4);
        {
            auto exp = {4, 3, 2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        CHECK_THROWS(l.push_front(5));
    }

    {
        CONTAINER_T<test_move, 5, Alloc_pattern> l;
        test_move m("test");
        l.push_front(std::move(m));
        CHECK(m._val == nullptr);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_emplace_front()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> l;
        l.emplace_front(0);
        {
            auto exp = {0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_front(1);
        {
            auto exp = {1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_front(2);
        {
            auto exp = {2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_front(3);
        {
            auto exp = {3, 2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_front(4);
        {
            auto exp = {4, 3, 2, 1, 0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        CHECK_THROWS(l.emplace_front(5));
    }

    {
        unsigned int val = 1;
        CONTAINER_T<test_emplace_struct, 5, Alloc_pattern> l;
        l.emplace_front(2, 'c', &val);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_push_back()
{
    {
        CONTAINER_T<std::size_t, 5, Alloc_pattern> l;

        for(std::size_t i = 0; i < l.max_size(); i++)
        {
            l.push_back(i);
            CHECK(l.size() == i + 1);
            CHECK(l.back() == i);
        }
        CHECK_THROWS(l.push_back(0u));
    }

    {
        CONTAINER_T<std::size_t, 5, Alloc_pattern> l;
        l.push_back(0);
        {
            auto exp = {0u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_back(1);
        {
            auto exp = {0u, 1u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_back(2);
        {
            auto exp = {0u, 1u, 2u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_back(3);
        {
            auto exp = {0u, 1u, 2u, 3u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.push_back(4);
        {
            auto exp = {0u, 1u, 2u, 3u, 4u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
    }

    {
        CONTAINER_T<test_move, 5, Alloc_pattern> l;
        l.push_back(test_move("test"));
        CHECK(l.size() == 1);
        test_move val("toto");
        l.push_back(std::move(val));
        CHECK(val._val == nullptr);
        CHECK(l.size() == 2);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_emplace_back()
{
    {
        CONTAINER_T<std::size_t, 5, Alloc_pattern> l;

        for(std::size_t i = 0; i < l.max_size(); i++)
        {
            l.emplace_back(i);
            CHECK(l.size() == i + 1);
            CHECK(l.back() == i);
        }
        CHECK_THROWS(l.emplace_back(0u));
    }

    {
        CONTAINER_T<std::size_t, 5, Alloc_pattern> l;
        l.emplace_back(0);
        {
            auto exp = {0u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_back(1);
        {
            auto exp = {0u, 1u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_back(2);
        {
            auto exp = {0u, 1u, 2u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_back(3);
        {
            auto exp = {0u, 1u, 2u, 3u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace_back(4);
        {
            auto exp = {0u, 1u, 2u, 3u, 4u};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
    }

    {
        CONTAINER_T<test_move, 5, Alloc_pattern> l;
        l.emplace_back(test_move("test"));
        CHECK(l.size() == 1);
        test_move val("toto");
        l.emplace_back(std::move(val));
        CHECK(val._val == nullptr);
        CHECK(l.size() == 2);
    }

    {
        unsigned int val;
        CONTAINER_T<test_emplace_struct, 5, Alloc_pattern> l;
        l.emplace_back(0, 'c', &val);
        CHECK(l.size() == 1);
        test_emplace_struct exp{0, 'c', &val};
        CHECK(l.back() == exp);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_pop_back()
{
    {
        CONTAINER_T<std::size_t, 5, Alloc_pattern> l;

        for(std::size_t i = 0; i < l.max_size(); i++)
        {
            l.emplace_back(i);
            CHECK(l.size() == i + 1);
            CHECK(l.back() == i);
        }
        for(std::size_t i = 0; i < l.max_size(); i++)
        {
            CHECK(l.size() == 5 - i);
            CHECK(l.back() == 4 - i);
            l.pop_back();
        }
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> l = {0, 1, 2, 3, 4};
        l.pop_back();
        {
            auto exp = {0, 1, 2, 3};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.pop_back();
        {
            auto exp = {0, 1, 2};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.pop_back();
        {
            auto exp = {0, 1};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.pop_back();
        {
            auto exp = {0};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.pop_back();
        CHECK(l.size() == 0);
        CHECK_THROWS(l.pop_back());
    }

    {
        int val = 0;
        CONTAINER_T<test_construct, 5, Alloc_pattern> l(5, test_construct(val));
        CHECK(val == 5);

        for(int i = 0; i < 5; i++)
        {
            l.pop_back();
            CHECK(val == int(l.size()));
        }
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_swap()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{1, 2, 3};
        CONTAINER_T<int, 4, Alloc_pattern> rval{3, 4};

        auto exp1 = {1, 2, 3};
        auto exp2 = {3, 4};
        lval.swap(rval);

        CHECK(lval.size() == exp2.size());
        CHECK(std::equal(lval.begin(), lval.end(), exp2.begin(), exp2.end()));
        CHECK(rval.size() == exp1.size());
        CHECK(std::equal(rval.begin(), rval.end(), exp1.begin(), exp1.end()));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{1, 2, 3, 4, 5};
        CONTAINER_T<int, 4, Alloc_pattern> rval{3, 4};

        CHECK_THROWS(lval.swap(rval));
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_emplace()
{
    {
        CONTAINER_T<test, 5, Alloc_pattern> l
            = {test{"toto"}, test{"titi"}, test{"tata"}};

        l.emplace(l.begin() + 1, "test");
        auto exp = {test{"toto"}, test{"test"}, test{"titi"}, test{"tata"}};
        CHECK(l.size() == exp.size());
        CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
    }

    {
        unsigned int val;
        CONTAINER_T<test_emplace_struct, 5, Alloc_pattern> l
            = {test_emplace_struct{0, 'a', &val},
                test_emplace_struct{1, 'z', &val},
                test_emplace_struct{2, 'e', &val}};

        l.emplace(l.end(), 3, 'r', &val);
        {
            auto exp = {test_emplace_struct{0, 'a', &val},
                test_emplace_struct{1, 'z', &val},
                test_emplace_struct{2, 'e', &val},
                test_emplace_struct{3, 'r', &val}};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        l.emplace(l.end(), 4, 't', &val);
        {
            auto exp = {test_emplace_struct{0, 'a', &val},
                test_emplace_struct{1, 'z', &val},
                test_emplace_struct{2, 'e', &val},
                test_emplace_struct{3, 'r', &val},
                test_emplace_struct{4, 't', &val}};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        CHECK_THROWS(l.emplace(l.end(), 5, 'y', &val));
    }

    {
        CONTAINER_T<test_move, 5, Alloc_pattern> l;

        l.emplace(l.end(), test_move("test"));
        test_move mov("test");
        l.emplace(l.end(), std::move(mov));
    }

    {
        CONTAINER_T<int, 10, Alloc_pattern> l{1, 2, 4};
        auto result = l.emplace(l.begin() + 2, 3);
        {
            auto exp = {1, 2, 3, 4};
            CHECK(*result == 3);
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        result = l.emplace(l.begin() + 4, 5);
        {
            auto exp = {1, 2, 3, 4, 5};
            CHECK(*result == 5);
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
        result = l.emplace(l.begin(), 0);
        {
            auto exp = {0, 1, 2, 3, 4, 5};
            CHECK(*result == 0);
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
        }
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_constant_reference()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> l{0, 1, 2, 3, 4};
        auto& val = *(l.begin() + 4);
        CHECK(val == 4);
        l.erase(l.begin(), l.begin() + 4);
        {
            auto exp = {4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(val == 4);
        }
        l.push_front(0);
        auto& val0 = *(l.begin());
        {
            auto exp = {0, 4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(val == 4);
            CHECK(val0 == 0);
        }
        l.push_front(1);
        auto& val1 = *(l.begin());
        {
            auto exp = {1, 0, 4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(val == 4);
            CHECK(val0 == 0);
            CHECK(val1 == 1);
        }
    }

	{
		CONTAINER_T<int, 30, Alloc_pattern> l;
		auto init = { 1, 2, 3, 4, 5 };
		l.assign(init);

		auto& front = l.front();
		auto& next = *(l.begin() + 1);
		CHECK(front == 1);
		CHECK(next == 2);
		auto result = l.insert(l.begin() + 1, 2, 10);
		auto exp = { 1, 10, 10, 2, 3, 4, 5 };
		CHECK(l.size() == exp.size());
		CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
		CHECK(front == 1);
		CHECK(next == 2);
		auto& new_next = *(l.begin() + 1);
		CHECK(new_next == 10);
		CHECK(*result == 10);
	}

	{
		CONTAINER_T<int, 30, Alloc_pattern> l = { 1, 2, 3, 4, 5 };

		auto& pos0 = l.front();
		auto& pos1 = *(l.begin() + 1);
		auto& pos2 = *(l.begin() + 2);
		auto& pos3 = *(l.begin() + 3);

		CHECK(pos0 == 1);
		CHECK(pos1 == 2);
		CHECK(pos2 == 3);
		CHECK(pos3 == 4);

		auto to_insert = { 10, 11, 12 };
		auto result
			= l.insert(l.begin() + 2, to_insert.begin(), to_insert.end());
		auto exp = { 1, 2, 10, 11, 12, 3, 4, 5 };
		CHECK(l.size() == exp.size());
		CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));

		CHECK(pos0 == 1);
		CHECK(pos1 == 2);
		CHECK(pos2 == 3);
		CHECK(pos3 == 4);
		CHECK(*result == 10);
	}

}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_constant_iterator()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> l{0, 1, 2, 3, 4};
        auto val = (l.begin() + 4);
        CHECK(val == 4);
        l.erase(l.begin(), l.begin() + 4);
        {
            auto exp = {4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(*val == 4);
        }
        l.push_front(0);
        auto val0 = (l.begin());
        {
            auto exp = {0, 4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(*val == 4);
            CHECK(*val0 == 0);
        }
        l.push_front(1);
        auto val1 = (l.begin());
        {
            auto exp = {1, 0, 4};
            CHECK(l.size() == exp.size());
            CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
            CHECK(*val == 4);
            CHECK(*val0 == 0);
            CHECK(*val1 == 1);
        }
    }
	{
		CONTAINER_T<int, 30, Alloc_pattern> l;
		auto init = { 1, 2, 3, 4, 5 };
		l.assign(init);

		auto& front = l.front();
		auto next = (l.begin() + 1);
		CHECK(front == 1);
		CHECK(next == 2);
		auto result = l.insert(l.begin() + 1, 2, 10);
		auto exp = { 1, 10, 10, 2, 3, 4, 5 };
		CHECK(l.size() == exp.size());
		CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));
		CHECK(front == 1);
		CHECK(*next == 2);
		auto new_next = (l.begin() + 1);
		CHECK(*new_next == 10);
		CHECK(*result == 10);
	}

	{
		CONTAINER_T<int, 30, Alloc_pattern> l = { 1, 2, 3, 4, 5 };

		auto& pos0 = l.front();
		auto pos1 = (l.begin() + 1);
		auto pos2 = (l.begin() + 2);
		auto pos3 = (l.begin() + 3);

		CHECK(pos0 == 1);
		CHECK(*pos1 == 2);
		CHECK(*pos2 == 3);
		CHECK(*pos3 == 4);

		auto to_insert = { 10, 11, 12 };
		auto result
			= l.insert(l.begin() + 2, to_insert.begin(), to_insert.end());
		auto exp = { 1, 2, 10, 11, 12, 3, 4, 5 };
		CHECK(l.size() == exp.size());
		CHECK(std::equal(l.begin(), l.end(), exp.begin(), exp.end()));

		CHECK(pos0 == 1);
		CHECK(*pos1 == 2);
		CHECK(*pos2 == 3);
		CHECK(*pos3 == 4);
		CHECK(*result == 10);
	}

}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_merge()
{
    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{1, 2, 3};
        CONTAINER_T<int, 4, Alloc_pattern> rval{3, 4};

        auto exp = {1, 2, 3, 3, 4};
        lval.merge(std::move(rval));

        CHECK(lval.size() == exp.size());
        CHECK(rval.size() == 0);
        CHECK(std::equal(lval.begin(), lval.end(), exp.begin(), exp.end()));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{1, 2, 3};

        auto exp = {1, 2, 3, 3, 4};
        lval.merge(CONTAINER_T<int, 4, Alloc_pattern>{3, 4});

        CHECK(lval.size() == exp.size());
        CHECK(std::equal(lval.begin(), lval.end(), exp.begin(), exp.end()));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{1, 2, 3};
        CONTAINER_T<int, 4, Alloc_pattern> rval{4, 5, 6};

        CHECK_THROWS(lval.merge(std::move(rval)));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{5, 2, 3};
        lval.merge(std::move(lval));
        auto exp = {5, 2, 3};
        CHECK(lval.size() == exp.size());
        CHECK(std::equal(lval.begin(), lval.end(), exp.begin(), exp.end()));
    }

    {
        CONTAINER_T<int, 5, Alloc_pattern> lval{3, 4, 5};
        CONTAINER_T<int, 5, Alloc_pattern> rval{1, 2};

        auto exp = {1, 2, 3, 4, 5};
        rval.merge(std::move(lval));

        CHECK(rval.size() == exp.size());
        CHECK(lval.size() == 0);
        CHECK(std::equal(rval.begin(), rval.end(), exp.begin(), exp.end()));
    }
}

#endif //! FIXED_TEST_FUNCTIONS_UNARY_CONTAINERS_HPP