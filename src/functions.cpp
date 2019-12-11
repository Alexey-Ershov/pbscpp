#include "functions.hpp"



#include <sstream>


const std::string IPolynomial::toString() const
{
    std::stringstream res;
    res << "f(x) = ";

    // If there is an exponent part.
    if (coeff_vect_.at(0) != 0) {
        if (coeff_vect_.at(0) != 1) {
            res << coeff_vect_.at(0)
                   << "*";
        }

        res << "exp(x)";

        if (coeff_vect_.size() > 1) {
            res << " + ";
        }
    }

    // For every coefficient print corresponding string representation.
    for (unsigned i = 1; i < coeff_vect_.size(); i++) {
        if (i == 1) {
            if (coeff_vect_.at(i) != 0 or 
                    (coeff_vect_.size() == 2 and coeff_vect_.at(0) == 0)) {
                
                res << coeff_vect_.at(i);

                if (i + 1 < coeff_vect_.size()) {
                    res << " + ";
                }
            }

        } else {
            if (coeff_vect_.at(i) == 0) {
                continue;
            }

            if (coeff_vect_.at(i) != 1) {
                res << coeff_vect_.at(i)
                       << "*";
            }

            res << "x";

            if (i > 2) {
                res << "^"
                       << i - 1;
            }

            if (i + 1 < coeff_vect_.size()) {
                res << " + ";
            }
        }
    }

    return res.str();
}



double IPolynomial::operator()(double x) const
{
    /*double res = 0.0;

    for (const auto& f: get_val_ftor_) {
        res += f(*this, x);
    }

    return res;*/

    return get_val_ftor_(x);
}

double IPolynomial::getDeriv(double x) const
{
    return get_deriv_ftor_(x);
}


VectOfDouble vectAddition(const VectOfDouble& lhs, const VectOfDouble& rhs)
{
    // Check for the longest vector.
    bool l_longer_r = false;
    if (lhs.size() > rhs.size()) {
        l_longer_r = true;
    }

    // Copy the longest vector.
    VectOfDouble res;
    if (l_longer_r) {
        res = std::move(lhs);
    
    } else {
        res = std::move(rhs);
    }

    // Add every element of the vectors.
    for (unsigned i = 0; i < std::min(lhs.size(), rhs.size()); i++) {
        if (l_longer_r) {
            res[i] += rhs[i];
        
        } else {
            res[i] += lhs[i];
        }
    }

    return res;
}

