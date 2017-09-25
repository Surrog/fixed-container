#ifndef FIXED_TEST_FUNCTION_UNARY_HPP
#define FIXED_TEST_FUNCTION_UNARY_HPP

#include "test_struct.hpp"

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename CONTAINER_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_erase()
{
    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};

        CHECK_THROWS(v.erase(v.end()));
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin());
        auto exp = {1, 2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 1);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 1);
        auto exp = {0, 2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 2);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 2);
        auto exp = {0, 1, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 3);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 3);
        auto exp = {0, 1, 2, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 4);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 4);
        auto exp = {0, 1, 2, 3};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(it == v.end());
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 0, v.begin() + 2);
        auto exp = {2, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 2);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 1, v.begin() + 3);
        auto exp = {0, 3, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 3);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
            = {0, 1, 2, 3, 4};
        auto it = v.erase(v.begin() + 2, v.begin() + 4);
        auto exp = {0, 1, 4};
        CHECK(v.size() == exp.size());
        CHECK(std::equal(v.begin(), v.end(), exp.begin(), exp.end()));
        CHECK(*it == 4);
    }

    {
        CONTAINER_T<int, 10, fixed::_impl::basic_stack_allocator> v
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
        CONTAINER_T<test_emplace, 5, Alloc_pattern> l;
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
        CONTAINER_T<test_emplace, 5, Alloc_pattern> l;
        l.emplace_back(0, 'c', &val);
        CHECK(l.size() == 1);
        test_emplace exp{0, 'c', &val};
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

#endif //! FIXED_TEST_FUNCTION_UNARY_HPP