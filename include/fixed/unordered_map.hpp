#ifndef HEAPLESS_UNORDERED_MAP_HPP
#define HEAPLESS_UNORDERED_MAP_HPP

#include "impl/basic_unordered_map.hpp"

namespace fixed
{
	template <typename Key, typename T, _impl::container_size_type SIZE, class HASH = std::hash<Key>, class PRED = std::equal_to<Key>>
	using unordered_map = _impl::basic_unordered_map<Key, T, SIZE, HASH, PRED>;

	template <typename Key, typename T
		, typename HASHER1, typename HASHER2
		, typename EQUAL1, typename EQUAL2
		, _impl::container_size_type SIZE1, _impl::container_size_type SIZE2>
		bool operator==(const unordered_map<Key, T, SIZE1, HASHER1, EQUAL1>& lval
			, const unordered_map<Key, T, SIZE2, HASHER2, EQUAL2>& rval)
	{
		auto result = lval.size() == rval.size() &&
			std::all_of(lval.begin(), lval.end(), [&rval](const auto& lval_pair)
		{
			return std::find(rval.begin(), rval.end(), lval_pair) != rval.end();
		});
		return result;
	}

	template <typename Key, typename T
		, typename HASHER1, typename HASHER2
		, typename EQUAL1, typename EQUAL2
		, _impl::container_size_type SIZE1, _impl::container_size_type SIZE2>
		bool operator!=(const unordered_map<Key, T, SIZE1, HASHER1, EQUAL1>& lval
			, const unordered_map<Key, T, SIZE2, HASHER2, EQUAL2>& rval)
	{
		return !operator==(lval, rval);
	}


}

#endif //!HEAPLESS_UNORDERED_MAP_HPP