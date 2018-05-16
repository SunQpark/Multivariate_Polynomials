#pragma once
#include"Polynomials.hpp"

template<class ring>
int MultPolyOrder(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	Monomial<ring> lt_lhs = lhs.back();
	Monomial<ring> lt_rhs = rhs.back();

	return lexico_order(lt_lhs, lt_rhs);
}

template<class ring>
bool reducible(Polynomial<ring> &f, Polynomial<ring> &g){
	auto lm_g = g.back();
	
	for (auto &mon: f){
		auto mon_reducible = std::all_of(
			mon.exponents.begin(), 
			mon.exponents.end(),
			[iter = lm_g.exponents.begin()](ring e)mutable-> bool{return e > *(iter++);}
			);
		if (!mon_reducible){
			return false;
		}
	}
	return true;
}


template<class ring>
bool operator==(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	return MultPolyOrder(lhs, rhs) == 0;
}

template<class ring>
bool operator<(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	return MultPolyOrder(lhs, rhs) == 1;
}

template<class ring>
bool operator>(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	return MultPolyOrder(lhs, rhs) == -1;
}

template<class ring>
bool operator<=(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	return MultPolyOrder(lhs, rhs) != -1;
}

template<class ring>
bool operator>=(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	return MultPolyOrder(lhs, rhs) != 1;
}

template<class ring>
void poly_sort(std::vector<Polynomial<ring>> &polys) {
	std::sort(polys.begin(), polys.end());
}


template<class ring>
class Division {
private:
	unsigned num_var;
	Polynomial<ring> dividend;
	Polynomial<ring> divisor;
	std::vector<Polynomial<ring>> divisors;
	Polynomial<ring> quotient;
	std::vector<Polynomial<ring>> quotients;
	Polynomial<ring> remainder;

public:
	Division(Polynomial<ring> &poly1, Polynomial<ring> &poly2): dividend(poly1), divisor(poly2) {
		num_var = poly1.front().exponents.size();
		remainder = dividend;
		quotient = Polynomial<ring>();

		while(remainder >= divisor){
			division_step();
			if (remainder.is_zero()){ break; }
			if (!reducible(remainder, divisor)){ break; }
		}
		// print(multiplied);
		std::cout << "\nquotient: " << std::endl;
		print(quotient);
		std::cout << "\nremainder: " << std::endl;
		print(remainder);
	}

	Division(Polynomial<ring> &poly1, std::vector<Polynomial<ring>> &divisors): dividend(poly1), divisors(divisors) {
		for (auto &div: divisors){
			divisor = div;
			num_var = poly1.front().exponents.size();
			remainder = dividend;
			quotient = Polynomial<ring>();

			while(remainder >= divisor){
				division_step();
				if (remainder.is_zero()){ break; }
				if (!reducible(remainder, divisor)){ break; }
			}

			std::cout << "\nquotient: " << std::endl;
			quotients.push_back(quotient);
			print(quotient);
			std::cout << "\nremainder: " << std::endl;
			print(remainder);
		}

	}

private:

	void division_step() {
		remainder = dividend;
	
		Monomial<ring> temp_q;
		temp_q = dividend.back() / divisor.back();

		auto poly_temp = Polynomial<ring>(temp_q);
		auto multiplied = divisor * poly_temp;

		dividend -= multiplied;
		quotient.add(temp_q);
		remainder -= multiplied;
	}
};
