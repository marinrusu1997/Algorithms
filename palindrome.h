#ifndef _PALINDROME_HEADER_
#define _PALINDROME_HEADER_

#include <string.h>

int doCheck(const char *str, size_t l, size_t h)
{
	if (l >= h)
		return 0;
	if (str[l++] != str[h--])
		return -1;
	return doCheck(str, l, h);
}

int isPalindrom(const char *str)
{
	return doCheck(str, 0, strlen(str) - 1);
}

int isPalindromNumericRec(int num, int *copy)
{
	if (0 <= num && num < 10)
		return num == *copy % 10 ? 0 : -1;
	if (isPalindromNumericRec(num / 10, copy) == -1)
		return -1;
	*copy /= 10;
	return num % 10 == *copy % 10 ? 0 : -1;
}

int isPalindromNumeric(int num)
{
	int copy = num;
	return isPalindromNumericRec(num, &copy);
}

#endif // !_PALINDROME_HEADER_
