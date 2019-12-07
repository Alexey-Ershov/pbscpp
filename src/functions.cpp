#include "functions.hpp"

#include <sstream>


const std::string IPolynomial::toString() const
{
    /*for (const auto& it: coeff_vect_) {
        std::cerr << it << " ";
    }
    std::cerr << std::endl;*/

    std::stringstream result;
    result << "f(x) = ";

    /*std::cerr << "DEBUG" << __LINE__  << ": " << result.str() << std::endl;*/

    if (coeff_vect_.at(0) != 0) {
        if (coeff_vect_.at(0) != 1) {
            result << coeff_vect_.at(0)
                   << "*";
        }

        result << "exp(x)";

        if (coeff_vect_.size() > 1) {
            result << " + ";
        }
    }

    for (unsigned i = 1; i < coeff_vect_.size(); i++) {
        if (i == 1) {
            /*std::cerr << "DEBUG" << __LINE__  << ": " << result.str() << std::endl;*/
            if (coeff_vect_.at(i) != 0 or 
                    (coeff_vect_.size() == 2 and coeff_vect_.at(0) == 0)) {
                
                result << coeff_vect_.at(i);

                if (i + 1 < coeff_vect_.size()) {
                    result << " + ";
                }
            }
            /*std::cerr << "DEBUG" << __LINE__  << ": " << result.str() << std::endl;*/

        } else {
            if (coeff_vect_.at(i) == 0) {
                continue;
            }

            if (coeff_vect_.at(i) != 1) {
                result << coeff_vect_.at(i)
                       << "*";
            }

            result << "x";

            if (i > 2) {
                result << "^"
                       << i - 1;
            }

            if (i + 1 < coeff_vect_.size()) {
                result << " + ";
            }
        }

        /*std::cerr << "DEBUG" << __LINE__  << ": " << result.str() << std::endl;*/
    }

    return result.str();
}