/*
factory/ThrowFactory.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines a concrete factory class, whose .create() method will
    throw an instance of the concrete class as an exception.

    Part of the engine's implementation of the Abstract Factory pattern.

    Provides a macro, BS_MAKE_THROW_FACTORY, to automate the creation of a throw factory and
    its registration with a factory manager.
*/
#ifndef BS_FACTORY_THROWFACTORY_HPP
#define BS_FACTORY_THROWFACTORY_HPP




//Includes
#include <brimstone/factory/IFactory.hpp>   //IFactory




namespace Brimstone {

//Basic implementation of IFactory, whose .create() method throws concreteClassName with its default constructor.
template< typename Concrete >
class ThrowFactory : public IFactory< void > {
public:
    virtual void create() const;
};

template< typename Concrete >
virtual void ThrowFactory< Concrete >::create() const {
    throw Concrete();
}

//Make a ThrowFactory (with the given concrete type) that adds itself
//to the given manager under the given key when it has constructed itself.
#define BS_MAKE_THROW_FACTORY( concreteClassName, manager, key ) \
    class concreteClassName##_Factory : public ThrowFactory< void > { \
    public:\
        concreteClassName##_Factory() { manager.add( key, *this ); }\
    \
    } concreteClassName##_FactoryInst{ manager, key };

}





#endif //BS_FACTORY_THROWFACTORY_HPP