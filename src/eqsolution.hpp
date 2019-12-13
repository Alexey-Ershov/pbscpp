#ifndef EQSOLUTION
#define EQSOLUTION


#include "functions.hpp"

#include <optional>


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

    std::optional<double> solveEquation(const IPolynomial& f);

private:
    IPolynomial f_;
    IPolynomial abs_fun_;
    unsigned max_iter_;
    int init_x_;
    double eps_;

    std::optional<double> get_alpha(double x,
                                    double a0 = -10000,
                                    double b0 =  10000,
                                    unsigned recur_depth = 10);

    std::optional<double> gr_descent();
};


#endif
