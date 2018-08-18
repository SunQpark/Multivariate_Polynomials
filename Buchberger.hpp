#pragma once
#include"Polynomials.hpp"
#include"Division.hpp"
#include<iterator>

template<class ring>
ring ring_gcd(ring m, ring n) {
    if (m < 0){m *= -1;}
    if (n < 0){n *= -1;}
	while (m != n) {
		if (m > n) {
			m = m - n;
		}
		else if (n > m) {
			n = n - m;
		}
	}
	return m;
}


template<class ring>
Monomial<ring> lcm(Monomial<ring> &mon1, Monomial<ring> &mon2){

    auto exp_1 = mon1.exponents;
    auto exp_2 = mon2.exponents;

    auto coef1 = mon1.coef;
    auto coef2 = mon2.coef;

    assert(coef1 != 0 && coef2 != 0);

    auto gcd_coef = ring_gcd(coef1, coef2);
    ring lcm_coef = abs(coef1 * coef2) / gcd_coef;

    std::vector<unsigned> lcm_exp(exp_1.size());
    for(unsigned i = 0; i <= exp_1.size(); i++){
        lcm_exp[i] = (exp_1[i] > exp_2[i] ? exp_1[i] : exp_2[i]);
    }

    Monomial<ring> result(lcm_coef, lcm_exp);
    return result;
}

template<class ring>
class Groebner_Basis{
public:
    Groebner_Basis() {

    }

    Groebner_Basis(std::vector<Polynomial<ring>> &Ideal){
        compute_GB(Ideal);
    }

private:
    std::vector<Polynomial<ring>> basis;
    
    void compute_GB(std::vector<Polynomial<ring>> &Ideal){
        basis = Ideal;
        std::vector<Polynomial<ring>> S;
        std::vector<Polynomial<ring>> divisors;
        Polynomial<ring> S_poly;
        Polynomial<ring> r;
        while (true){
            poly_sort(basis);
            for (unsigned i = 0; i < basis.size() - 1 ; i++){
            S.clear();
                for (unsigned j = i; j < basis.size(); j++){
                    S_poly = buchberger(basis[i], basis[j]);
                    r = Division<ring>(S_poly, basis).get_remainder();
                    if (!r.is_zero()){
                        S.push_back(r);
                    }
                    if (S.empty()){
                        continue;
                    }
                }       

            }
            
            if (S.empty()){
                break;
            }
            else{
                for (auto &e:S){
                    basis.push_back(e);
                }
            }
            
        }


        std::cout << "basis: \n";
        for (auto &p : basis){
            print(p);
        }
        std::cout << basis.size() << std::endl;
        
    }

    Polynomial<ring> buchberger(Polynomial<ring> &poly1, Polynomial<ring> &poly2){
        auto lt_poly1 = poly1.back();
        auto lt_poly2 = poly2.back();
        
        auto lcm_lt = lcm(lt_poly1, lt_poly2);
        auto factor1 = lcm_lt / lt_poly1;
        auto factor2 = lcm_lt / lt_poly2;

        Polynomial<ring> S_poly = poly1 * factor1;
        Polynomial<ring> temp_p = poly2 * factor2;
        S_poly -= temp_p;
        return S_poly;
    }
};