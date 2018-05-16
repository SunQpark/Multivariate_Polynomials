#include<iostream>
#include<vector>
#include"Polynomials.hpp"
#include"Division.hpp"
#include"tester.hpp"



int main() {
	mpq_class x = 1;
	mpq_class y = 2;

	std::cout << x/y << std::endl;
	// test_division_uni();
	test_division_mult();
	return 0;
}