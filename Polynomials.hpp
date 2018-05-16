#pragma once
#include<iostream>
#include<vector>
#include<cassert>
#include<list>
#include<cmath>
#include<iterator>
#include<algorithm>
#include<gmpxx.h>


template<class ring>
class Monomial;
template<class ring>
const int lexico_order(const Monomial<ring> &lhs, const Monomial<ring> &rhs);
template<class ring>
const void print(const Monomial<ring> &mon);

template<class ring>
class Monomial {
public:
	Monomial(): coef(0.0) {

	}

	Monomial(unsigned num_var): coef(0.0) {
		exponents.resize(num_var, 0);
	}

	template<class T>
	Monomial(ring coef, T values): coef(coef) {
		set_exps(values);
	}

	ring coef;
	std::vector<ring> exponents;

private:
	template<class T>
	void set_exps(T values) {
		exponents.resize(values.size());

		for (unsigned i = 0; i < values.size(); i++) {
			exponents[i] = values[i];
		}
	}

public:
	// multiplication between monomials
	Monomial<ring>& operator*=(const Monomial<ring> &other) {
		this->coef *= other.coef;
		assert(this->exponents.size() == other.exponents.size());
		auto iter = other.exponents.begin();
		for (auto &e: this->exponents) {
			e += *iter;
			iter++;
		}
		return *this;
	}

	//division by other monomial
	Monomial<ring> operator/=(const Monomial<ring> &other) {
		assert(other.coef != 0);
		this->coef /= other.coef;
		assert(this->exponents.size() == other.exponents.size());
		auto iter = other.exponents.begin();
		for (auto &e : this->exponents) {
			if (e < *iter) {
				// return zero Monomial for divisor larger then dividend
				// return Monomial<ring>(this->exponents.size());
				
				this->coef = 0.0;
				break;
			}
			e -= *iter;
			iter++;
		}
		return *this;
	}

	Monomial<ring> operator/(const Monomial<ring> &other) {
		auto temp = *this;
		return temp /= other;
	}
};


template<class ring>
const int lexico_order(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	//return 1 if lhs < rhs, -1 if lhs > rhs, 0 otherwise
	unsigned num_var = lhs.exponents.size();
	assert(rhs.exponents.size() == num_var);
	
	int result = 0;
	
	for (unsigned i = 0; i < num_var; i++) {
		if (rhs.exponents[i] > lhs.exponents[i]) {
			result = 1;
			break;
		}
		else if (rhs.exponents[i] < lhs.exponents[i]) {
			result = -1;
			break;
		}
		else {
			continue;
		}
	}
	return result;
}

//implementation of Monomial ordering
template<class ring>
bool operator==(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	return (lexico_order(lhs, rhs) == 0);
}

template<class ring>
bool operator<=(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	return !(lexico_order(lhs, rhs) == -1);
}

template<class ring>
bool operator>=(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	return rhs <= lhs;
}

template<class ring>
bool operator>(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	return !(lhs <= rhs);
}

template<class ring>
bool operator<(const Monomial<ring> &lhs, const Monomial<ring> &rhs) {
	return rhs > lhs;
}


template<class ring>
class Polynomial: public std::list<Monomial<ring>> {
public:
	Polynomial() {

	}

	Polynomial(unsigned num_var) {
		
	}

	Polynomial(Monomial<ring> &singleton) {
		this->push_front(singleton);
	}

	void add(Monomial<ring> &mon) {
		// just add monomial if this polynomial is empty
		if (this->empty()) {
			this->push_front(mon);
		}
		else {
			// find the place where monomial be inserted
			auto iter = std::find_if(this->begin(), this->end(),
				[mon](const Monomial<ring> &m) ->bool {return m >= mon; });

			// compare and add monomial to appropriate location
			if (iter == this->end()) {
				this->insert(iter, mon);
			}
			else if (*iter == mon) {
				(*iter).coef += mon.coef;
			}
			else {
				this->insert(iter, mon);
			}

		}
	}

	unsigned degree() const{
		return this->back().exponents[0];
	}



	const bool is_zero() const{
		if (this->empty()) {
			return true;
		}
		for (auto &mon : *this) {
			if (mon.coef != 0) {
				return false;
			}
		}
		return true;
	}

public:
	Polynomial<ring> &operator+=(Polynomial<ring> &other) {
		
		//for zero polynomials
		if (this->is_zero()) {
			*this = other;
			return *this;
		}
		else if (other.is_zero()) {
			return *this;
		}

		auto iter1 = this->begin();
		auto iter2 = other.begin();
		while (iter2 != other.end()) {
			if (iter1 == this->end()) {
				this->push_back(*iter2);
				iter2++;
			}
			else {
				// compare and add monomials
				int order = lexico_order(*iter1, *iter2);
				switch (order)
				{
				case(1):
					iter1++;
					break;
				case(0):
					(*iter1).coef += (*iter2).coef;
					// cancellation
					if ((*iter1).coef == 0) {
						this->erase(iter1++);
					}
					iter2++;
					break;
				case(-1):
					this->insert(iter1, *iter2);
					iter2++;
					break;
				default:
					std::cout << "something's wrong";
					break;
				}
			}
		}
		return *this;
	}

	Polynomial<ring> &operator-=(Polynomial<ring> &other) {
		auto temp = other;
		temp *= ring(-1);
		*this += temp;
		return *this;
	}

	//scalar multiplication
	Polynomial<ring> &operator*=(ring scalar) {
		for (auto &mon : *this) {
			mon.coef *= scalar;
		}
		return *this;
	}

	//multiplication by another polynomial
	Polynomial<ring> operator*(Polynomial<ring> &other) {
		return this->naive_mult(other);
	}

private:
	//multiplication by another polynomial(not optimal)
	Polynomial<ring> naive_mult(Polynomial<ring> &other) {
		Monomial<ring> temp_mon;
		Polynomial<ring> result;

		for (const auto &m1 : other) {
			for (const auto &m2 : *this) {
				temp_mon = m1;
				temp_mon *= m2;
				result.add(temp_mon);
				temp_mon.coef = 0;
			}
		}

		return result;
	}



};

template<class ring>
const void print(const Monomial<ring> &mon) {
	std::cout << mon.coef;
	
	unsigned count_var = 0;
	
	bool is_const = std::all_of(mon.exponents.begin(), mon.exponents.end(), [](ring r) ->bool{ return r == 0; });

	if (!is_const) {
		for (auto e : mon.exponents) {
			if (e != 0){
				std::cout << "x_{" << count_var << "}";
				if (e != 1) {
					std::cout << "^{" << e << "}";
				}
			}
			count_var++;
			
		}
	}
	// return false if this function print nothing ie, zero monomial
	// return is_const && (mon.coef == 0.0); 
}

template<class ring>
const void print(const Polynomial<ring> &pol) {
	bool first = true;
	if (pol.is_zero()){
		std::cout << "printing zero polynomial";
	}
	else{
		for (const auto &mon: pol) {
			if (!first) {
				std::cout << " + ";
			}
			if (first) { first = false; }
			print(mon);
		}
	}
	std::cout << std::endl;
}
