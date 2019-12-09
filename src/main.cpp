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

    // Create the function from the addition of two functions.
    auto sum_f = *func_factory.createObject("exp") +
                 *func_factory.createObject("polynomial", { 1, 2, 3 });

    cont.emplace_back(std::move(sum_f));

    // For every function in our vector print string representation,
    // the value and the derivative value.
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

    // Trying to create a new function via addition of the function and
    // some incompatible type.
    try {
        auto sum_f = *func_factory.createObject("exp") + "Trying to break you";
    
    } catch (const std::logic_error& exc) {
        std::cerr << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
