#ifndef FACT_HEADER
#define FACT_HEADER


#include "functions.hpp"

#include <vector>
#include <map>


class TFactory
{
private:
    class TImpl
    {
    private:
        class ICreator
        {
        public:
            virtual ~ICreator() = default;
            virtual std::unique_ptr<TFunction> create() const = 0;
            virtual std::unique_ptr<TFunction> create(double opt) const = 0;
            virtual std::unique_ptr<TFunction> create(int opt) const = 0;
            virtual std::unique_ptr<TFunction> create(
                    const std::vector<double>& opt) const = 0;
        };
        
        using TCreatorPtr = std::shared_ptr<ICreator>;
        using TRegisteredCreators = std::map<const std::string, TCreatorPtr>;
        
        TRegisteredCreators registered_creators_;

    public:
        template<class T>
        class TCreator : public ICreator
        {
            virtual std::unique_ptr<TFunction> create() const override
            {
                return std::make_unique<T>();
            }

            virtual std::unique_ptr<TFunction> create(double opt)
                    const override
            {
                return std::make_unique<T>(opt);
            }
            
            virtual std::unique_ptr<TFunction> create(int opt) const override
            {
                return std::make_unique<T>(opt);
            }
            
            virtual std::unique_ptr<TFunction> create(
                    const std::vector<double>& opt) const override
            {
                return std::make_unique<T>(opt);
            }
        };

        TImpl()
        {
            registerAll();
        }

        template<class T>
        void registerCreator(const std::string& name)
        {
            registered_creators_[name] = std::make_shared<TCreator<T>>();
        }

        void registerAll()
        {
            registerCreator<TIdent>("ident");
            registerCreator<TConst>("const");
            registerCreator<TPower>("power");
            registerCreator<TExp>("exp");
            registerCreator<TPolynomial>("polynomial");
        }

        std::unique_ptr<TFunction> createObject(const std::string& name) const
        {
            auto creator = registered_creators_.find(name);
            if (creator == registered_creators_.end()) {
                return nullptr;
            }
            
            return creator->second->create();
        }

        std::unique_ptr<TFunction> createObject(
                const std::string& name, 
                const std::initializer_list<double>& opts) const
        {
            auto creator = registered_creators_.find(name);
            if (creator == registered_creators_.end()) {
                return nullptr;
            }
            
            return creator->second->create(opts);
        }

        template<class T>
        std::unique_ptr<TFunction> createObject(const std::string& name,
                                                const T& opts) const
        {
            auto creator = registered_creators_.find(name);
            if (creator == registered_creators_.end()) {
                return nullptr;
            }
            
            /*if (name == "ident" or name == "exp") {*/
                return creator->second->create(opts);
            
            /*} else if (name == )*/
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

    std::unique_ptr<const TImpl> impl;

public:
    TFactory();
    ~TFactory();
    
    std::unique_ptr<TFunction> createObject(const std::string& name) const
    {
        return impl->createObject(name);
    }

    std::unique_ptr<TFunction> createObject(
            const std::string& name, 
            const std::initializer_list<double>& opts) const
    {
        return impl->createObject(name, opts);
    }

    template<class T>
    std::unique_ptr<TFunction> createObject(const std::string& name,
                                            const T& opts) const
    {
        return impl->createObject(name, opts);
    }

    std::vector<std::string> getAvailableObjects() const;
};


#endif
