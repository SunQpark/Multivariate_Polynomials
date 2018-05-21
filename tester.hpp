#pragma once
#include"Polynomials.hpp"
#include"Division.hpp"
// #include"MultVarDivision.hpp"

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
	using ring = mpq_class;
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

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
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

	std::vector<unsigned> constant{ 0, 0, 0 };
	std::vector<unsigned> x { 1, 0, 0 };
	std::vector<unsigned> y { 0, 1, 0 };
	std::vector<unsigned> z { 0, 0, 1 };
	std::vector<unsigned> xyz { 1, 1, 1 };
	std::vector<unsigned> sq_x { 2, 0, 0 };

	Mono mon1_1(1, constant);
	Mono mon1_2(1, x);
	Mono mon1_3(1, xyz);
	Mono mon1_4(1, sq_x);

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
	poly1.add(mon1_4);

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
	std::cout << "\nresult: \n";
	print(div.get_remainder());
}




void test_lcm(){
	
	using ring = mpq_class;
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

	std::vector<unsigned> constant{ 0, 0, 0 };
	std::vector<unsigned> x { 1, 0, 0 };
	std::vector<unsigned> y { 0, 1, 0 };
	std::vector<unsigned> z { 0, 0, 1 };
	std::vector<unsigned> xyz { 1, 1, 1 };

	Mono mon1_1(1, constant);
	Mono mon1_2(3, x);
	Mono mon1_3(4, xyz);

	auto result = lcm(mon1_2, mon1_3);

	print(result);

}

void test_reduction(){
	using ring = mpq_class;
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

	std::vector<unsigned> constant{ 0, 0, 0 };
	std::vector<unsigned> x { 1, 0, 0 };
	std::vector<unsigned> sq_x { 2, 0, 0 };
	std::vector<unsigned> cub_x { 3, 0, 0 };
	std::vector<unsigned> y { 0, 1, 0 };
	std::vector<unsigned> sq_y { 0, 2, 0 };
	std::vector<unsigned> z { 0, 0, 1 };
	std::vector<unsigned> sq_z { 0, 0, 2 };
	
	std::vector<unsigned> xy2 { 1, 2, 0 };
	std::vector<unsigned> xz2 { 1, 0, 2 };

	Mono mon1_1(-1, constant);
	Mono mon1_2(1, sq_x);
	Mono mon1_3(1, sq_y);
	Mono mon1_4(1, sq_z);

	Mono mon2_1(-1, sq_z);
	Mono mon2_2(1, cub_x);
	Mono mon2_3(1, sq_x);
	Mono mon2_4(-1, sq_y);
	
	Mono mon3_1(1, sq_z);
	Mono mon3_2(1, sq_y);
	Mono mon3_3(-1, x);
	Mono mon3_4(1, xy2);
	Mono mon3_5(1, xz2);
	Mono mon3_6(-1, sq_x);


	Poly poly1;
	Poly poly2;
	Poly poly3;

	poly1.add(mon1_1);
	poly1.add(mon1_2);
	poly1.add(mon1_3);
	poly1.add(mon1_4);

	poly2.add(mon2_1);
	poly2.add(mon2_2);
	poly2.add(mon2_3);
	poly2.add(mon2_4);

	poly3.add(mon3_1);
	poly3.add(mon3_2);
	poly3.add(mon3_3);
	poly3.add(mon3_4);
	poly3.add(mon3_5);
	poly3.add(mon3_6);


	print(poly1);
	print(poly2);
	print(poly3);
	std::cout << reducible(poly3, poly1) << std::endl;
	
	// std::cout << lead_reducible(poly2, poly1);
}

void test_groebner_1(){
	using ring = mpq_class;
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

	std::vector<unsigned> constant{ 0, 0, 0 };
	std::vector<unsigned> x { 1, 0, 0 };
	std::vector<unsigned> sq_x { 2, 0, 0 };
	std::vector<unsigned> cub_x { 3, 0, 0 };
	std::vector<unsigned> y { 0, 1, 0 };
	std::vector<unsigned> sq_y { 0, 2, 0 };
	std::vector<unsigned> z { 0, 0, 1 };
	std::vector<unsigned> sq_z { 0, 0, 2 };

	Mono mon1_1(-1, constant);
	Mono mon1_2(1, sq_x);
	Mono mon1_3(1, sq_y);
	Mono mon1_4(1, sq_z);

	Mono mon2_1(-1, sq_z);
	Mono mon2_2(1, cub_x);
	Mono mon2_3(1, sq_x);
	Mono mon2_4(-1, sq_y);

	Poly poly1;
	Poly poly2;

	poly1.add(mon1_1);
	poly1.add(mon1_2);
	poly1.add(mon1_3);
	poly1.add(mon1_4);


	poly2.add(mon2_1);
	poly2.add(mon2_2);
	poly2.add(mon2_3);
	poly2.add(mon2_4);


	std::vector<Poly> Ideal { poly1, poly2 };
	std::cout << "Ideal:" << std::endl;
	for (auto &pol: Ideal){
		print(pol);
	}


	// auto div = Division<ring>(poly2, poly1);
	// print(div.get_remainder());

	Groebner_Basis<ring> grob (Ideal);
}



void test_groebner_2(){
	using ring = mpq_class;
	using Mono = Monomial<ring>;
	using Poly = Polynomial<ring>;

	std::vector<unsigned> constant{ 0, 0, 0, 0 };
	std::vector<unsigned> x { 0, 0, 0, 1 };
	std::vector<unsigned> t { 1, 0, 0, 0 };
	std::vector<unsigned> t_sq { 2, 0, 0, 0 };
	std::vector<unsigned> t_cub { 3, 0, 0, 0 };
	std::vector<unsigned> y { 0, 0, 1, 0 };
	// std::vector<unsigned> sq_y { 0, 2, 0 };
	std::vector<unsigned> z { 0, 1, 0, 0 };
	// std::vector<unsigned> sq_z { 0, 0, 2 };

	Mono mon1_1(1, x);
	Mono mon1_2(-1, t);
	
	Mono mon2_1(-1, t_sq);
	Mono mon2_2(1, y);

	Mono mon3_1(-1, t_cub);
	Mono mon3_2(1, z);

	Poly poly1;
	Poly poly2;
	Poly poly3;

	poly1.add(mon1_1);
	poly1.add(mon1_2);

	poly2.add(mon2_1);
	poly2.add(mon2_2);

	poly3.add(mon3_1);
	poly3.add(mon3_2);

	std::vector<Poly> Ideal { poly1, poly2 };
	std::cout << "Ideal:" << std::endl;
	for (auto &pol: Ideal){
		print(pol);
	}


	// auto div = Division<ring>(poly2, poly1);
	// print(div.get_remainder());

	Groebner_Basis<ring> grob (Ideal);
}