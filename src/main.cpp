#include "functions.hpp"
#include "factory.hpp"


int main()
{
    /*std::unique_ptr<TFunction> p {new TIdent};
    std::cout << "DEBUG: " << p->toString() << std::endl;

    p = std::make_unique<TConst>();
    std::cout << "DEBUG: " << p->toString() << std::endl;*/


    TFactory funcFactory;
    
    auto f = funcFactory.createObject("ident");
    std::cout << f->toString() << std::endl;

    auto g = funcFactory.createObject("const");
    std::cout << g->toString() << std::endl;

    return 0;
}
