#ifndef FUNC_HEADER
#define FUNC_HEADER


#include <iostream>
#include <string>
#include <memory>
#include <vector>


class TFunction
{
public:
    /*TFunction() = default;
    TFunction(double opt) {}
    TFunction(const std::vector<double>& opt) {}*/
    virtual ~TFunction() = default;

    /*virtual double operator()() const = 0;
    virtual double getDeriv() const = 0;*/
    virtual const std::string toString() const = 0;
};

class IPolynomial : public TFunction
{
protected:
    std::vector<double> coeff_vect_;

    IPolynomial() = default;

    IPolynomial(const std::vector<double> c_v)
        : coeff_vect_ { c_v }
    {}

    virtual const std::string toString() const override;
};

class TIdent : public IPolynomial
{
public:
    TIdent(double opt) {}
    TIdent(const std::vector<double>& opt) {}

    TIdent()
        : IPolynomial({ 0, 0, 1 })
    {}
};

class TConst : public IPolynomial
{
public:
    TConst() {}
    TConst(const std::vector<double>& opt) {}
    
    TConst(double opt)
        : IPolynomial({ 0, opt })
    {}
};

class TPower : public IPolynomial
{
public:
    TPower() {}
    TPower(const std::vector<double>& opt) {}
    
    TPower(int opt)
    {
        coeff_vect_ = {};
        for (int i = 0; i < opt + 1; i++) {
            coeff_vect_.emplace_back(0);
        }

        coeff_vect_.emplace_back(1);
    }
};

class TExp : public IPolynomial
{
public:
    TExp(const std::vector<double>& opt) {}
    TExp(double opt) {}

    TExp()
        : IPolynomial({ 1 })
    {}
};

class TPolynomial : public IPolynomial
{
public:
    TPolynomial() {}
    TPolynomial(double opt) {}
    TPolynomial(const std::vector<double>& opt)
        : IPolynomial(opt)
    {
        coeff_vect_.emplace(coeff_vect_.begin(), 0);
    }
};


#endif
