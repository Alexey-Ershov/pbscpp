#include "functions.hpp"
#include "factory.hpp"


int main()
{
    // Create factory.
    TFactory func_factory;
    using TFunctionPtr = std::shared_ptr<TFunction>;
    std::vector<TFunctionPtr> cont;
    
    // Create different types of functions and add it to the cont vector.

    auto i = func_factory.createObject("ident");
    cont.emplace_back(std::move(i));

    auto c = func_factory.createObject("const", 1.5);
    cont.emplace_back(std::move(c));

    auto p = func_factory.createObject("power", 3);
    cont.emplace_back(std::move(p));

    auto e = func_factory.createObject("exp");
    cont.emplace_back(std::move(e));

    auto pol = func_factory.createObject("polynomial", { 0, 2, 3, 0, 4 });
    cont.emplace_back(std::move(pol));

    // For every function in our vector print string representation,
    // the value and the derivative value.
    std::cout << "=== BASIC FUNCTIONS ===" << std::endl;
    for (const auto ptr: cont) {
        if (ptr != nullptr) {
            std::cout << ptr->toString() << std::endl;

            double x = 10;

            std::cout << "f("
                      << x
                      << ") = "
                      << (*ptr)(x)
                      << std::endl;

            std::cout << "df/dx ("
                      << x
                      << ") = "
                      << ptr->getDeriv(x)
                      << "\n"
                      << std::endl;

        } else {
            std::cerr << "Unregistered function" << std::endl;
        }
    }


    std::cout << "=== COMPOSITE FUNCTIONS ===" << std::endl;
    
    auto s1 = func_factory.createObject("polynomial", { 10, 2 });
    auto s2 = func_factory.createObject("const", 1);
    auto sum_f = *s1 + *s2;

    auto d1 = func_factory.createObject("polynomial", { 20, 3 });
    auto d2 = func_factory.createObject("const", 2);
    auto diff_f = *d1 - *d2;
                  
    auto p1 = func_factory.createObject("polynomial", { 30, 4 });
    auto p2 = func_factory.createObject("const", 3);
    auto prod_f = (*p1) * (*p2);

    auto r1 = func_factory.createObject("polynomial", { 40, 5 });
    auto r2 = func_factory.createObject("const", 10);
    auto rat_f = *r1 / *r2;

    double x = 10;

    std::cout << "sum_f("
              << x
              << ") = "
              << (*sum_f)(x)
              << std::endl;

    std::cout << "d(sum_f)/dx ("
              << x
              << ") = "
              << sum_f->getDeriv(x)
              << "\n"
              << std::endl;


    std::cout << "diff_f("
              << x
              << ") = "
              << (*diff_f)(x)
              << std::endl;

    std::cout << "d(diff_f)/dx ("
              << x
              << ") = "
              << diff_f->getDeriv(x)
              << "\n"
              << std::endl;


    std::cout << "prod_f("
              << x
              << ") = "
              << (*prod_f)(x)
              << std::endl;

    std::cout << "d(prod_f)/dx ("
              << x
              << ") = "
              << prod_f->getDeriv(x)
              << "\n"
              << std::endl;


    std::cout << "rat_f("
              << x
              << ") = "
              << (*rat_f)(x)
              << std::endl;

    std::cout << "d(rat_f)/dx ("
              << x
              << ") = "
              << rat_f->getDeriv(x)
              << "\n"
              << std::endl;


    // Trying to create a new function via addition of the function and
    // some incompatible type.
    std::cout << "=== INCOMPATIBLE ARGUMENTS OPERATIONS ===" << std::endl;
    try {
        auto sum_f = *func_factory.createObject("exp") + "Trying to break you";
    
    } catch (const std::logic_error& exc) {
        std::cerr << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
