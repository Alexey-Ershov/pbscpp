#include "factory.hpp"


TFactory::TFactory()
    : impl { std::make_unique<TFactory::TImpl>() }
{}

TFactory::~TFactory() = default;

std::vector<std::string> TFactory::getAvailableObjects() const
{
    return impl->getAvailableObjects();
}
