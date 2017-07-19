
#include "fixed/unordered_map.hpp"
#include "catch.hpp"
#include <algorithm>

typedef fixed::unordered_map<std::string, std::string, 10> testing_hm;

static void test_constructor()
{
	testing_hm hash_a;
	testing_hm hash_b(hash_a);
	testing_hm hash_c(std::move(hash_b));

	hash_a = hash_b;
	hash_b = std::move(hash_a);
}

static void test_access()
{
	testing_hm hash;

	auto pair = hash.insert({ "test", "test" });

	CHECK(pair.second == true);

	auto it = pair.first;

	CHECK(it->first == "test");
	CHECK(it->second == "test");
}

static void test_capacity()
{
	testing_hm hash;

	CHECK(hash.size() == 0);
	CHECK(hash.empty());
	CHECK(hash.max_size() == 10);


	hash["test"] = "toto";

	CHECK(hash.size() == 1);
	CHECK(!hash.empty());
	CHECK(hash.max_size() == 10);
}

static void test_iterators()
{
	testing_hm hash;
	hash["test"] = "toto";
	hash["titi"] = "tutu";
	hash["tata"] = "tyty";

	for (auto& test : hash)
	{
		CHECK(test.first.size() > 0);
		CHECK(test.second.size() > 0);
	}

	std::for_each(hash.begin(), hash.end(), [](auto& value) {
		CHECK(value.first.size() > 0);
	});
	std::for_each(hash.cbegin(), hash.cend(), [](const auto& value) {
		CHECK(value.first.size() > 0);
	});
}

static void test_modifiers()
{
	testing_hm hash;
	hash["test"] = "toto";
	hash["titi"] = "tutu";
	hash["tata"] = "tyty";

	std::array<testing_hm::value_type, 3> valid = {
		testing_hm::value_type{ "test", "toto" },
		testing_hm::value_type{ "titi", "tutu" },
		testing_hm::value_type{ "tata", "tyty" }
	};

	CHECK(hash.size() == 3);
	for (auto& pair : valid)
	{
		auto it = hash.find(pair.first);
		CHECK(it != hash.end());
		CHECK(it->second == pair.second);
	}

	hash.erase("test");

	REQUIRE(hash.size() == 2);

	std::array<testing_hm::value_type, 3> valid1 = {
		testing_hm::value_type{ "titi", "tutu" },
		testing_hm::value_type{ "tata", "tyty" }
	};


	CHECK(hash.find("test") == hash.end());
	for (auto& pair : valid1)
	{
		CHECK(hash.find(pair.first)->second == pair.second);
	}

	hash["test"] = "toto";

	CHECK(hash.size() == 3);

	auto pair_insert = hash.insert({ "test", "titi" });
	CHECK(pair_insert.second == false);
	CHECK(pair_insert.first->first == "test");
	CHECK(pair_insert.first->second == "toto");
	REQUIRE(hash.size() == 3);


	pair_insert = hash.insert_or_assign("test", "titi");
	CHECK(pair_insert.second == false);
	CHECK(pair_insert.first->first == "test");
	CHECK(pair_insert.first->second == "titi");
	REQUIRE(hash.size() == 3);

	pair_insert = hash.insert_or_assign("neo", "titi");
	CHECK(pair_insert.second == true);
	CHECK(pair_insert.first->first == "neo");
	CHECK(pair_insert.first->second == "titi");
	REQUIRE(hash.size() == 4);

	hash.erase(hash.find("tata"));
	CHECK(hash.size() == 3);

	CHECK(hash.find("tata") == hash.end());
	hash.clear();
	CHECK(hash.size() == 0);
	CHECK(hash.bucket_count() == 0);

	hash["test"] = "yoyo";
	hash["habsburg"] = "austria";
	hash["de valois"] = "france";


	testing_hm hash2;
	hash.swap(hash2);

	std::array<testing_hm::value_type, 3> valid3 = {
		testing_hm::value_type{ "test", "yoyo" },
		testing_hm::value_type{ "habsburg", "austria" },
		testing_hm::value_type{ "de valois", "france" }
	};

	CHECK(hash2.size() == 3);
	CHECK(hash.size() == 0);
	for (auto& pair : valid3)
	{
		auto it = hash2.find(pair.first);
		CHECK(it != hash2.end());
		CHECK(it->first == pair.first);
		CHECK(it->second == pair.second);
	}
}

static void test_limit()
{
	fixed::unordered_map<int, char, 5> hash;
	hash.insert({ 1, 'a' });
	hash.insert({ 2, 'a' });
	hash.insert({ 3, 'a' });
	hash.insert({ 4, 'a' });
	hash.insert({ 5, 'a' });
	CHECK_THROWS(hash.insert({ 6, 'a' }));
}

TEST_CASE("testing hashmap", "[statics]")
{
	//test_constructor();
	//test_capacity();
	//test_access();
	//test_modifiers();
	//test_limit();
}