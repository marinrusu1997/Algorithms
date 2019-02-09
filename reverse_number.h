#ifndef _REVERSE_NUMBER_HEADER_
#define _REVERSE_NUMBER_HEADER_

#define MOD(dividend,divisor) (dividend) % (divisor)
#define DIV(dividend,divisor) (dividend) / (divisor)
#define CLEAR_SIGN(number) (number + (number >> sizeof(number) * CHAR_BIT - 1)) ^ \
							(number >> sizeof(number) * CHAR_BIT - 1)
#define MAKE_POSITIVE(number) ~(number - 1)

/*
	CLEAR_SIGN and MAKE_POSITIVE => equivalent
	2's Complement:
		1) Number to binary
		2) Reverse bits
		3) Add 1
*/

int reverse(int number)
{
	int reversed = 0, prev = 0;
	char digit = 0;
	while (number)
	{
		digit = MOD(number, 10);
		reversed = reversed * 10 + digit;
		if ((reversed - digit) / 10 != prev)
			return -1;
		prev = reversed;
		number = DIV(number, 10);
	}
	return reversed;
}

#endif // !_REVERSE_NUMBER_HEADER_

