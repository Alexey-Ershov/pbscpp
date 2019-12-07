#include "functions.hpp"
#include "factory.hpp"


int main()
{
    TFactory func_factory;
    using TFunctionPtr = std::shared_ptr<TFunction>;
    std::vector<TFunctionPtr> cont;
    
    auto i = func_factory.createObject("ident");
    cont.emplace_back(std::move(i));

    auto c = func_factory.createObject("const", 1.5);
    cont.emplace_back(std::move(c));

    auto p = func_factory.createObject("power", 1);
    cont.emplace_back(std::move(p));

    auto e = func_factory.createObject("exp");
    cont.emplace_back(std::move(e));

    auto pol = func_factory.createObject("polynomial", { 0, 2, 3, 0, 4 });
    cont.emplace_back(std::move(pol));

    for (const auto ptr: cont) {
        if (ptr) {
            std::cout << "DEBUG: " << ptr->toString() << std::endl;

        } else {
            std::cerr << "Unregistered function" << std::endl;
        }
    }

    return 0;
}
