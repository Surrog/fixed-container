
#include <vector>
#include <string>
#include <iostream>
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

void test_vector_constructor()
{
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

	{
		fixed::vector<std::string, 10> words1{ "the", "frogurt", "is", "also", "cursed" };
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words1.begin(), words1.end(), expected.begin(), expected.end()));
		}

		// words2 == words1
		fixed::vector<std::string, 10> words2(words1.begin(), words1.end());
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words2.begin(), words2.end(), expected.begin(), expected.end()));
		}
	}

	{
		// c++11 initializer list syntax:
		fixed::vector<std::string, 10> words1{ "the", "frogurt", "is", "also", "cursed" };
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words1.begin(), words1.end(), expected.begin(), expected.end()));
		}

		// words2 == words1
		fixed::vector<std::string, 10> words2(words1.begin(), words1.end());
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words2.begin(), words2.end(), expected.begin(), expected.end()));
		}

		// words3 == words1
		fixed::vector<std::string, 10> words3(words1);
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words3.begin(), words3.end(), expected.begin(), expected.end()));
		}
	}

	{
		// c++11 initializer list syntax:
		fixed::vector<std::string, 5> words1{ "the", "frogurt", "is", "also", "cursed" };
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words1.begin(), words1.end(), expected.begin(), expected.end()));
		}

		// words2 == words1
		fixed::vector<std::string, 10> words2(words1.begin(), words1.end());
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words2.begin(), words2.end(), expected.begin(), expected.end()));
		}

		// words3 == words1
		fixed::vector<std::string, 10> words3(words1);
		{
			auto expected = { "the", "frogurt", "is", "also", "cursed" };
			CHECK(std::equal(words3.begin(), words3.end(), expected.begin(), expected.end()));
		}

		// words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
		fixed::vector<std::string, 10> words4(5, "Mo");
		{
			auto expected = { "Mo", "Mo", "Mo", "Mo", "Mo" };
			CHECK(std::equal(words4.begin(), words4.end(), expected.begin(), expected.end()));
		}
	}

	{
		fixed::vector<int, 10> nums1{ 3, 1, 4, 6, 5, 9 };
		fixed::vector<int, 6> nums2;
		fixed::vector<int, 20> nums3;

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

void basic_testing()
{
	{
		fixed::vector<char, 5> characters;

		characters.assign(5, 'a');

		CHECK(characters.size() == 5);
		int iteration = 0;
		for (auto& c : characters)
		{
			CHECK(c == 'a');
			iteration++;
		}
		CHECK(iteration == 5);
	}

	{
		fixed::vector<int, 10> myvector;
		for (int i = 1; i <= 5; i++) myvector.push_back(i);

		int i = 1;
		for (fixed::vector<int, 10>::iterator it = myvector.begin(); it != myvector.end(); ++it)
		{
			CHECK(*it == i);
			i++;
		}
	}

	{
		fixed::vector<int, 10> myvector(5);

		fixed::vector<int, 10>::reverse_iterator rbit = myvector.rbegin();
		fixed::vector<int, 10>::reverse_iterator reit = myvector.rend();
		fixed::vector<int, 10>::iterator it = myvector.begin() + (myvector.size() - 1);
		int i = 0;
		while (rbit != reit) {
			CHECK(&*it == &*rbit);
			--it;
			++rbit;
			++i;
		}
		CHECK(i == 5);

	}


	{
		fixed::vector<int, 10> myvector(5);  // 5 default-constructed ints

		int i = 0;

		fixed::vector<int, 10>::reverse_iterator rit = myvector.rbegin();
		for (; rit != myvector.rend(); ++rit)
			*rit = ++i;
		CHECK(i == 5);
		for (fixed::vector<int, 10>::iterator it = myvector.begin(); it != myvector.end(); ++it)
		{
			CHECK(*it == i);
			i--;
		}
	}

	{
		fixed::vector<int, 10> myvector = { 10,20,30,40,50 };

		auto expected = { 10,20,30,40,50 };
		int i = 0;
		for (auto it = myvector.cbegin(); it != myvector.cend(); ++it)
		{
			CHECK(*it == *(expected.begin() + i));
			i++;
		}
	}

	{
		fixed::vector<int, 10> myvector = { 10,20,30,40,50 };

		auto expected = { 10,20,30,40,50 };
		for (std::size_t i = 0; i < expected.size(); i++)
		{
			CHECK(myvector[i] == *(expected.begin() + i));
		}
	}

	{
		fixed::vector<int, 10> myvector = { 10,20,30,40,50 };

		auto expected = { 10,20,30,40,50 };
		for (std::size_t i = 0; i < expected.size(); i++)
		{
			CHECK(myvector.at(i) == *(expected.begin() + i));
		}

		CHECK(myvector.front() == 10);
		CHECK(myvector.back() == 50);
	}

	{
		fixed::vector<int, 10> myvector;

		CHECK(myvector.empty());
		myvector = { 10,20,30,40,50 };
		CHECK(!myvector.empty());
		CHECK(myvector.size() == 5);
		myvector.clear();
		CHECK(myvector.empty());
		CHECK(myvector.max_size() == 10);
		myvector.reserve(10);
		CHECK(myvector.capacity() == 10);
		myvector.shrink_to_fit();
	}
}

void test_vector_modifier()
{
	{
		fixed::vector<int, 20> vec(3, 100);
		std::vector<int> expected = { 100, 100, 100 };
		CHECK(vec == expected);


		auto it = vec.begin();
		it = vec.insert(it, 200);
		expected = { 200, 100, 100, 100 };
		CHECK(vec == expected);

		vec.insert(it, 2u, 300);
		expected = { 300, 300, 200, 100, 100, 100 };
		CHECK(vec == expected);

		it = vec.begin();
		std::vector<int> vec2(2, 400);
		vec.insert(it + 2, vec2.begin(), vec2.end());
		expected = { 300, 300, 400, 400, 200, 100, 100, 100 };
		CHECK(vec == expected);

		int arr[] = { 501,502,503 };
		CHECK_NOTHROW(vec.insert(vec.begin(), arr, arr + 3));
		expected = { 501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100 };
		CHECK(vec == expected);
	}


	{
		fixed::vector<int, 10> c{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::vector<int> expected{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		

		c.erase(c.begin());
		expected.erase(expected.begin());

		CHECK(c == expected);

		expected.erase(expected.begin() + 2, expected.begin() + 5);
		c.erase(c.begin() + 2, c.begin() + 5);

		CHECK(c == expected);
	}
}

TEST_CASE("testing vector", "[linear]")
{
	test_vector_constructor();
	test_vector_modifier();
	basic_testing();

}