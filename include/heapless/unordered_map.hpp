#ifndef HEAPLESS_UNORDERED_MAP_HPP
#define HEAPLESS_UNORDERED_MAP_HPP

#include "impl/basic_unordered_map.hpp"

namespace heapless
{
	template <typename KEY, typename VALUE, std::size_t SIZE, class HASH = std::hash<KEY>, class PRED = std::equal_to<KEY>>
	using unordered_map = _impl::basic_unordered_map<KEY, VALUE, SIZE, HASH, PRED>;

	template <typename KEY, typename VALUE
		, typename HASHER1, typename HASHER2
		, typename EQUAL1, typename EQUAL2
		, std::size_t SIZE1, std::size_t SIZE2>
		bool operator==(const unordered_map<KEY, VALUE, SIZE1, HASHER1, EQUAL1>& lval
			, const unordered_map<KEY, VALUE, SIZE2, HASHER2, EQUAL2>& rval)
	{
		auto result = lval.size() == rval.size() &&
			std::all(lval.begin(), lval.end(), [&rval](const auto& lval_pair)
		{
			return std::find(rval.begin(), rval.end(), lval_pair) != rval.end();
		});
	}

	template <typename KEY, typename VALUE
		, typename HASHER1, typename HASHER2
		, typename EQUAL1, typename EQUAL2
		, std::size_t SIZE1, std::size_t SIZE2>
		bool operator!=(const unordered_map<KEY, VALUE, SIZE1, HASHER1, EQUAL1>& lval
			, const unordered_map<KEY, VALUE, SIZE2, HASHER2, EQUAL2>& rval)
	{
		return !operator==(lval, rval);
	}


}

#endif //!HEAPLESS_UNORDERED_MAP_HPP