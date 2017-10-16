
#include "catch.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "fixed/listed_vector.hpp"
#include <cstring>
#include <list>
#include <string>
#include <iostream>
#include <functional>

#include "test_functions_unary_containers.hpp"
#include "test_struct.hpp"

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_contructors()
{
    test_constructor<LIST_T, Alloc_pattern>();
    test_destructor<LIST_T, Alloc_pattern>();
    test_operator_assignment<LIST_T, Alloc_pattern>();
    test_assign<LIST_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_element_access()
{
    test_front<LIST_T, Alloc_pattern>();
    test_back<LIST_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_iterator()
{
    test_begin_end<LIST_T, Alloc_pattern>();
    test_rbegin_rend<LIST_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_capacity()
{
    test_generic_capacity<LIST_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_modifiers()
{
    test_clear<LIST_T, Alloc_pattern>();
    test_insert<LIST_T, Alloc_pattern>();
    test_emplace<LIST_T, Alloc_pattern>();
    test_erase<LIST_T, Alloc_pattern>();
    test_push_back<LIST_T, Alloc_pattern>();
    test_emplace_back<LIST_T, Alloc_pattern>();
    test_pop_back<LIST_T, Alloc_pattern>();
    test_push_front<LIST_T, Alloc_pattern>();
    test_emplace_front<LIST_T, Alloc_pattern>();
    test_pop_front<LIST_T, Alloc_pattern>();
    test_resize<LIST_T, Alloc_pattern>();
    test_swap<LIST_T, Alloc_pattern>();
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
#ifdef FIXED_CONTAINER_CHECK_BOUND
        CHECK_THROWS(lval.merge(std::move(rval)));
#endif
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

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_splice()
{
    {
        LIST_T<int, 10, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 10, Alloc_pattern> rc = {0, 1};

        lc.splice(lc.cbegin(), std::move(rc));
        auto exp = {0, 1, 2, 3, 4, 5};
        CHECK(lc.size() == exp.size());
        CHECK(std::equal(lc.begin(), lc.end(), exp.begin(), exp.end()));
		CHECK(rc.size() == 0);
    }

    {
        LIST_T<int, 10, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 10, Alloc_pattern> rc = {0, 1};

        lc.splice(lc.begin(), std::move(rc), rc.begin() + 1);
        auto exp = {1, 2, 3, 4, 5};
        CHECK(lc.size() == exp.size());
        CHECK(std::equal(lc.begin(), lc.end(), exp.begin(), exp.end()));
		auto rc_exp = { 0 };
		CHECK(rc.size() == rc_exp.size());
		CHECK(std::equal(rc.begin(), rc.end(), rc_exp.begin(), rc_exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 10, Alloc_pattern> rc = {0, 1};

        lc.splice(lc.begin(), std::move(rc), rc.begin(), rc.begin() + 2);
        auto exp = {0, 1, 2, 3, 4, 5};
        CHECK(lc.size() == exp.size());
        CHECK(std::equal(lc.begin(), lc.end(), exp.begin(), exp.end()));
		CHECK(rc.size() == 0);
    }

    {
        LIST_T<int, 4, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 4, Alloc_pattern> rc = {0, 1};
#ifdef FIXED_CONTAINER_CHECK_BOUND
        CHECK_THROWS(lc.splice(lc.begin(), std::move(rc)));
#endif
    }

    {
        LIST_T<int, 4, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 4, Alloc_pattern> rc = {0, 1};
#ifdef FIXED_CONTAINER_CHECK_BOUND
        CHECK_THROWS(lc.splice(lc.begin(), std::move(rc), rc.begin() + 1));
#endif
    }

    {
        LIST_T<int, 4, Alloc_pattern> lc = {2, 3, 4, 5};
        LIST_T<int, 4, Alloc_pattern> rc = {0, 1};

#ifdef FIXED_CONTAINER_CHECK_BOUND
        CHECK_THROWS(lc.splice(lc.begin(), std::move(rc), rc.begin(), rc.begin() + 2));
#endif
    }

    {
        LIST_T<test_move, 10, Alloc_pattern> lc;

		lc.push_back(test_move("test1"));
		lc.push_back(test_move("test2"));

		LIST_T<test_move, 10, Alloc_pattern> rc;
		
		rc.push_back(test_move("toto"));
		rc.push_back(test_move("titi"));
		rc.push_back(test_move("tata"));

        lc.splice(lc.begin(), std::move(rc));
		CHECK(rc.size() == 0);
		CHECK(lc.size() == 5);
    }

    {
		LIST_T<test_move, 10, Alloc_pattern> lc;
		lc.push_back(test_move("test1"));
		lc.push_back(test_move("test2"));

		LIST_T<test_move, 10, Alloc_pattern> rc;
		rc.push_back(test_move("toto"));
		rc.push_back(test_move("titi"));
		rc.push_back(test_move("tata"));

        lc.splice(lc.begin(), std::move(rc), rc.begin() + 1);
    }

    {
		LIST_T<test_move, 10, Alloc_pattern> lc;
		lc.push_back(test_move("test1"));
		lc.push_back(test_move("test2"));

		LIST_T<test_move, 10, Alloc_pattern> rc;
		rc.push_back(test_move("toto"));
		rc.push_back(test_move("titi"));
		rc.push_back(test_move("tata"));

        lc.splice(lc.begin(), std::move(rc), rc.begin(), rc.begin() + 2);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_remove()
{
    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove(10);
        auto exp = {1, 2, 3, 4, 5};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove(5);
        auto exp = {1, 2, 3, 4};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4};
        c.remove(4);
        auto exp = {1, 2, 3};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }
    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3};
        c.remove(3);
        auto exp = {1, 2};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }
    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2};
        c.remove(2);
        auto exp = {1};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }
    {
        LIST_T<int, 10, Alloc_pattern> c = {1};
        CHECK_NOTHROW(c.remove(1));
        CHECK(c.size() == 0);
    }

    {
        LIST_T<int, 10, Alloc_pattern> c;
        CHECK_NOTHROW(c.remove(1));
        CHECK(c.size() == 0);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_remove_if()
{
    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove_if([](int n) { return n < 3; });
        auto exp = {3, 4, 5};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove_if([](int n) { return n == 3; });
        auto exp = {1, 2, 4, 5};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove_if([](int n) { return n > 2; });
        auto exp = {1, 2};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c = {1, 2, 3, 4, 5};
        c.remove_if([](int n) { return n == 10; });
        auto exp = {1, 2, 3, 4, 5};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c;
        CHECK_NOTHROW(c.remove_if([](int n) { return n == 10; }));
        CHECK(c.size() == 0);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_reverse()
{
    {
        LIST_T<int, 10, Alloc_pattern> c = {0, 1, 2, 3, 4, 5};
        CHECK_NOTHROW(c.reverse());
        auto exp = {5, 4, 3, 2, 1, 0};
        CHECK(c.size() == exp.size());
        CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

    {
        LIST_T<int, 10, Alloc_pattern> c;
        CHECK_NOTHROW(c.reverse());
        CHECK(c.size() == 0);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_unique()
{
	{
		LIST_T<int, 10, Alloc_pattern> c = { 1, 2, 3, 3, 3, 2, 1, 1, 2 };
		c.unique();
		auto exp = { 1, 2, 3, 2, 1, 2 };
		CHECK(c.size() == exp.size());
		CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
    }

	{
		int i = 0;
		LIST_T<test_construct, 10, Alloc_pattern> c;
		for (typename LIST_T<std::size_t, 5, Alloc_pattern>::size_type v = 0; v < c.max_size(); v++)
		{
			c.push_back(test_construct(i));
		}
		CHECK(i == 10);
		CHECK_NOTHROW(c.unique([](const test_construct&, const test_construct&)
		{
			return true;
		}));
		CHECK(c.size() == 1);
		CHECK(i == 1);
	}


    {
        LIST_T<int, 10, Alloc_pattern> c;
        CHECK_NOTHROW(c.unique());
        CHECK(c.size() == 0);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_sort()
{
    {
        LIST_T<int, 10, Alloc_pattern> c = {8, 7, 5, 9, 0, 1, 3, 2, 6, 4};
        CHECK_NOTHROW(c.sort());
		{
			auto exp = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			CHECK(c.size() == exp.size());
			CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
		}
        CHECK_NOTHROW(c.sort(std::greater<int>()));
		{
			auto exp = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
			CHECK(c.size() == exp.size());
			CHECK(std::equal(c.begin(), c.end(), exp.begin(), exp.end()));
		}
    }

    {
        LIST_T<int, 10, Alloc_pattern> c;
        CHECK_NOTHROW(c.sort());
        CHECK(c.size() == 0);
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list_operation()
{
    test_merge<LIST_T, Alloc_pattern>();
    test_splice<LIST_T, Alloc_pattern>();
    test_remove<LIST_T, Alloc_pattern>();
    test_remove_if<LIST_T, Alloc_pattern>();
    test_reverse<LIST_T, Alloc_pattern>();
    test_unique<LIST_T, Alloc_pattern>();
    test_sort<LIST_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list()
{
	test_noexcept_constructor<LIST_T, Alloc_pattern>();
    test_list_contructors<LIST_T, Alloc_pattern>();
    test_list_element_access<LIST_T, Alloc_pattern>();
    test_list_iterator<LIST_T, Alloc_pattern>();
    test_list_capacity<LIST_T, Alloc_pattern>();
    test_list_modifiers<LIST_T, Alloc_pattern>();
    test_list_operation<LIST_T, Alloc_pattern>();
    test_list_constant_reference<LIST_T, Alloc_pattern>();
}

TEST_CASE("testing lists", "[linear]")
{
    test_list<fixed::listed_vector, fixed::_impl::aligned_stack_allocator>();
    test_list<fixed::listed_vector, fixed::_impl::aligned_heap_allocator>();
}
