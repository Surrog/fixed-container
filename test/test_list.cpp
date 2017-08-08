
#include "catch.hpp"
#include "fixed/list.hpp"

template <typename LIST, typename T>
void test_list()
{

}

TEST_CASE("testing lists", "[linear]")
{
	test_list<fixed::list<int, 20>, int>();
}
