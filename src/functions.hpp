#ifndef FUNC_HEADER
#define FUNC_HEADER


#include <iostream>
#include <string>
#include <memory>


class TFunction
{
public:
    virtual ~TFunction() = default;

    /*virtual double operator()() const = 0;
    virtual double getDeriv() const = 0;*/
    virtual const std::string toString() const = 0;
};

class IPolinomial : public TFunction
{

};

class TIdent : public IPolinomial
{
public:
    virtual const std::string toString() const override
    {
        return std::string("ident");
    }
};

class TConst : public IPolinomial
{
public:
    virtual const std::string toString() const override
    {
        return std::string("const");
    }
};


#endif
