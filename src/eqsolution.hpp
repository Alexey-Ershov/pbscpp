#ifndef EQSOLUTION
#define EQSOLUTION


#include "functions.hpp"

#include <optional>


// Class to solve equations fo the form f(x) = 0.
class EqSolver
{
public:
    EqSolver(unsigned max_iter = 1000,
             int init_x = 1,
             double eps = 0.001)
        
        : max_iter_ { max_iter },
          init_x_ { init_x },
          eps_ { eps }
    {}

    // Method to solve the equation.
    std::optional<double> solveEquation(const IPolynomial& f);

private:
    IPolynomial f_;
    IPolynomial abs_fun_;
    unsigned max_iter_;
    int init_x_;
    double eps_;

    // Method to get minimum point of the function on [a0, b0] for the
    // steepest descent.
    std::optional<double> get_alpha(double x,
                                    double a0 = -10000,
                                    double b0 =  10000,
                                    unsigned recur_depth = 10);

    // Gradient descent algorithm.
    std::optional<double> gr_descent();
};


#endif
