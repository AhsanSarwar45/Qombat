#include <QMBTPCH.hpp>

namespace QMBT
{
	namespace Utility
	{
		template <class U, class T>
		struct IsExplicitlyConvertible
		{
			static constexpr bool value = std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value;
		};
	} // namespace Utility

} // namespace QMBT