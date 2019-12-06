#include "factory.hpp"


class TFactory::TImpl
{
private:
    class ICreator
    {
    public:
        virtual ~ICreator() = default;
        virtual std::unique_ptr<TFunction> create() const = 0;
    };
    
    using TCreatorPtr = std::shared_ptr<ICreator>;
    using TRegisteredCreators = std::map<const std::string, TCreatorPtr>;
    
    TRegisteredCreators registered_creators_;

public:
    template <class T>
    class TCreator : public ICreator
    {
        std::unique_ptr<TFunction> create() const override
        {
            return std::make_unique<T>();
        }
    };

    TImpl()
    {
        RegisterAll();
    }

    template <typename T>
    void registerCreator(const std::string& name)
    {
        registered_creators_[name] = std::make_shared<TCreator<T>>();
    }

    void RegisterAll()
    {
        registerCreator<TIdent>("ident");
        registerCreator<TConst>("const");
    }

    std::unique_ptr<TFunction> createObject(const std::string& name) const
    {
        auto creator = registered_creators_.find(name);
        if (creator == registered_creators_.end()) {
            return nullptr;
        }
        
        return creator->second->create();
    }

    std::vector<std::string> getAvailableObjects() const
    {
        std::vector<std::string> result;
        for (const auto& creator_pair: registered_creators_) {
            result.push_back(creator_pair.first);
        }
        
        return result;
    }
};

std::unique_ptr<TFunction> TFactory::createObject(const std::string& name) const
{
    return impl->createObject(name);
}

TFactory::TFactory()
: impl { std::make_unique<TFactory::TImpl>() }
{}

TFactory::~TFactory() = default;

std::vector<std::string> TFactory::getAvailableObjects() const
{
    return impl->getAvailableObjects();
}
