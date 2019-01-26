#include <string>
#include <string.h>
#include <iostream>
#include <stack>

using namespace std;

template<typename T>
void swap(T *first,T *second)
{
	// accumulate first & second -> accumulator
	*first	^= *second;
	// substract second from accumulator -> first
	*second	^= *first;
	// substract second(i.e initial first) from accumulator -> second
	*first	^= *second;
}

template<typename CharT>
void reverse(CharT *c_str)
{
	for(size_t begin = 0, end = strlen(c_str) - 1; begin < end; begin++, end -= begin)
		::swap(&c_str[begin], &c_str[end]);
}

template<typename CharT>
void reverse_recursive(CharT *c_str, size_t end, size_t begin = 0)
{
	if (begin >= end)
		return;
	::swap(&c_str[begin], &c_str[end]);
	reverse_recursive(c_str, --end, ++begin);
}

template<typename CharT>
void reverse_recursive(CharT *c_str)
{
	stack<CharT> chars;
	auto end = strlen(c_str) - 1;
	for(size_t i = 0; i <= end; i++)
		chars.push(c_str[i]);
	for(size_t i = 0 ; i <= end; i++)
	{
		c_str[i] = chars.top();
		chars.pop();
	}
}

int main()
{
	string s;
	cout << "Your string: ";
	cin >> s;
	reverse((char*)s.data());
	cout <<"Reversed: " << s << endl;
	cout << "Your string: ";
	cin >> s;
	reverse_recursive((char*)s.data(), s.length() - 1);
	cout <<"Reversed: " << s << endl;
	cout << "Your string: ";
	cin >> s;
	reverse_recursive((char*)s.data());
	cout <<"Reversed: " << s << endl;
	return 0;
}