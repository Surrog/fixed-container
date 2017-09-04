
#include "catch.hpp"
#include "fixed/list.hpp"

void test_list()
{
	fixed::list<int, 20> l;
}

TEST_CASE("testing lists", "[linear]")
{
	test_list();
}
