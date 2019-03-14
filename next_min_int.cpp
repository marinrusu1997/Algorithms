#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

template <auto base = 10>
class digit_repr
{
 public:
   struct no_free_digits_exception : std::exception
   {
      no_free_digits_exception(const char *msg) : _msg{msg}
      {
      }

      const char *what() const noexcept { return _msg; }

    private:
      const char *_msg;
   };

   digit_repr() : _digits{}, _count{0}
   {
   }

   auto nextMinimalInt(uint64_t number) noexcept(false)
   {
      getDigitsFromInt(number);
      throwIfNoFreeDigits();
      std::vector<uint8_t> digitsForNextInt;
      for (auto i = 0; i < _count; i++)
      {
         if (i == _count - 1)
         {
            if (_most_significant_digit == base - 1) // handling cases where 9 is most significant, we need two digits instead of one
            {
               digitsForNextInt.push_back(getMinimumDigitGreatherThan(0)); // 0 is almost always the starting digit for all bases, i.e for 2, 8, 10
            }
            else // we need next digit greather than most significant digit
            {
               digitsForNextInt.push_back(getMinimumDigitGreatherThan(_most_significant_digit));
               break; // we are done
            }
         }
         digitsForNextInt.push_back(getMinimumDigitGreatherThan(0));
      }
      return getIntFromDigits(digitsForNextInt);
   }

 private:
   auto inline isPresent(uint8_t digit) noexcept
   {
      return _digits.find(digit) != _digits.end();
   }

   auto getMinimumDigitGreatherThan(uint8_t digit) noexcept
   {
      while (isPresent(digit) && digit < base)
         digit++;
      return digit;
   }

   auto getIntFromDigits(std::vector<uint8_t> const &digits) noexcept
   {
      auto len = digits.size();
      int number = 0;
      for (int i = len - 1; i >= 0; i--)
         number += digits[i] * (int)pow(base, i);
      return number;
   }

   auto getDigitsFromInt(uint64_t number)
   {
      resetState();
      while (number)
      {
         _most_significant_digit = number % base;
         _digits.insert(_most_significant_digit);
         number /= base;
         _count++;
      }
   }

   auto inline resetState() noexcept
   {
      _digits.clear();
      _count = 0;
   }

   auto inline throwIfNoFreeDigits() noexcept(false)
   {
      if (_digits.size() == base ||
          _digits.size() == base - 1 && !isPresent(0))
         throw no_free_digits_exception("invalid number, there are not free digits");
   }

   std::set<uint8_t> _digits;
   size_t _count;
   uint8_t _most_significant_digit;
};

int main(int argc, char *argv[])
{
   digit_repr repr;
   std::cout << "2 -> " << repr.nextMinimalInt(2) << std::endl;             // 3
   std::cout << "123 -> " << repr.nextMinimalInt(123) << std::endl;         // 400
   std::cout << "793 -> " << repr.nextMinimalInt(793) << std::endl;         // 800
   std::cout << "3025 -> " << repr.nextMinimalInt(3025) << std::endl;       // 4111
   std::cout << "901 -> " << repr.nextMinimalInt(901) << std::endl;         // 2222
   std::cout << "9990 -> " << repr.nextMinimalInt(9990) << std::endl;       // 11111
   std::cout << "654321 -> " << repr.nextMinimalInt(654321) << std::endl;   // 700000
   std::cout << "6543210 -> " << repr.nextMinimalInt(6543210) << std::endl;   // 7777777
   std::cout << "76543210 -> " << repr.nextMinimalInt(76543210) << std::endl;   // 88888888
   std::cout << "76543210 -> " << repr.nextMinimalInt(7654320) << std::endl;   // 8111111
   std::cout << "8962356 -> " << repr.nextMinimalInt(8962356) << std::endl; // 10000000
   try
   {
      std::cout << "123456789 -> " << repr.nextMinimalInt(123456789) << std::endl; // exception
   }
   catch (std::exception const &e)
   {
      std::cout << e.what() << std::endl;
   }
   return 0;
}
