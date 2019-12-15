#include "functions.hpp"
#include "factory.hpp"
#include "eqsolution.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>


#define ITER_NUM 10
#define MAXRAND 100
#define MAXRAND_EXP 5
#define EPS 0.001


VectOfDouble genPolyCoeffs()
{
    std::srand(static_cast<unsigned int>(time(0)));

    unsigned degree = std::rand() % (MAXRAND_EXP + 1);
    VectOfDouble res;

    for (unsigned i = 0; i < degree + 1; i++) {
        res.emplace_back(std::rand() % MAXRAND + 1);
    }

    return res;
}

std::string getPolyStrRepr(const VectOfDouble& coeff_vect)
{
    std::stringstream res;
    res << "f(x) = ";

    for (unsigned i = 0; i < coeff_vect.size(); i++) {
        if (i == 0) {
            if (coeff_vect.at(i) != 0 or 
                    coeff_vect.size() == 1) {
                
                res << coeff_vect.at(i);

                if (i + 1 < coeff_vect.size()) {
                    res << " + ";
                }
            }

        } else {
            if (coeff_vect.at(i) == 0) {
                continue;
            }

            if (coeff_vect.at(i) != 1) {
                res << coeff_vect.at(i) << "*";
            }

            res << "x";

            if (i > 1) {
                res << "^" << i;
            }

            if (i + 1 < coeff_vect.size()) {
                res << " + ";
            }
        }
    }

    return res.str();
}

double getPolyVal(const VectOfDouble& coeff_vect, double x)
{
    double res = 0;

    // Compute function value in for loop.
    for (unsigned i = 0; i < coeff_vect.size(); i++) {
        switch (i) {
          case 0: {
            res += coeff_vect[i];
            break;
          }
          case 1: {
            res += coeff_vect[i] * x;
            break;
          }
          default: {
            res += coeff_vect[i] * pow(x, i);
            break;
          }
        }
    }

    return res;
}

double getPolyDeriv(const VectOfDouble& coeff_vect, double x)
{
    double res = 0;

    // Compute derivative value in for loop.
    for (unsigned i = 0; i < coeff_vect.size(); i++) {
        if (i > 0) {
            res += coeff_vect[i] * i * pow(x, i - 1);
        }
    }

    return res;
}

double getMulDeriv(double df, double dg, double f, double g)
{
    return df * g + f * dg;
}

double getDivDeriv(double df, double dg, double f, double g)
{
    return (df * g - f * dg) / (g * g);
}


// Idential function tests.
TEST(TestIdent, StringRepr)
{
    TFactory func_factory;
    auto f = func_factory.createObject("ident");

    ASSERT_STREQ("f(x) = x", f->toString().c_str());
}

TEST(TestIdent, Val)
{
    TFactory func_factory;
    auto f = func_factory.createObject("ident");

    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(rand_x, (*f)(rand_x));
    }
}

TEST(TestIdent, Deriv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("ident");

    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(1, f->getDeriv(rand_x));
    }
}

TEST(TestIdent, EqSolv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("ident");

    auto eq_root = EqSolver().solveEquation(*f);

    ASSERT_TRUE(eq_root.has_value());
    ASSERT_NEAR(0, eq_root.value(), EPS);
}


// Constant function tests.
TEST(TestConst, StringRepr)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    auto f = func_factory.createObject("const", rand_const);

    std::stringstream ss;
    ss << "f(x) = " << rand_const;

    ASSERT_STREQ(ss.str().c_str(), f->toString().c_str());
}

TEST(TestConst, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    auto f = func_factory.createObject("const", rand_const);

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(rand_const, (*f)(rand_x));
    }
}

TEST(TestConst, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    auto f = func_factory.createObject("const", rand_const);

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(0, f->getDeriv(rand_x));
    }
}

TEST(TestConst, EqSolv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("const", 0);
    auto eq_root = EqSolver().solveEquation(*f);
    ASSERT_TRUE(eq_root.has_value());

    auto g = func_factory.createObject("const", 1);
    eq_root = EqSolver().solveEquation(*g);
    ASSERT_FALSE(eq_root.has_value());
}


// Power function tests.
TEST(TestPower, StringRepr)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) { 
        double rand_exp = std::rand() % MAXRAND_EXP + 1;
        auto f = func_factory.createObject("power", rand_exp);

        std::stringstream ss;
        ss << "f(x) = x";
        if (rand_exp > 1) {
            ss << "^" << rand_exp;
        }

        ASSERT_STREQ(ss.str().c_str(), f->toString().c_str());
    }
}

TEST(TestPower, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_exp = std::rand() % MAXRAND_EXP + 1;
        auto f = func_factory.createObject("power", rand_exp);
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp), (*f)(rand_x));
    }
}

TEST(TestPower, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_exp = std::rand() % MAXRAND_EXP + 1;
        auto f = func_factory.createObject("power", rand_exp);
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1),
                         f->getDeriv(rand_x));
    }
}

TEST(TestPower, EqSolv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_exp = std::rand() % MAXRAND_EXP + 1;
        auto f = func_factory.createObject("power", rand_exp);

        auto eq_root = EqSolver().solveEquation(*f);

        ASSERT_TRUE(eq_root.has_value());
        ASSERT_NEAR(0, eq_root.value(), EPS);
    }
}


// Exponential function tests.
TEST(TestExp, StringRepr)
{
    TFactory func_factory;
    auto f = func_factory.createObject("exp");

    ASSERT_STREQ("f(x) = exp(x)", f->toString().c_str());
}

TEST(TestExp, Val)
{
    TFactory func_factory;
    auto f = func_factory.createObject("exp");

    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(exp(rand_x), (*f)(rand_x));
    }
}

TEST(TestExp, Deriv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("exp");

    std::srand(static_cast<unsigned int>(time(0)));

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(exp(rand_x), f->getDeriv(rand_x));
    }
}

TEST(TestExp, EqSolv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("exp");
    auto eq_root = EqSolver(2, 1, 0.000001).solveEquation(*f);

    ASSERT_FALSE(eq_root.has_value());
}


// Polynomial function tests.
TEST(TestPoly, StringRepr)
{
    TFactory func_factory;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        auto rand_coeffs = genPolyCoeffs();
        auto f = func_factory.createObject("polynomial", rand_coeffs);

        ASSERT_STREQ(getPolyStrRepr(rand_coeffs).c_str(),
                     f->toString().c_str());
    }
}

TEST(TestPoly, Val)
{
    TFactory func_factory;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        auto rand_coeffs = genPolyCoeffs();
        auto f = func_factory.createObject("polynomial", rand_coeffs);
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x), (*f)(rand_x));
    }
}

TEST(TestPoly, Deriv)
{
    TFactory func_factory;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        auto rand_coeffs = genPolyCoeffs();
        auto f = func_factory.createObject("polynomial", rand_coeffs);
        double rand_x = std::rand() % MAXRAND;

        if (i >= ITER_NUM / 2) {
            rand_x = -rand_x;
        }

        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x),
                         f->getDeriv(rand_x));
    }
}

TEST(TestPoly, EqSolv)
{
    TFactory func_factory;
    auto f = func_factory.createObject("polynomial", { 1, -2, 1 });
    auto eq_root = EqSolver().solveEquation(*f);

    ASSERT_TRUE(eq_root.has_value());
    ASSERT_NEAR(1, eq_root.value(), EPS);
}


// Addition tests.
TEST(TestAdd, ExcNotThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_NO_THROW(*f1 + *f1);
    ASSERT_NO_THROW(*f1 + *f2);
    ASSERT_NO_THROW(*f1 + *f3);
    ASSERT_NO_THROW(*f1 + *f4);
    ASSERT_NO_THROW(*f1 + *f5);

    ASSERT_NO_THROW(*f2 + *f1);
    ASSERT_NO_THROW(*f2 + *f2);
    ASSERT_NO_THROW(*f2 + *f3);
    ASSERT_NO_THROW(*f2 + *f4);
    ASSERT_NO_THROW(*f2 + *f5);

    ASSERT_NO_THROW(*f3 + *f1);
    ASSERT_NO_THROW(*f3 + *f2);
    ASSERT_NO_THROW(*f3 + *f3);
    ASSERT_NO_THROW(*f3 + *f4);
    ASSERT_NO_THROW(*f3 + *f5);

    ASSERT_NO_THROW(*f4 + *f1);
    ASSERT_NO_THROW(*f4 + *f2);
    ASSERT_NO_THROW(*f4 + *f3);
    ASSERT_NO_THROW(*f4 + *f4);
    ASSERT_NO_THROW(*f4 + *f5);

    ASSERT_NO_THROW(*f5 + *f1);
    ASSERT_NO_THROW(*f5 + *f2);
    ASSERT_NO_THROW(*f5 + *f3);
    ASSERT_NO_THROW(*f5 + *f4);
    ASSERT_NO_THROW(*f5 + *f5);
}

TEST(TestAdd, ExcThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_THROW(*f1 + 1, std::logic_error);
    ASSERT_THROW(*f1 + 1.0, std::logic_error);
    ASSERT_THROW(*f1 + "", std::logic_error);
    ASSERT_THROW(*f1 + true, std::logic_error);
    ASSERT_THROW(*f1 + std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f2 + 1, std::logic_error);
    ASSERT_THROW(*f2 + 1.0, std::logic_error);
    ASSERT_THROW(*f2 + "", std::logic_error);
    ASSERT_THROW(*f2 + true, std::logic_error);
    ASSERT_THROW(*f2 + std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f3 + 1, std::logic_error);
    ASSERT_THROW(*f3 + 1.0, std::logic_error);
    ASSERT_THROW(*f3 + "", std::logic_error);
    ASSERT_THROW(*f3 + true, std::logic_error);
    ASSERT_THROW(*f3 + std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f4 + 1, std::logic_error);
    ASSERT_THROW(*f4 + 1.0, std::logic_error);
    ASSERT_THROW(*f4 + "", std::logic_error);
    ASSERT_THROW(*f4 + true, std::logic_error);
    ASSERT_THROW(*f4 + std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f5 + 1, std::logic_error);
    ASSERT_THROW(*f5 + 1.0, std::logic_error);
    ASSERT_THROW(*f5 + "", std::logic_error);
    ASSERT_THROW(*f5 + true, std::logic_error);
    ASSERT_THROW(*f5 + std::make_unique<char>(), std::logic_error);
}

TEST(TestAdd, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 + *f1;
    auto f12 = *f1 + *f2;
    auto f13 = *f1 + *f3;
    auto f14 = *f1 + *f4;
    auto f15 = *f1 + *f5;

    auto f21 = *f2 + *f1;
    auto f22 = *f2 + *f2;
    auto f23 = *f2 + *f3;
    auto f24 = *f2 + *f4;
    auto f25 = *f2 + *f5;

    auto f31 = *f3 + *f1;
    auto f32 = *f3 + *f2;
    auto f33 = *f3 + *f3;
    auto f34 = *f3 + *f4;
    auto f35 = *f3 + *f5;

    auto f41 = *f4 + *f1;
    auto f42 = *f4 + *f2;
    auto f43 = *f4 + *f3;
    auto f44 = *f4 + *f4;
    auto f45 = *f4 + *f5;

    auto f51 = *f5 + *f1;
    auto f52 = *f5 + *f2;
    auto f53 = *f5 + *f3;
    auto f54 = *f5 + *f4;
    auto f55 = *f5 + *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(rand_x + rand_x, (*f11)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x + rand_const, (*f12)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x + pow(rand_x, rand_exp), (*f13)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x + exp(rand_x), (*f14)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x + getPolyVal(rand_coeffs, rand_x),
                         (*f15)(rand_x));

        ASSERT_DOUBLE_EQ(rand_const + rand_x, (*f21)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const + rand_const, (*f22)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const + pow(rand_x, rand_exp), (*f23)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const + exp(rand_x), (*f24)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const + getPolyVal(rand_coeffs, rand_x),
                         (*f25)(rand_x));

        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) + rand_x, (*f31)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) + rand_const, (*f32)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) + pow(rand_x, rand_exp),
                         (*f33)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) + exp(rand_x), (*f34)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) +
                         getPolyVal(rand_coeffs, rand_x),
                         (*f35)(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) + rand_x, (*f41)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + rand_const, (*f42)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + pow(rand_x, rand_exp), (*f43)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + exp(rand_x), (*f44)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + getPolyVal(rand_coeffs, rand_x),
                         (*f45)(rand_x));

        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) + rand_x,
                         (*f51)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) + rand_const,
                         (*f52)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) +
                         pow(rand_x, rand_exp), (*f53)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) + exp(rand_x),
                         (*f54)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) +
                         getPolyVal(rand_coeffs, rand_x),
                         (*f55)(rand_x));
    }
}

TEST(TestAdd, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 + *f1;
    auto f12 = *f1 + *f2;
    auto f13 = *f1 + *f3;
    auto f14 = *f1 + *f4;
    auto f15 = *f1 + *f5;

    auto f21 = *f2 + *f1;
    auto f22 = *f2 + *f2;
    auto f23 = *f2 + *f3;
    auto f24 = *f2 + *f4;
    auto f25 = *f2 + *f5;

    auto f31 = *f3 + *f1;
    auto f32 = *f3 + *f2;
    auto f33 = *f3 + *f3;
    auto f34 = *f3 + *f4;
    auto f35 = *f3 + *f5;

    auto f41 = *f4 + *f1;
    auto f42 = *f4 + *f2;
    auto f43 = *f4 + *f3;
    auto f44 = *f4 + *f4;
    auto f45 = *f4 + *f5;

    auto f51 = *f5 + *f1;
    auto f52 = *f5 + *f2;
    auto f53 = *f5 + *f3;
    auto f54 = *f5 + *f4;
    auto f55 = *f5 + *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(1 + 1, f11->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 + 0, f12->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 + rand_exp * pow(rand_x, rand_exp - 1),
                         f13->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 + exp(rand_x), f14->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 + getPolyDeriv(rand_coeffs, rand_x),
                         f15->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(0 + 1, f21->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 + 0, f22->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 + rand_exp * pow(rand_x, rand_exp - 1),
                         f23->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 + exp(rand_x), f24->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 + getPolyDeriv(rand_coeffs, rand_x),
                         f25->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) + 1,
                         f31->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) + 0,
                         f32->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) +
                         rand_exp * pow(rand_x, rand_exp - 1),
                         f33->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) + exp(rand_x),
                         f34->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) +
                         getPolyDeriv(rand_coeffs, rand_x),
                         f35->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) + 1, f41->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + 0, f42->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + rand_exp * pow(rand_x, rand_exp - 1),
                         f43->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + exp(rand_x), f44->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) + getPolyDeriv(rand_coeffs, rand_x),
                         f45->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) + 1,
                         f51->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) + 0,
                         f52->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) +
                         rand_exp * pow(rand_x, rand_exp - 1),
                         f53->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) + exp(rand_x),
                         f54->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) +
                         getPolyDeriv(rand_coeffs, rand_x),
                         f55->getDeriv(rand_x));
    }
}


// Subtraction tests.
TEST(TestSub, ExcNotThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_NO_THROW(*f1 - *f1);
    ASSERT_NO_THROW(*f1 - *f2);
    ASSERT_NO_THROW(*f1 - *f3);
    ASSERT_NO_THROW(*f1 - *f4);
    ASSERT_NO_THROW(*f1 - *f5);

    ASSERT_NO_THROW(*f2 - *f1);
    ASSERT_NO_THROW(*f2 - *f2);
    ASSERT_NO_THROW(*f2 - *f3);
    ASSERT_NO_THROW(*f2 - *f4);
    ASSERT_NO_THROW(*f2 - *f5);

    ASSERT_NO_THROW(*f3 - *f1);
    ASSERT_NO_THROW(*f3 - *f2);
    ASSERT_NO_THROW(*f3 - *f3);
    ASSERT_NO_THROW(*f3 - *f4);
    ASSERT_NO_THROW(*f3 - *f5);

    ASSERT_NO_THROW(*f4 - *f1);
    ASSERT_NO_THROW(*f4 - *f2);
    ASSERT_NO_THROW(*f4 - *f3);
    ASSERT_NO_THROW(*f4 - *f4);
    ASSERT_NO_THROW(*f4 - *f5);

    ASSERT_NO_THROW(*f5 - *f1);
    ASSERT_NO_THROW(*f5 - *f2);
    ASSERT_NO_THROW(*f5 - *f3);
    ASSERT_NO_THROW(*f5 - *f4);
    ASSERT_NO_THROW(*f5 - *f5);
}

TEST(TestSub, ExcThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_THROW(*f1 - 1, std::logic_error);
    ASSERT_THROW(*f1 - 1.0, std::logic_error);
    ASSERT_THROW(*f1 - "", std::logic_error);
    ASSERT_THROW(*f1 - true, std::logic_error);
    ASSERT_THROW(*f1 - std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f2 - 1, std::logic_error);
    ASSERT_THROW(*f2 - 1.0, std::logic_error);
    ASSERT_THROW(*f2 - "", std::logic_error);
    ASSERT_THROW(*f2 - true, std::logic_error);
    ASSERT_THROW(*f2 - std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f3 - 1, std::logic_error);
    ASSERT_THROW(*f3 - 1.0, std::logic_error);
    ASSERT_THROW(*f3 - "", std::logic_error);
    ASSERT_THROW(*f3 - true, std::logic_error);
    ASSERT_THROW(*f3 - std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f4 - 1, std::logic_error);
    ASSERT_THROW(*f4 - 1.0, std::logic_error);
    ASSERT_THROW(*f4 - "", std::logic_error);
    ASSERT_THROW(*f4 - true, std::logic_error);
    ASSERT_THROW(*f4 - std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f5 - 1, std::logic_error);
    ASSERT_THROW(*f5 - 1.0, std::logic_error);
    ASSERT_THROW(*f5 - "", std::logic_error);
    ASSERT_THROW(*f5 - true, std::logic_error);
    ASSERT_THROW(*f5 - std::make_unique<char>(), std::logic_error);
}

TEST(TestSub, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 - *f1;
    auto f12 = *f1 - *f2;
    auto f13 = *f1 - *f3;
    auto f14 = *f1 - *f4;
    auto f15 = *f1 - *f5;

    auto f21 = *f2 - *f1;
    auto f22 = *f2 - *f2;
    auto f23 = *f2 - *f3;
    auto f24 = *f2 - *f4;
    auto f25 = *f2 - *f5;

    auto f31 = *f3 - *f1;
    auto f32 = *f3 - *f2;
    auto f33 = *f3 - *f3;
    auto f34 = *f3 - *f4;
    auto f35 = *f3 - *f5;

    auto f41 = *f4 - *f1;
    auto f42 = *f4 - *f2;
    auto f43 = *f4 - *f3;
    auto f44 = *f4 - *f4;
    auto f45 = *f4 - *f5;

    auto f51 = *f5 - *f1;
    auto f52 = *f5 - *f2;
    auto f53 = *f5 - *f3;
    auto f54 = *f5 - *f4;
    auto f55 = *f5 - *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(rand_x - rand_x, (*f11)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x - rand_const, (*f12)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x - pow(rand_x, rand_exp), (*f13)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x - exp(rand_x), (*f14)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x - getPolyVal(rand_coeffs, rand_x),
                         (*f15)(rand_x));

        ASSERT_DOUBLE_EQ(rand_const - rand_x, (*f21)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const - rand_const, (*f22)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const - pow(rand_x, rand_exp), (*f23)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const - exp(rand_x), (*f24)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const - getPolyVal(rand_coeffs, rand_x),
                         (*f25)(rand_x));

        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) - rand_x, (*f31)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) - rand_const, (*f32)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) - pow(rand_x, rand_exp),
                         (*f33)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) - exp(rand_x), (*f34)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) -
                         getPolyVal(rand_coeffs, rand_x),
                         (*f35)(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) - rand_x, (*f41)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - rand_const, (*f42)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - pow(rand_x, rand_exp), (*f43)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - exp(rand_x), (*f44)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - getPolyVal(rand_coeffs, rand_x),
                         (*f45)(rand_x));

        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) - rand_x,
                         (*f51)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) - rand_const,
                         (*f52)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) -
                         pow(rand_x, rand_exp), (*f53)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) - exp(rand_x),
                         (*f54)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) -
                         getPolyVal(rand_coeffs, rand_x),
                         (*f55)(rand_x));
    }
}

TEST(TestSub, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 - *f1;
    auto f12 = *f1 - *f2;
    auto f13 = *f1 - *f3;
    auto f14 = *f1 - *f4;
    auto f15 = *f1 - *f5;

    auto f21 = *f2 - *f1;
    auto f22 = *f2 - *f2;
    auto f23 = *f2 - *f3;
    auto f24 = *f2 - *f4;
    auto f25 = *f2 - *f5;

    auto f31 = *f3 - *f1;
    auto f32 = *f3 - *f2;
    auto f33 = *f3 - *f3;
    auto f34 = *f3 - *f4;
    auto f35 = *f3 - *f5;

    auto f41 = *f4 - *f1;
    auto f42 = *f4 - *f2;
    auto f43 = *f4 - *f3;
    auto f44 = *f4 - *f4;
    auto f45 = *f4 - *f5;

    auto f51 = *f5 - *f1;
    auto f52 = *f5 - *f2;
    auto f53 = *f5 - *f3;
    auto f54 = *f5 - *f4;
    auto f55 = *f5 - *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(1 - 1, f11->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 - 0, f12->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 - rand_exp * pow(rand_x, rand_exp - 1),
                         f13->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 - exp(rand_x), f14->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(1 - getPolyDeriv(rand_coeffs, rand_x),
                         f15->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(0 - 1, f21->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 - 0, f22->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 - rand_exp * pow(rand_x, rand_exp - 1),
                         f23->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 - exp(rand_x), f24->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(0 - getPolyDeriv(rand_coeffs, rand_x),
                         f25->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) - 1,
                         f31->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) - 0,
                         f32->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) -
                         rand_exp * pow(rand_x, rand_exp - 1),
                         f33->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) - exp(rand_x),
                         f34->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(rand_exp * pow(rand_x, rand_exp - 1) -
                         getPolyDeriv(rand_coeffs, rand_x),
                         f35->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) - 1, f41->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - 0, f42->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - rand_exp * pow(rand_x, rand_exp - 1),
                         f43->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - exp(rand_x), f44->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) - getPolyDeriv(rand_coeffs, rand_x),
                         f45->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) - 1,
                         f51->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) - 0,
                         f52->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) -
                         rand_exp * pow(rand_x, rand_exp - 1),
                         f53->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) - exp(rand_x),
                         f54->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getPolyDeriv(rand_coeffs, rand_x) -
                         getPolyDeriv(rand_coeffs, rand_x),
                         f55->getDeriv(rand_x));
    }
}


// Multiplication tests.
TEST(TestMul, ExcNotThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_NO_THROW((*f1) * (*f1));
    ASSERT_NO_THROW((*f1) * (*f2));
    ASSERT_NO_THROW((*f1) * (*f3));
    ASSERT_NO_THROW((*f1) * (*f4));
    ASSERT_NO_THROW((*f1) * (*f5));

    ASSERT_NO_THROW((*f2) * (*f1));
    ASSERT_NO_THROW((*f2) * (*f2));
    ASSERT_NO_THROW((*f2) * (*f3));
    ASSERT_NO_THROW((*f2) * (*f4));
    ASSERT_NO_THROW((*f2) * (*f5));

    ASSERT_NO_THROW((*f3) * (*f1));
    ASSERT_NO_THROW((*f3) * (*f2));
    ASSERT_NO_THROW((*f3) * (*f3));
    ASSERT_NO_THROW((*f3) * (*f4));
    ASSERT_NO_THROW((*f3) * (*f5));

    ASSERT_NO_THROW((*f4) * (*f1));
    ASSERT_NO_THROW((*f4) * (*f2));
    ASSERT_NO_THROW((*f4) * (*f3));
    ASSERT_NO_THROW((*f4) * (*f4));
    ASSERT_NO_THROW((*f4) * (*f5));

    ASSERT_NO_THROW((*f5) * (*f1));
    ASSERT_NO_THROW((*f5) * (*f2));
    ASSERT_NO_THROW((*f5) * (*f3));
    ASSERT_NO_THROW((*f5) * (*f4));
    ASSERT_NO_THROW((*f5) * (*f5));
}

TEST(TestMul, ExcThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_THROW((*f1) * 1, std::logic_error);
    ASSERT_THROW((*f1) * 1.0, std::logic_error);
    ASSERT_THROW((*f1) * "", std::logic_error);
    ASSERT_THROW((*f1) * true, std::logic_error);
    ASSERT_THROW((*f1) * std::make_unique<char>(), std::logic_error);

    ASSERT_THROW((*f2) * 1, std::logic_error);
    ASSERT_THROW((*f2) * 1.0, std::logic_error);
    ASSERT_THROW((*f2) * "", std::logic_error);
    ASSERT_THROW((*f2) * true, std::logic_error);
    ASSERT_THROW((*f2) * std::make_unique<char>(), std::logic_error);

    ASSERT_THROW((*f3) * 1, std::logic_error);
    ASSERT_THROW((*f3) * 1.0, std::logic_error);
    ASSERT_THROW((*f3) * "", std::logic_error);
    ASSERT_THROW((*f3) * true, std::logic_error);
    ASSERT_THROW((*f3) * std::make_unique<char>(), std::logic_error);

    ASSERT_THROW((*f4) * 1, std::logic_error);
    ASSERT_THROW((*f4) * 1.0, std::logic_error);
    ASSERT_THROW((*f4) * "", std::logic_error);
    ASSERT_THROW((*f4) * true, std::logic_error);
    ASSERT_THROW((*f4) * std::make_unique<char>(), std::logic_error);

    ASSERT_THROW((*f5) * 1, std::logic_error);
    ASSERT_THROW((*f5) * 1.0, std::logic_error);
    ASSERT_THROW((*f5) * "", std::logic_error);
    ASSERT_THROW((*f5) * true, std::logic_error);
    ASSERT_THROW((*f5) * std::make_unique<char>(), std::logic_error);
}

TEST(TestMul, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = (*f1) * (*f1);
    auto f12 = (*f1) * (*f2);
    auto f13 = (*f1) * (*f3);
    auto f14 = (*f1) * (*f4);
    auto f15 = (*f1) * (*f5);

    auto f21 = (*f2) * (*f1);
    auto f22 = (*f2) * (*f2);
    auto f23 = (*f2) * (*f3);
    auto f24 = (*f2) * (*f4);
    auto f25 = (*f2) * (*f5);

    auto f31 = (*f3) * (*f1);
    auto f32 = (*f3) * (*f2);
    auto f33 = (*f3) * (*f3);
    auto f34 = (*f3) * (*f4);
    auto f35 = (*f3) * (*f5);

    auto f41 = (*f4) * (*f1);
    auto f42 = (*f4) * (*f2);
    auto f43 = (*f4) * (*f3);
    auto f44 = (*f4) * (*f4);
    auto f45 = (*f4) * (*f5);

    auto f51 = (*f5) * (*f1);
    auto f52 = (*f5) * (*f2);
    auto f53 = (*f5) * (*f3);
    auto f54 = (*f5) * (*f4);
    auto f55 = (*f5) * (*f5);

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(rand_x * rand_x, (*f11)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x * rand_const, (*f12)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x * pow(rand_x, rand_exp), (*f13)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x * exp(rand_x), (*f14)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x * getPolyVal(rand_coeffs, rand_x),
                         (*f15)(rand_x));

        ASSERT_DOUBLE_EQ(rand_const * rand_x, (*f21)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const * rand_const, (*f22)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const * pow(rand_x, rand_exp), (*f23)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const * exp(rand_x), (*f24)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const * getPolyVal(rand_coeffs, rand_x),
                         (*f25)(rand_x));

        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) * rand_x, (*f31)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) * rand_const, (*f32)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) * pow(rand_x, rand_exp),
                         (*f33)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) * exp(rand_x), (*f34)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) *
                         getPolyVal(rand_coeffs, rand_x),
                         (*f35)(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) * rand_x, (*f41)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) * rand_const, (*f42)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) * pow(rand_x, rand_exp), (*f43)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) * exp(rand_x), (*f44)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) * getPolyVal(rand_coeffs, rand_x),
                         (*f45)(rand_x));

        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) * rand_x,
                         (*f51)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) * rand_const,
                         (*f52)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) *
                         pow(rand_x, rand_exp), (*f53)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) * exp(rand_x),
                         (*f54)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) *
                         getPolyVal(rand_coeffs, rand_x),
                         (*f55)(rand_x));
    }
}

TEST(TestMul, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = (*f1) * (*f1);
    auto f12 = (*f1) * (*f2);
    auto f13 = (*f1) * (*f3);
    auto f14 = (*f1) * (*f4);
    auto f15 = (*f1) * (*f5);

    auto f21 = (*f2) * (*f1);
    auto f22 = (*f2) * (*f2);
    auto f23 = (*f2) * (*f3);
    auto f24 = (*f2) * (*f4);
    auto f25 = (*f2) * (*f5);

    auto f31 = (*f3) * (*f1);
    auto f32 = (*f3) * (*f2);
    auto f33 = (*f3) * (*f3);
    auto f34 = (*f3) * (*f4);
    auto f35 = (*f3) * (*f5);

    auto f41 = (*f4) * (*f1);
    auto f42 = (*f4) * (*f2);
    auto f43 = (*f4) * (*f3);
    auto f44 = (*f4) * (*f4);
    auto f45 = (*f4) * (*f5);

    auto f51 = (*f5) * (*f1);
    auto f52 = (*f5) * (*f2);
    auto f53 = (*f5) * (*f3);
    auto f54 = (*f5) * (*f4);
    auto f55 = (*f5) * (*f5);

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(getMulDeriv(1, 1, rand_x, rand_x),
                         f11->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(1, 0, rand_x, rand_const),
                         f12->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(1, rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_x, pow(rand_x, rand_exp)),
                         f13->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(1, exp(rand_x), rand_x, exp(rand_x)),
                         f14->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(1, getPolyDeriv(rand_coeffs, rand_x),
                                     rand_x, getPolyVal(rand_coeffs, rand_x)),
                         f15->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getMulDeriv(0, 1, rand_const, rand_x),
                         f21->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(0, 0, rand_const, rand_const),
                         f22->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(0, rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_const, pow(rand_x, rand_exp)),
                         f23->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(0, exp(rand_x), rand_const, exp(rand_x)),
                         f24->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(0,
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     rand_const,
                                     getPolyVal(rand_coeffs, rand_x)),
                         f25->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getMulDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     1,
                                     pow(rand_x, rand_exp),
                                     rand_x),
                         f31->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     0,
                                     pow(rand_x, rand_exp),
                                     rand_const),
                         f32->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     pow(rand_x, rand_exp),
                                     pow(rand_x, rand_exp)),
                         f33->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     exp(rand_x),
                                     pow(rand_x, rand_exp),
                                     exp(rand_x)),
                         f34->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     pow(rand_x, rand_exp),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f35->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getMulDeriv(exp(rand_x), 1, exp(rand_x), rand_x),
                         f41->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(exp(rand_x), 0, exp(rand_x), rand_const),
                         f42->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(exp(rand_x),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     exp(rand_x),
                                     pow(rand_x, rand_exp)),
                         f43->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(exp(rand_x),
                                     exp(rand_x),
                                     exp(rand_x),
                                     exp(rand_x)),
                         f44->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(exp(rand_x),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     exp(rand_x),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f45->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getMulDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     1,
                                     getPolyVal(rand_coeffs, rand_x),
                                     rand_x),
                         f51->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     0,
                                     getPolyVal(rand_coeffs, rand_x),
                                     rand_const),
                         f52->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     getPolyVal(rand_coeffs, rand_x),
                                     pow(rand_x, rand_exp)),
                         f53->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     exp(rand_x),
                                     getPolyVal(rand_coeffs, rand_x),
                                     exp(rand_x)),
                         f54->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getMulDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     getPolyVal(rand_coeffs, rand_x),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f55->getDeriv(rand_x));
    }
}


// Division tests.
TEST(TestDiv, ExcNotThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_NO_THROW(*f1 / *f1);
    ASSERT_NO_THROW(*f1 / *f2);
    ASSERT_NO_THROW(*f1 / *f3);
    ASSERT_NO_THROW(*f1 / *f4);
    ASSERT_NO_THROW(*f1 / *f5);

    ASSERT_NO_THROW(*f2 / *f1);
    ASSERT_NO_THROW(*f2 / *f2);
    ASSERT_NO_THROW(*f2 / *f3);
    ASSERT_NO_THROW(*f2 / *f4);
    ASSERT_NO_THROW(*f2 / *f5);

    ASSERT_NO_THROW(*f3 / *f1);
    ASSERT_NO_THROW(*f3 / *f2);
    ASSERT_NO_THROW(*f3 / *f3);
    ASSERT_NO_THROW(*f3 / *f4);
    ASSERT_NO_THROW(*f3 / *f5);

    ASSERT_NO_THROW(*f4 / *f1);
    ASSERT_NO_THROW(*f4 / *f2);
    ASSERT_NO_THROW(*f4 / *f3);
    ASSERT_NO_THROW(*f4 / *f4);
    ASSERT_NO_THROW(*f4 / *f5);

    ASSERT_NO_THROW(*f5 / *f1);
    ASSERT_NO_THROW(*f5 / *f2);
    ASSERT_NO_THROW(*f5 / *f3);
    ASSERT_NO_THROW(*f5 / *f4);
    ASSERT_NO_THROW(*f5 / *f5);
}

TEST(TestDiv, ExcThrown)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    ASSERT_THROW(*f1 / 1, std::logic_error);
    ASSERT_THROW(*f1 / 1.0, std::logic_error);
    ASSERT_THROW(*f1 / "", std::logic_error);
    ASSERT_THROW(*f1 / true, std::logic_error);
    ASSERT_THROW(*f1 / std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f2 / 1, std::logic_error);
    ASSERT_THROW(*f2 / 1.0, std::logic_error);
    ASSERT_THROW(*f2 / "", std::logic_error);
    ASSERT_THROW(*f2 / true, std::logic_error);
    ASSERT_THROW(*f2 / std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f3 / 1, std::logic_error);
    ASSERT_THROW(*f3 / 1.0, std::logic_error);
    ASSERT_THROW(*f3 / "", std::logic_error);
    ASSERT_THROW(*f3 / true, std::logic_error);
    ASSERT_THROW(*f3 / std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f4 / 1, std::logic_error);
    ASSERT_THROW(*f4 / 1.0, std::logic_error);
    ASSERT_THROW(*f4 / "", std::logic_error);
    ASSERT_THROW(*f4 / true, std::logic_error);
    ASSERT_THROW(*f4 / std::make_unique<char>(), std::logic_error);

    ASSERT_THROW(*f5 / 1, std::logic_error);
    ASSERT_THROW(*f5 / 1.0, std::logic_error);
    ASSERT_THROW(*f5 / "", std::logic_error);
    ASSERT_THROW(*f5 / true, std::logic_error);
    ASSERT_THROW(*f5 / std::make_unique<char>(), std::logic_error);
}

TEST(TestDiv, Val)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 / *f1;
    auto f12 = *f1 / *f2;
    auto f13 = *f1 / *f3;
    auto f14 = *f1 / *f4;
    auto f15 = *f1 / *f5;

    auto f21 = *f2 / *f1;
    auto f22 = *f2 / *f2;
    auto f23 = *f2 / *f3;
    auto f24 = *f2 / *f4;
    auto f25 = *f2 / *f5;

    auto f31 = *f3 / *f1;
    auto f32 = *f3 / *f2;
    auto f33 = *f3 / *f3;
    auto f34 = *f3 / *f4;
    auto f35 = *f3 / *f5;

    auto f41 = *f4 / *f1;
    auto f42 = *f4 / *f2;
    auto f43 = *f4 / *f3;
    auto f44 = *f4 / *f4;
    auto f45 = *f4 / *f5;

    auto f51 = *f5 / *f1;
    auto f52 = *f5 / *f2;
    auto f53 = *f5 / *f3;
    auto f54 = *f5 / *f4;
    auto f55 = *f5 / *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(rand_x / rand_x, (*f11)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x / rand_const, (*f12)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x / pow(rand_x, rand_exp), (*f13)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x / exp(rand_x), (*f14)(rand_x));
        ASSERT_DOUBLE_EQ(rand_x / getPolyVal(rand_coeffs, rand_x),
                         (*f15)(rand_x));

        ASSERT_DOUBLE_EQ(rand_const / rand_x, (*f21)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const / rand_const, (*f22)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const / pow(rand_x, rand_exp), (*f23)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const / exp(rand_x), (*f24)(rand_x));
        ASSERT_DOUBLE_EQ(rand_const / getPolyVal(rand_coeffs, rand_x),
                         (*f25)(rand_x));

        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) / rand_x, (*f31)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) / rand_const, (*f32)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) / pow(rand_x, rand_exp),
                         (*f33)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) / exp(rand_x), (*f34)(rand_x));
        ASSERT_DOUBLE_EQ(pow(rand_x, rand_exp) /
                         getPolyVal(rand_coeffs, rand_x),
                         (*f35)(rand_x));

        ASSERT_DOUBLE_EQ(exp(rand_x) / rand_x, (*f41)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) / rand_const, (*f42)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) / pow(rand_x, rand_exp), (*f43)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) / exp(rand_x), (*f44)(rand_x));
        ASSERT_DOUBLE_EQ(exp(rand_x) / getPolyVal(rand_coeffs, rand_x),
                         (*f45)(rand_x));

        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) / rand_x,
                         (*f51)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) / rand_const,
                         (*f52)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) /
                         pow(rand_x, rand_exp), (*f53)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) / exp(rand_x),
                         (*f54)(rand_x));
        ASSERT_DOUBLE_EQ(getPolyVal(rand_coeffs, rand_x) /
                         getPolyVal(rand_coeffs, rand_x),
                         (*f55)(rand_x));
    }
}

TEST(TestDiv, Deriv)
{
    TFactory func_factory;
    std::srand(static_cast<unsigned int>(time(0)));
    double rand_const = std::rand() % MAXRAND;
    double rand_exp = std::rand() % MAXRAND_EXP + 1;
    auto rand_coeffs = genPolyCoeffs();

    auto f1 = func_factory.createObject("ident");
    auto f2 = func_factory.createObject("const", rand_const);
    auto f3 = func_factory.createObject("power", rand_exp);
    auto f4 = func_factory.createObject("exp");
    auto f5 = func_factory.createObject("polynomial", rand_coeffs);

    auto f11 = *f1 / *f1;
    auto f12 = *f1 / *f2;
    auto f13 = *f1 / *f3;
    auto f14 = *f1 / *f4;
    auto f15 = *f1 / *f5;

    auto f21 = *f2 / *f1;
    auto f22 = *f2 / *f2;
    auto f23 = *f2 / *f3;
    auto f24 = *f2 / *f4;
    auto f25 = *f2 / *f5;

    auto f31 = *f3 / *f1;
    auto f32 = *f3 / *f2;
    auto f33 = *f3 / *f3;
    auto f34 = *f3 / *f4;
    auto f35 = *f3 / *f5;

    auto f41 = *f4 / *f1;
    auto f42 = *f4 / *f2;
    auto f43 = *f4 / *f3;
    auto f44 = *f4 / *f4;
    auto f45 = *f4 / *f5;

    auto f51 = *f5 / *f1;
    auto f52 = *f5 / *f2;
    auto f53 = *f5 / *f3;
    auto f54 = *f5 / *f4;
    auto f55 = *f5 / *f5;

    for (unsigned i = 0; i < ITER_NUM; i++) {
        double rand_x = std::rand() % MAXRAND;

        ASSERT_DOUBLE_EQ(getDivDeriv(1, 1, rand_x, rand_x),
                         f11->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(1, 0, rand_x, rand_const),
                         f12->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(1, rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_x, pow(rand_x, rand_exp)),
                         f13->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(1, exp(rand_x), rand_x, exp(rand_x)),
                         f14->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(1, getPolyDeriv(rand_coeffs, rand_x),
                                     rand_x, getPolyVal(rand_coeffs, rand_x)),
                         f15->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getDivDeriv(0, 1, rand_const, rand_x),
                         f21->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(0, 0, rand_const, rand_const),
                         f22->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(0, rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_const, pow(rand_x, rand_exp)),
                         f23->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(0, exp(rand_x), rand_const, exp(rand_x)),
                         f24->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(0,
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     rand_const,
                                     getPolyVal(rand_coeffs, rand_x)),
                         f25->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getDivDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     1,
                                     pow(rand_x, rand_exp),
                                     rand_x),
                         f31->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     0,
                                     pow(rand_x, rand_exp),
                                     rand_const),
                         f32->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     pow(rand_x, rand_exp),
                                     pow(rand_x, rand_exp)),
                         f33->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     exp(rand_x),
                                     pow(rand_x, rand_exp),
                                     exp(rand_x)),
                         f34->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(rand_exp * pow(rand_x, rand_exp - 1),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     pow(rand_x, rand_exp),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f35->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getDivDeriv(exp(rand_x), 1, exp(rand_x), rand_x),
                         f41->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(exp(rand_x), 0, exp(rand_x), rand_const),
                         f42->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(exp(rand_x),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     exp(rand_x),
                                     pow(rand_x, rand_exp)),
                         f43->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(exp(rand_x),
                                     exp(rand_x),
                                     exp(rand_x),
                                     exp(rand_x)),
                         f44->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(exp(rand_x),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     exp(rand_x),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f45->getDeriv(rand_x));

        ASSERT_DOUBLE_EQ(getDivDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     1,
                                     getPolyVal(rand_coeffs, rand_x),
                                     rand_x),
                         f51->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     0,
                                     getPolyVal(rand_coeffs, rand_x),
                                     rand_const),
                         f52->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     rand_exp * pow(rand_x, rand_exp - 1),
                                     getPolyVal(rand_coeffs, rand_x),
                                     pow(rand_x, rand_exp)),
                         f53->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     exp(rand_x),
                                     getPolyVal(rand_coeffs, rand_x),
                                     exp(rand_x)),
                         f54->getDeriv(rand_x));
        ASSERT_DOUBLE_EQ(getDivDeriv(getPolyDeriv(rand_coeffs, rand_x),
                                     getPolyDeriv(rand_coeffs, rand_x),
                                     getPolyVal(rand_coeffs, rand_x),
                                     getPolyVal(rand_coeffs, rand_x)),
                         f55->getDeriv(rand_x));
    }
}
