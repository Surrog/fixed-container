
#include <vector>
#include <string>
#include "fixed/vector.hpp"
#include "catch.hpp"

struct test_struct
{
	int i = 10;
	char c = 'c';
	unsigned int* ui = nullptr;
};

bool operator==(const test_struct& lval, const test_struct& rval)
{
	return lval.i == rval.i && lval.c == rval.c && lval.ui == rval.ui;
}

typedef fixed::vector<test_struct, 10> local_static_vector;
typedef std::vector<test_struct> reference_vector;

void basic_testing()
{
	local_static_vector test;
	reference_vector vec_ref;

	unsigned int array_test[] = { 1u, 2u };
	test.push_back(test_struct());
	vec_ref.push_back(test_struct());
	test.push_back({ 30, 'b', array_test });
	vec_ref.push_back({ 30, 'b', array_test });

	local_static_vector test2 = test;

	CHECK(std::equal(test.begin(), test.end(), vec_ref.begin(), vec_ref.end()));
	CHECK(std::equal(test.begin(), test.end(), test2.begin(), test2.end()));
}

TEST_CASE("testing vector", "[linear]")
{
	basic_testing();
}