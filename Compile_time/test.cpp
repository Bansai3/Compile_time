#include "pch.h"
#include<vector>
#include<iostream>


template<int size>
struct S
{
	static const int s = 1 + S<size - 1>::s;
};

template<>
struct S<0>
{
	static const int s = 0;
};


class Polynomial
{
private:
	std::vector<std::pair<int, int>> polynomial;
public:
	int size = 0;
	template<typename ... rest>
	Polynomial(std::vector<int> vec);
	friend std::ostream& operator<<(std::ostream& out, Polynomial& pol);
	template<int degree>
	friend constexpr int sum(int N, const Polynomial& pol);
	std::vector<std::pair<int, int>> get_polynomial();
};

std::vector<std::pair<int, int>> Polynomial::get_polynomial()
{
	return polynomial;
}

template<typename ... rest>
Polynomial::Polynomial(std::vector<int> vec)
{
	int count = 0;
	for (auto i : vec)
	{
		if (i)
			polynomial.push_back(std::make_pair(i, count));
		count++;
	}

	size = vec.size();
}


std::ostream& operator<<(std::ostream& out, Polynomial& pol)
{
	for (auto i : pol.polynomial)
	{
		if (i == *(pol.polynomial.end() - 1))
			std::cout << i.first << "x^" << i.second;
		else if (i == *(pol.polynomial.begin()))
			std::cout << i.first << " + ";
		else if (i.first)
			std::cout << i.first << "x^" << i.second << " + ";
	}
	return out;
}

template<const int degree>
constexpr int sum(int N, const Polynomial& pol)
{
	return pol.polynomial[degree].first * std::pow(N, pol.polynomial[degree].second) + sum<degree - 1>(N, pol);
}

template<>
constexpr int sum<-1>(int N, const Polynomial& pol)
{
	return 0;
}

TEST(PositiveValue, PolynomialValue) 
{
	const int size = 4;
	Polynomial pol({ 1, 1, 2, 2, 0, 3 });
	std::vector<std::pair<int, int>> vec = pol.get_polynomial();
	int positiv_value = 1;
	int summ = 0;
	int count = 0;

	for (auto i : vec)
	{
		summ += i.first * std::pow(positiv_value, count);
		count++;
	}


	ASSERT_EQ(sum<size>(positiv_value, pol) + 1, summ);
	ASSERT_FALSE(summ != sum<size>(positiv_value, pol));
}

int main()
{
	Polynomial pol({ 1, 1, 2, 2, 0, 3 });
	S<4> s;
	std::cout << sum<s.s>(1, pol) << std::endl;
	std::cout << pol;

	return 0;
}