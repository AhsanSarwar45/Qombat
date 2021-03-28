#include <type_traits>

namespace QMBT
{
   namespace Utility
   {
      template <typename EnumType>
      constexpr inline decltype(auto) EnumToIntegral(EnumType enumValue)
      {
         static_assert(std::is_enum<EnumType>::value, "Enum type required");
         using EnumValueType = std::underlying_type_t<EnumType>;
         return static_cast<EnumValueType>(enumValue);
      }

      template <typename EnumType, typename IntegralType>
      constexpr inline EnumType IntegralToEnum(IntegralType value)
      {
         static_assert(std::is_enum<EnumType>::value, "Enum type required");
         static_assert(std::is_integral<IntegralType>::value, "Integer required");
         return static_cast<EnumType>(value);
      }

      template <typename EnumType, typename UnaryFunction>
      constexpr inline void EnumSetIntegral(EnumType &enumValue, UnaryFunction integralWritingFunction)
      {
         // Since using reinterpret_cast on reference to underlying enum type is UB must declare underlying type value and write to it and then cast it to enum type
         // See discussion on https://stackoverflow.com/questions/19476818/is-it-safe-to-reinterpret-cast-an-enum-class-variable-to-a-reference-of-the-unde

         static_assert(std::is_enum<EnumType>::value, "Enum type required");

         auto enumIntegralValue = EnumToIntegral(enumValue);
         integralWritingFunction(enumIntegralValue);
         enumValue = IntegralToEnum<EnumType>(enumIntegralValue);
      }
   }
}
