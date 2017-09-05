#ifndef FIXED_VOID_T_COMPATIBILITY_HPP
#define FIXED_VOID_T_COMPATIBILITY_HPP

namespace fixed
{
	namespace astd
	{
		template< class... >
		using void_t = void;

		template <class T> constexpr bool is_const_v = std::is_const<T>::value;
	}
}

#endif //!FIXED_VOID_T_COMPATIBILITY_HPP