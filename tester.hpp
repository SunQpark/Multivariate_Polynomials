#pragma once
#include"Polynomials.hpp"
#include"Division.hpp"
// #include"MultVarDivision.hpp"

using ring = mpq_class;
using Mono = Monomial<ring>;
using Poly = Polynomial<ring>;


void test_order() {

	std::vector<unsigned> exps1{ 1, 2 };
	std::vector<unsigned> exps2{ 1, 3 };

	Monomial<double> test1(3.0, exps1);
	Monomial<double> test2(4.0, exps1);
	Monomial<double> test3(3.0, exps2);

	std::cout << lexico_order(test1, test2) << "\t" << lexico_order(test1, test3);
}

void test_addition() {
	unsigned n_var = 2;
	std::vector<unsigned> exps1{ 1, 2 };
	std::vector<unsigned> exps2{ 1, 3 };
	std::vector<unsigned> exps3{ 2, 2 };

	Monomial<double> test1(3.0, exps1); // 3 x y^2
	Monomial<double> test2(4.0, exps1);
	Monomial<double> test3(3.0, exps2);
	Monomial<double> test4(1.0, exps3);
	Monomial<double> test5(-1.0, exps3);

	Polynomial<double> pol1(n_var);
	pol1.add(test1);
	pol1.add(test5);

	Polynomial<double> pol2(test3);
	pol2.add(test2);

	Polynomial<double> pol3(n_var);
	pol3.add(test4);
	pol3.add(test3);
	pol3.add(test1);

	print(pol1);
	std::cout << std::endl;
	print(pol2);
	std::cout << std::endl;
	print(pol3);

	pol3 += pol1;
	std::cout << std::endl;
	print(pol3);
	//std::cout << std::endl << pol3.is_zero();


}

void test_multiplication() {
	unsigned n_var = 1;
	Polynomial<double> poly1(n_var);
	for (unsigned i = 0; i < 100; i++) {
		auto mon = Monomial<double>(1.0, std::vector<unsigned>{i});
		poly1.add(mon);
	}
	auto poly2 = poly1;
	//poly2.back() = Monomial<double>(2.0, std::vector<unsigned>{4});

	print(poly1 * poly2);

	//print(poly2);

}

void test_mult_poly_sorting() {
	unsigned n_var = 2;
	std::vector<unsigned> exps1{ 1, 2 };
	std::vector<unsigned> exps2{ 1, 3 };
	std::vector<unsigned> exps3{ 2, 2 };

	Monomial<double> test1(3.0, exps1); // 3 x y^2
	Monomial<double> test2(4.0, exps1);
	Monomial<double> test3(3.0, exps2);
	Monomial<double> test4(1.0, exps3);
	Monomial<double> test5(-1.0, exps3);

	Polynomial<double> pol1(n_var);
	pol1.add(test1);
	pol1.add(test5);

	Polynomial<double> pol2(test3);
	pol2.add(test2);

	Polynomial<double> pol3(n_var);
	pol3.add(test4);
	pol3.add(test3);
	pol3.add(test1);


	std::vector<Polynomial<double>> polynomials{ pol1, pol2, pol3 };
	for (const auto &pol : polynomials) {
		print(pol);
	}
	
	std::cout << std::endl;

	poly_sort(polynomials);
	for (const auto &pol : polynomials) {
		print(pol);
	}




}

void test_division_uni() {
	std::vector<unsigned> constant{ 0 };
	std::vector<unsigned> x_to1{ 1 };
	std::vector<unsigned> x_to2{ 2 };
	std::vector<unsigned> x_to3{ 3 };
	std::vector<unsigned> x_to4{ 4 };

	Poly poly2;
	Poly poly1;
	
	Mono mon1_1(1.0, constant);
	Mono mon1_2(4.0, x_to1);
	Mono mon1_3(4.0, x_to2);
	Mono mon1_4(4.0, x_to3);
	Mono mon1_5(4.0, x_to4);

	Mono mon2_1(1.0, constant);
	Mono mon2_2(2.0, x_to1);


	poly1.add(mon1_1);
	poly1.add(mon1_2);
	poly1.add(mon1_3);
	poly1.add(mon1_4);
	poly1.add(mon1_5);

	poly2.add(mon2_1);
	poly2.add(mon2_2);
	
	std::cout << "\ndividend" << std::endl;
	print(poly1);
	std::cout << "\ndivisor" << std::endl;
	print(poly2);

	auto div = Division<mpq_class>(poly1, poly2);

}

void test_division_mult(){
	using ring = mpq_class;
	std::vector<unsigned> constant{ 0, 0, 0 };
	std::vector<unsigned> x { 1, 0, 0 };
	std::vector<unsigned> y { 0, 1, 0 };
	std::vector<unsigned> z { 0, 0, 1 };
	std::vector<unsigned> xyz { 1, 1, 1 };

	Mono mon1_1(1, constant);
	Mono mon1_2(1, x);
	Mono mon1_3(1, xyz);

	Mono mon2_1(1, constant);
	Mono mon2_2(1, y);

	Mono mon3_1(1, constant);
	Mono mon3_2(-1, z);

	Poly poly1;
	Poly poly2;
	Poly poly3;

	poly1.add(mon1_1);
	poly1.add(mon1_2);
	poly1.add(mon1_3);

	poly2.add(mon2_1);
	poly2.add(mon2_2);

	poly3.add(mon3_1);
	poly3.add(mon3_2);

	std::cout << "\ndividend" << std::endl;
	print(poly1);
	std::cout << "\ndivisor" << std::endl;
	print(poly2);
	print(poly3);

	std::vector<Poly> divisors { poly2, poly3 };
	auto div = Division<ring>(poly1, divisors);

}