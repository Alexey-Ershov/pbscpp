#ifndef FUNC_HEADER
#define FUNC_HEADER


#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

#include <cmath>


using VectOfDouble = std::vector<double>;


// Function to add two vectors.
VectOfDouble vectAddition(const VectOfDouble& lhs, const VectOfDouble& rhs);


// Base abstract class.
class TFunction
{
public:
    using Functor = std::function<double(double)>;
    
    virtual ~TFunction() = default;

    // Methods to get string representation, to get the value of the function
    // at the given point and to get the value of derivative respectively.
    virtual const std::string toString() const = 0;
    virtual double operator()(double x) const = 0;
    virtual double getDeriv(double x) const = 0;

    Functor get_val_ftor_;
    Functor get_deriv_ftor_;
};



// Intermediate class to represent polynomial nature of basic functions.
class IPolynomial : public TFunction
{
public:
    IPolynomial()
    {
        get_val_ftor_ = basic_get_val_lambda_;
        get_deriv_ftor_ = basic_get_deriv_lambda_;
    }

    IPolynomial(const VectOfDouble c_v)
        : coeff_vect_ { c_v }
    {
        get_val_ftor_ = basic_get_val_lambda_;
        get_deriv_ftor_ = basic_get_deriv_lambda_;
    }

    IPolynomial(const Functor& g_v, const Functor& g_d)
    {
        get_val_ftor_ = g_v;
        get_deriv_ftor_ = g_d;
    }
    
    const VectOfDouble& getCoeffVect() const
    {
        return coeff_vect_;
    }

    // Current class implement pure virtual functions in general terms.
    virtual const std::string toString() const override final;
    virtual double operator()(double x) const override final;
    virtual double getDeriv(double x) const override final;

protected:
    // Coefficient vector of the polynom:
    // c0*exp(x) + c1 + c2*x + c3*x^2 + c4*x^3 + ...
    // where c0, c1, ... are elements of coeff_vect_.
    VectOfDouble coeff_vect_;

    // Get value lambda function for basic functions.
    Functor basic_get_val_lambda_ =
    [this](double x)
    {
        double res = 0;

        // Compute function value in for loop.
        for (unsigned i = 0; i < this->getCoeffVect().size(); i++) {
            switch (i) {
              case 0: {
                res += this->getCoeffVect()[i] * exp(x);
                break;
              }
              case 1: {
                res += this->getCoeffVect()[i];
                break;
              }
              case 2: {
                res += this->getCoeffVect()[i] * x;
                break;
              }
              default: {
                res += this->getCoeffVect()[i] * pow(x, i - 1);
                break;
              }
            }
        }

        return res;
    };

    // Get derivative lambda function for basic functions.
    Functor basic_get_deriv_lambda_ =
    [this](double x)
    {
        double res = 0;

        // Compute derivative value in for loop.
        for (unsigned i = 0; i < coeff_vect_.size(); i++) {
            if (i == 0) {
                res += coeff_vect_[i] * exp(x);
            
            } else if (i > 1) {
                    res += coeff_vect_[i] * (i - 1) * pow(x, i - 2);
            }
        }

        return res;
    };
};

// Idential function.
class TIdent : public IPolynomial
{
public:
    TIdent(double opt) {}
    TIdent(const VectOfDouble& opt) {}

    TIdent()
        : IPolynomial({ 0, 0, 1 })
    {}
};

// Constant function.
class TConst : public IPolynomial
{
public:
    TConst() {}
    TConst(const VectOfDouble& opt) {}
    
    TConst(double opt)
        : IPolynomial({ 0, opt })
    {}
};

// Power function.
class TPower : public IPolynomial
{
public:
    TPower() {}
    TPower(const VectOfDouble& opt) {}
    
    TPower(int opt)
        : IPolynomial()
    {
        coeff_vect_ = {};
        for (int i = 0; i < opt + 1; i++) {
            coeff_vect_.emplace_back(0);
        }

        coeff_vect_.emplace_back(1);
    }
};

// Exponential function.
class TExp : public IPolynomial
{
public:
    TExp(const VectOfDouble& opt) {}
    TExp(double opt) {}

    TExp()
        : IPolynomial(std::vector<double>({ 1 }))
    {}
};

// Polynomial function.
class TPolynomial : public IPolynomial
{
public:
    TPolynomial() {}
    TPolynomial(double opt) {}
    TPolynomial(const VectOfDouble& opt)
        : IPolynomial(opt)
    {
        coeff_vect_.emplace(coeff_vect_.begin(), 0);
    }
};


// Template function to implement arithmetic operations with functions.

template<class TL, class TR>
std::enable_if_t<std::is_base_of_v<TFunction, TL> or
                 std::is_base_of_v<TFunction, TR>,
        std::unique_ptr<TFunction>>
operator+(const TL& lhs, const TR& rhs)
{
    // If types are incompatible throw an exception.
    if constexpr (std::is_base_of_v<TFunction, TL> and
                  std::is_base_of_v<TFunction, TR>) {
        
        TFunction::Functor new_get_val_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_val_ftor_(x) + rhs.get_val_ftor_(x);
        };

        TFunction::Functor new_get_deriv_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_deriv_ftor_(x) + rhs.get_deriv_ftor_(x);
        };

        return std::make_unique<IPolynomial>(new_get_val_ftor_,
                                             new_get_deriv_ftor_);

    } else {
        throw std::logic_error("Error: Incompatible types");
    }
}

template<class TL, class TR>
std::enable_if_t<std::is_base_of_v<TFunction, TL> or
                 std::is_base_of_v<TFunction, TR>,
        std::unique_ptr<TFunction>>
operator-(const TL& lhs, const TR& rhs)
{
    // If types are incompatible throw an exception.
    if constexpr (std::is_base_of_v<TFunction, TL> and
                  std::is_base_of_v<TFunction, TR>) {
        
        TFunction::Functor new_get_val_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_val_ftor_(x) - rhs.get_val_ftor_(x);
        };

        TFunction::Functor new_get_deriv_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_deriv_ftor_(x) - rhs.get_deriv_ftor_(x);
        };

        return std::make_unique<IPolynomial>(new_get_val_ftor_,
                                             new_get_deriv_ftor_);

    } else {
        throw std::logic_error("Error: Incompatible types");
    }
}

template<class TL, class TR>
std::enable_if_t<std::is_base_of_v<TFunction, TL> or
                 std::is_base_of_v<TFunction, TR>,
        std::unique_ptr<TFunction>>
operator*(const TL& lhs, const TR& rhs)
{
    // If types are incompatible throw an exception.
    if constexpr (std::is_base_of_v<TFunction, TL> and
                  std::is_base_of_v<TFunction, TR>) {
        
        TFunction::Functor new_get_val_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_val_ftor_(x) * rhs.get_val_ftor_(x);
        };

        TFunction::Functor new_get_deriv_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_deriv_ftor_(x) * rhs.get_val_ftor_(x) +
                   lhs.get_val_ftor_(x) * rhs.get_deriv_ftor_(x);
        };

        return std::make_unique<IPolynomial>(new_get_val_ftor_,
                                             new_get_deriv_ftor_);

    } else {
        throw std::logic_error("Error: Incompatible types");
    }
}

template<class TL, class TR>
std::enable_if_t<std::is_base_of_v<TFunction, TL> or
                 std::is_base_of_v<TFunction, TR>,
        std::unique_ptr<TFunction>>
operator/(const TL& lhs, const TR& rhs)
{
    // If types are incompatible throw an exception.
    if constexpr (std::is_base_of_v<TFunction, TL> and
                  std::is_base_of_v<TFunction, TR>) {
        
        TFunction::Functor new_get_val_ftor_ =
        [&lhs, &rhs](double x)
        {
            return lhs.get_val_ftor_(x) / rhs.get_val_ftor_(x);
        };

        TFunction::Functor new_get_deriv_ftor_ =
        [&lhs, &rhs](double x)
        {
            return (lhs.get_deriv_ftor_(x) * rhs.get_val_ftor_(x) -
                    lhs.get_val_ftor_(x) * rhs.get_deriv_ftor_(x)) /
                   (rhs.get_val_ftor_(x) * rhs.get_val_ftor_(x));
        };

        return std::make_unique<IPolynomial>(new_get_val_ftor_,
                                             new_get_deriv_ftor_);

    } else {
        throw std::logic_error("Error: Incompatible types");
    }
}


#endif
