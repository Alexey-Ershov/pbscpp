#include "functions.hpp"

#include <cmath>

#include <sstream>


const std::string IPolynomial::toString() const
{
    /*for (const auto& it: coeff_vect_) {
        std::cerr << it << " ";
    }
    std::cerr << std::endl;*/

    std::stringstream res;
    res << "f(x) = ";

    /*std::cerr << "DEBUG" << __LINE__  << ": " << res.str() << std::endl;*/

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

    for (unsigned i = 1; i < coeff_vect_.size(); i++) {
        if (i == 1) {
            /*std::cerr << "DEBUG" << __LINE__  << ": " << res.str() << std::endl;*/
            if (coeff_vect_.at(i) != 0 or 
                    (coeff_vect_.size() == 2 and coeff_vect_.at(0) == 0)) {
                
                res << coeff_vect_.at(i);

                if (i + 1 < coeff_vect_.size()) {
                    res << " + ";
                }
            }
            /*std::cerr << "DEBUG" << __LINE__  << ": " << res.str() << std::endl;*/

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

        /*std::cerr << "DEBUG" << __LINE__  << ": " << res.str() << std::endl;*/
    }

    return res.str();
}

double IPolynomial::operator()(double x) const
{
    double res = 0;

    for (unsigned i = 0; i < coeff_vect_.size(); i++) {
        switch (i) {
          case 0: {
            res += coeff_vect_[i] * exp(x);
            break;
          }
          case 1: {
            res += coeff_vect_[i];
            break;
          }
          case 2: {
            res += coeff_vect_[i] * x;
            break;
          }
          default: {
            res += coeff_vect_[i] * pow(x, i - 1);
            break;
          }
        }
    }

    return res;
}

double IPolynomial::getDeriv(double x) const
{
    double res = 0;

    for (unsigned i = 0; i < coeff_vect_.size(); i++) {
        if (i == 0) {
            res += coeff_vect_[i] * exp(x);
        
        } else {
            if (i == 1) {
                res += coeff_vect_[i] * (i - 1);
            
            } else {
                res += coeff_vect_[i] * (i - 1) * pow(x, i - 2);
            }
        }
    }

    return res;
}
