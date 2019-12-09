#ifndef FUNC_HEADER
#define FUNC_HEADER


#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>


using VectOfDouble = std::vector<double>;


// Function to add two vectors.
VectOfDouble vectAddition(const VectOfDouble& lhs, const VectOfDouble& rhs);


// Base abstract class.
class TFunction
{
public:
    virtual ~TFunction() = default;

    // Methods to get string representation, to get the value of the function
    // at the given point and to get the value of derivative respectively.
    virtual const std::string toString() const = 0;
    virtual double operator()(double x) const = 0;
    virtual double getDeriv(double x) const = 0;
};

// Intermediate class to represent polynomial nature of basic functions.
class IPolynomial : public TFunction
{
public:
    IPolynomial() = default;
    IPolynomial(const VectOfDouble c_v)
        : coeff_vect_ { c_v }
    {}

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
        : IPolynomial({ 1 })
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

// Template function to add two functions.
template<class TL, class TR>
std::enable_if_t<std::is_base_of_v<TFunction, TL> or
                 std::is_base_of_v<TFunction, TR>,
        std::unique_ptr<TFunction>>
operator+(const TL& lhs, const TR& rhs)
{
    // If types are incompatible throw an exception.
    if constexpr (std::is_base_of_v<TFunction, TL> and
                  std::is_base_of_v<TFunction, TR>) {
        
        VectOfDouble res_vect = vectAddition(
                lhs.getCoeffVect(), rhs.getCoeffVect());

        return std::make_unique<IPolynomial>(res_vect);

    } else {
        throw std::logic_error("Error: Incompatible types");
    }
}


#endif
