#include <set>
#include <vector>
#include <iostream>
#include <limits.h>
#include <numeric>

using namespace std;

template<typename IntType>
IntType findMissing(vector<IntType> const& bigger, vector<IntType> const& lower)
{
	// a ^ a = 0
	// a ^ 0 = a
	IntType missing {0};
	for(const auto& number : bigger)
		missing ^= number;

	for(const auto& number : lower)
		missing ^= number;
	
	return missing;
}

template<typename IntType>
IntType findMissingLookup(vector<IntType> const& bigger, vector<IntType> const& lower)
{
	// Binary tree fast lookup
	set<IntType> lowerSet {lower.begin(), lower.end()};
	for(const auto& number : bigger)
		if (lowerSet.find(number) == lowerSet.end())
			return number;
	return -1;
}

template<typename IntType>
IntType findMissingWithSumm(vector<IntType> const& bigger, vector<IntType> const& lower)
{
	// Warning! Overflow problem
	return accumulate(bigger.begin(), bigger.end(), IntType{0}) - 
				accumulate(lower.begin(), lower.end(), IntType{0});
}

int main()
{
	vector<int> bigger{4,8,INT_MAX, 9, 3, -2};
	vector<int> lower{4,8,9,3, INT_MAX};
	cout << "XOR: " << findMissing(bigger,lower) << endl;
	cout << "Binary Tree: " << findMissingLookup(bigger, lower) << endl;
	cout << "Summ: " << findMissingWithSumm(bigger, lower) << endl;
}