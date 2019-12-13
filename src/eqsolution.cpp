#include "eqsolution.hpp"

#include <cstdlib>


std::optional<double> EqSolver::get_alpha(double x,
                                          double a0,
                                          double b0,
                                          unsigned recur_depth)
{
    if (recur_depth == 0) {
        return {};
    }

    double delta = 0.5 * eps_;
    double x_min = 0;
    double a_k = a0;
    double b_k = b0;

    auto g =
    [this](double x, double alpha)
    {
        return abs_fun_(x - alpha * abs_fun_.getDeriv(x));
    };
    
    do {
        double l_k = (a_k + b_k - delta) / 2;
        double m_k = (a_k + b_k + delta) / 2;

        double g_l_k = g(x, l_k);
        double g_m_k = g(x, m_k);

        if (std::isnan(g_l_k) or std::isnan(g_m_k)) {
            double length = std::abs(b0 - a0);
            double new_a0 = a0 + length / 4;
            double new_b0 = b0 - length / 4;
            
            return get_alpha(x, new_a0, new_b0, recur_depth - 1);
        }

        if (g_l_k <= g_m_k) {
            b_k = m_k;
        
        } else {
            a_k = l_k;
        }

    } while ((b_k - a_k) >= eps_);

    x_min = (a_k + b_k) / 2;

    return x_min;
}


std::optional<double> EqSolver::gr_descent()
{
    std::vector<double> x(max_iter_);
    x.at(0) = init_x_;

    unsigned k;
    for (k = 0; k <= max_iter_ - 2; k++) {
        auto alpha = get_alpha(x.at(k));
        if (not alpha.has_value()) {
            return {};
        }

        x[k + 1] = x.at(k) - alpha.value() * abs_fun_.getDeriv(x.at(k));

        if (std::abs(abs_fun_(x.at(k))) < eps_) {
            return x.at(k);
        }
    }

    return {};
}


std::optional<double> EqSolver::solveEquation(const IPolynomial& f)
{
    f_ = f;

    TFunction::Functor new_get_val_ftor_ =
    [this](double x)
    {
        if (f_(x) >= 0) {
            return f_(x);
        
        } else {
            return -f_(x);
        }
    };

    TFunction::Functor new_get_deriv_ftor_ =
    [this](double x)
    {
        if (f_(x) >= 0) {
            return f_.getDeriv(x);
        
        } else {
            return -f_.getDeriv(x);
        }
    };

    abs_fun_ = IPolynomial(new_get_val_ftor_, new_get_deriv_ftor_);

    return gr_descent();
}
