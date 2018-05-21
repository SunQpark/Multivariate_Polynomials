#pragma once
#include"Polynomials.hpp"

template<class ring>
int MultPolyOrder(Polynomial<ring> &lhs, Polynomial<ring> &rhs) {
	if (lhs.is_zero() && rhs.is_zero()){
		return 0;
	}
	else if (lhs.is_zero()){
		return 1;
	}
	else if (rhs.is_zero()){
		return -1;
	}

	Monomial<ring> lt_lhs = lhs.back();
	Monomial<ring> lt_rhs = rhs.back();

	return lexico_order(lt_lhs, lt_rhs);
}

template<class ring>
const bool reducible(const Polynomial<ring> &f, const Polynomial<ring> &g){
	// auto lm_g = g.back();
	
	// for (auto &mon: f){
	// 	bool mon_reducible = std::all_of(
	// 		mon.exponents.begin(), 
	// 		mon.exponents.end(),
	// 		[iter = lm_g.exponents.begin()](ring e)mutable-> bool{ return e >= *(iter++); }
	// 		);
	// 	if (mon_reducible){
	// 		return true;
	// 	}
	// }
	// return false;
	
	auto iter = f.rbegin();
	auto lt_g = g.back();
	// finding reducible monomial in dividend
	while (iter != f.rend()){
		auto mon_reducible = std::all_of(
			iter->exponents.begin(), 
			iter->exponents.end(),
			[i = lt_g.exponents.begin()](ring e)mutable-> bool{ return e >= *(i++); }
			);
		if (mon_reducible){
			break;
		}
		else{
			iter++;
		}
	}

	if (iter == f.rend()){
		return false;
	}
	else{
		return true;
	}

}

template<class ring>
const bool lead_reducible(const Polynomial<ring> &f, const Polynomial<ring> &g){
	auto lt_g = g.back();
	auto lt_f = f.back();
	return std::all_of(
			lt_f.exponents.begin(), 
			lt_f.exponents.end(),
			[iter = lt_g.exponents.begin()](ring e)mutable-> bool{return e >= *(iter++);}
			);
}

template<class ring>
const bool identical(const Polynomial<ring> &f, const Polynomial<ring> &g){
	auto iter1 = f.begin();
	auto iter2 = g.begin();

	while(iter1 != f.end() && iter2 != g.end()){

		if (*iter1 != *iter2){
			return false;
		}
		
	}
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
void poly_r_sort(std::vector<Polynomial<ring>> &polys) {
	std::sort(polys.begin(), polys.end());
	std::reverse(polys.begin(), polys.end());
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
			if (!reducible(remainder, divisor)){ break; }
			division_step();
			if (remainder.is_zero()){ break; }
		}
	}

	Division(Polynomial<ring> &poly, std::vector<Polynomial<ring>> &divisors): dividend(poly), divisors(divisors) {
		poly_sort(divisors);
		
		remainder = poly;
		for (auto &div: divisors){
			divisor = div;
			num_var = poly.front().exponents.size();
			remainder = dividend;
			quotient = Polynomial<ring>();
			// std::cout << "remainder: \n";
			// print(remainder);
			// std::cout << "divisor: \n";
			// print(divisor);
			// std::cout << "hi\n" << (remainder >= divisor) << "this will not be printed";
			
			while(remainder >= divisor){
				if (!reducible(remainder, divisor)){ break; }
				if (remainder.is_zero()){ break; }
				division_step();
			}
			quotients.push_back(quotient);
		}
	}

	Polynomial<ring> get_remainder(){
		return remainder;
	}

private:
	void division_step() {
		remainder = dividend;
		assert(!divisor.is_zero());
		auto target = dividend.rbegin();
		auto lt_divisor = divisor.back();
		// finding reducible monomial in dividend
		while (target != dividend.rend()){
			auto mon_reducible = std::all_of(
				target->exponents.begin(), 
				target->exponents.end(),
				[iter = lt_divisor.exponents.begin()](ring e)mutable-> bool{ return e >= *(iter++); }
				);
			if (mon_reducible){
				break;
			}
			else{
				target++;
			}
		}
		
		if (target != dividend.rend()){
			Monomial<ring> temp_q;
			temp_q = *target / lt_divisor;
			auto poly_temp = Polynomial<ring>(temp_q);
			auto multiplied = divisor * poly_temp;

			dividend -= multiplied;
			quotient.add(temp_q);
			remainder -= multiplied;
		}
		
	}
};
