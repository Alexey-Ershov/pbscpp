#include "functions.hpp"
#include "factory.hpp"
#include "eqsolution.hpp"


int main()
{
    TFactory func_factory;
    
    auto f = func_factory.createObject("polynomial", { 1, 1, 1, 1, 1, 1 });
    std::cerr << f->toString() << std::endl;

    auto eq_root = EqSolver().solveEquation(*f);
    
    if (eq_root.has_value()) {
        std::cerr.precision(2);
        std::cerr << std::fixed << "root = " << eq_root.value() << std::endl;
    
    } else {
        std::cerr << "No root" << std::endl;
    }

    return 0;
}
