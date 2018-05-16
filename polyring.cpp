#include <iostream>
#include <vector>
//the following library is for rational numbers; it is not installed in the computer lab
//compile and link with
// g++ -std=c++11 -o poly polyring.cpp -lgmp -lgmpxx
#include <gmp.h>

using namespace std;


//The following is an example of operator overloading.
//This is not an efficient implementation, and some functions should be done differently
//For example, it is more common (and faster) to do the + operator inside the class
//This version has the += operator inside the class, but the + operator is defined outside.
template <typename ring>
class polynomial_ring
{
public:
    vector<ring> coeff;

    template <typename replacement_name>
    friend std::ostream& operator<<(std::ostream&, const polynomial_ring<replacement_name>&);

    void set_monomial(unsigned int deg, ring set_coeff);
    void zero();

    //constructor: maybe this should do something in the future.
    polynomial_ring(){}

    int leading_index();
    ring return_coeff(unsigned int i);

    polynomial_ring& operator*=(const polynomial_ring& rhs);
    polynomial_ring& operator/=(const polynomial_ring& rhs);
    polynomial_ring& operator%=(const polynomial_ring& rhs);
    polynomial_ring& operator+=(const polynomial_ring& rhs);
    polynomial_ring& operator-=(const polynomial_ring& rhs);
    polynomial_ring& operator=(const polynomial_ring& rhs);
    polynomial_ring& operator=(const ring& rhs);

};

template <typename ring>
int polynomial_ring<ring>::leading_index()
{
    int i=coeff.size()-1;
    while( i>=0 && coeff[i]==0 )
    {
        i--;
    }
    return i;
}

template <typename ring>
ring polynomial_ring<ring>::return_coeff(unsigned int i)
{
    if( i<coeff.size() )
        return coeff[i];
    else
        return 0;
}


template <typename ring>
void polynomial_ring<ring>::set_monomial(unsigned int deg, ring set_coeff)
{
    if( deg<coeff.size() )
    {
        coeff[deg]=set_coeff;
    }
    else
    {
        while( deg>=coeff.size() )
        {
            coeff.push_back(0);
        }
        coeff[deg]=set_coeff;
    }
}

template <typename ring>
void polynomial_ring<ring>::zero()
{
    coeff.clear();
}


//overloading the comparison operator: two polynomials are equal if and only if all coefficients are equal.
template <typename ring>
bool operator==(const polynomial_ring<ring>& lhs, const polynomial_ring<ring>& rhs)
{
    //innocent until proven guilty
    bool comparison_holds=true;
    unsigned int i;
    unsigned int min_deg;
    const polynomial_ring<ring> *larger_poly;

    if( lhs.coeff.size()<rhs.coeff.size() )
    {
        larger_poly=&rhs;
        min_deg=lhs.coeff.size();
    }
    else
    {
        larger_poly=&lhs;
        min_deg=rhs.coeff.size();
    }
    i=0;
    while( i<min_deg && comparison_holds==true )
    {
        comparison_holds= (lhs.coeff[i]==rhs.coeff[i]);
        i+=1;
    }
    if( comparison_holds==false )
        return false;
    //so far all coefficients are equal. Now look at untested
    i=min_deg; //not necessary: this should hold after the previous tests.
    while( i<(*larger_poly).coeff.size() && comparison_holds==true )
    {
        comparison_holds= ( (*larger_poly).coeff[i]==0 );
        i+=1;
    }
    return comparison_holds;
}

template <typename ring>
bool operator!=(const polynomial_ring<ring>& lhs, const polynomial_ring<ring>& rhs)
{
    return !(lhs==rhs);
}

template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator+=(const polynomial_ring<ring>& rhs)
{
        unsigned int i;
        while( rhs.coeff.size()>coeff.size() )
        {
            //cout << "pushing" << endl;
            coeff.push_back(0);
        }
        for( i=0; i<rhs.coeff.size(); i+=1 )
        {
            coeff[i]+=rhs.coeff[i];
        }
        return *this;
}


template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator-=(const polynomial_ring<ring>& rhs)
{
        unsigned int i;
        while( rhs.coeff.size()>coeff.size() )
        {
            coeff.push_back(0);
        }
        for( i=0; i<rhs.coeff.size(); i+=1 )
        {
            coeff[i]-=rhs.coeff[i];
        }
        return *this;
}

//simple classical multiplication
template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator*=(const polynomial_ring<ring>& rhs)
{
    unsigned int i, j;
    vector<ring> tmp_poly;
    ring tmp;

    for( i=0; i<rhs.coeff.size()+this->coeff.size(); i+=1 )
    {
        tmp=0;
        for( j=0; j<=i; j+=1 )
        {
            if( j<this->coeff.size() && (i-j)<rhs.coeff.size() )
                tmp+=this->coeff[j]*rhs.coeff[i-j];
        }
        tmp_poly.push_back(tmp);
    }

    this->coeff.clear();
    for( i=0; i<tmp_poly.size(); i+=1 )
    {
        this->coeff.push_back( tmp_poly[i] );
    }
    return *this;
}


//assignment operator for polys
template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator=(const polynomial_ring<ring>& rhs)
{
    //check for self-assignment first to save some time (the code should have been correct before this check too)
    if( this!=&rhs )
    {
        unsigned int i;
        while( rhs.coeff.size()>coeff.size() )
        {
            coeff.push_back(0);
        }
        for( i=0; i<rhs.coeff.size(); i+=1 )
        {
            coeff[i]=rhs.coeff[i];
        }
        while( rhs.coeff.size()<coeff.size() )
        {
            coeff.pop_back();
        }
    }
    return *this;
}

//assignment operator for polys: (assign constant)
template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator=(const ring& rhs)
{
    coeff.clear();
    coeff.push_back(rhs);
    return *this;
}

template <typename ring>
std::ostream& operator<<(std::ostream& out, const polynomial_ring<ring>& f)
{
    bool nothing_printed=true;
    for( unsigned int i=0; i<f.coeff.size(); i+=1 )
    {
        if( f.coeff[i]<0 )
        {
            nothing_printed=false;
            out << f.coeff[i] << "x^" << i;
        }
        if( f.coeff[i]>0 )
        {
            nothing_printed=false;
            out << "+" << f.coeff[i] << "x^" << i;
        }
    }
    if( nothing_printed )
        out << "0";
    return out;
}

template <typename ring>
polynomial_ring<ring> operator*(polynomial_ring<ring> lhs, const polynomial_ring<ring>& rhs)
{
    return lhs *= rhs;
}

template <typename ring>
polynomial_ring<ring> operator+(polynomial_ring<ring> lhs, const polynomial_ring<ring>& rhs)
{
    return lhs += rhs;
}

template <typename ring>
polynomial_ring<ring> operator-(polynomial_ring<ring> lhs, const polynomial_ring<ring>& rhs)
{
    return lhs -= rhs;
}


//this template function takes a and b as input and should return division with remainder
// a quo b should end up in q, a rem b should end up in r.
template <typename ring>
void poly_div_rem(polynomial_ring<ring>& a, polynomial_ring<ring>& b, polynomial_ring<ring>& q, polynomial_ring<ring>& r)
{
    //fill in the function.

}

template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator/=(const polynomial_ring<ring>& rhs)
{
    polynomial_ring<ring> tmp1, tmp2, q,r;
    tmp1=*this;
    tmp2=rhs;
    poly_div_rem<ring>( tmp1, tmp2, q, r);

    *this=q;
    return *this;
}

template <typename ring>
polynomial_ring<ring>& polynomial_ring<ring>::operator%=(const polynomial_ring<ring>& rhs)
{
    polynomial_ring<ring> tmp1, tmp2, q,r;
    tmp1=*this;
    tmp2=rhs;
    poly_div_rem<ring>( tmp1, tmp2, q, r);

    *this=r;
    return *this;
}

template <typename ring>
polynomial_ring<ring> operator/(polynomial_ring<ring> lhs, const polynomial_ring<ring>& rhs)
{
    return lhs /= rhs;
}

template <typename ring>
polynomial_ring<ring> operator%(polynomial_ring<ring> lhs, const polynomial_ring<ring>& rhs)
{
    return lhs %= rhs;
}



template <typename euclidean_ring>
euclidean_ring gcd( euclidean_ring& a, euclidean_ring& b)
{
    euclidean_ring r, z;
    z=0;
    r=a%b;
    cout << "Remainder=" << r << endl;
    if( r==z )
        return b;
    else
        return gcd(b,r);
}


//the main serves just to illustrate some of the operations.
int main()
{
    mpq_class a, b, c;

    //from the gmp library
    a = 12345;
    b = "-5678/7825652552";
    c = a+b;
    cout << "The sum is " << c << "\n";
    cout << "Absolute value is " << abs(c) << "\n";

    polynomial_ring<mpq_class> rp1, rp2, rp3, q, r;

    //we set rp1=b+1*x
    rp1=0;
    rp1.set_monomial(0,-764);
    rp1.set_monomial(1, 979);
    rp1.set_monomial(2,-741);
    rp1.set_monomial(3, 814);
    rp1.set_monomial(4,-65);
    rp1.set_monomial(5, 824);


    rp2=0;
    rp2.set_monomial(0, 617);
    rp2.set_monomial(1, 916);
    rp2.set_monomial(2, 880);
    rp2.set_monomial(3, 663);
    rp2.set_monomial(4, 216);



    cout << rp1 << endl;
    cout << rp2 << endl;

//after finishing the division with remainder algorithm, / and % should work as expected: the gcd function will then also work.
//    cout << gcd(rp1, rp2) << endl;


    return 0;
}
