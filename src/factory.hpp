#ifndef FACT_HEADER
#define FACT_HEADER


#include "functions.hpp"

#include <vector>
#include <map>


class TFactory
{
public:
    TFactory();
    ~TFactory();
    
    std::unique_ptr<TFunction> createObject(const std::string& name
                                        /*, const TOptions opts*/ ) const;
    
    std::vector<std::string> getAvailableObjects() const;

private:
    class TImpl;
    std::unique_ptr<const TImpl> impl;
};


#endif
