/*
factory/BasicFactory.hpp
------------------------
Copyright (c) 2024, theJ89

Description:
    Defines a basic concrete factory class, whose .create() method will instantiate
    a concrete class with its default constructor.

    Part of the engine's implementation of the Abstract Factory pattern.

    Provides a macro, BS_MAKE_FACTORY, to automate the creation of a basic factory and
    its registration with a factory manager.
*/
#ifndef BS_FACTORY_BASICFACTORY_HPP
#define BS_FACTORY_BASICFACTORY_HPP




//Includes
#include <brimstone/factory/IFactory.hpp>   //IFactory




namespace Brimstone {

template< typename Abstract, typename Concrete >
class BasicFactory : public IFactory< Abstract > {
public:
    virtual Abstract create() const;
};

template< typename Abstract, typename Concrete >
Abstract BasicFactory< Abstract, Concrete >::create() const {
    return Concrete();
}

//Specialization for pointer types
template< typename Abstract, typename Concrete >
class BasicFactory< Abstract*, Concrete* > : public IFactory< Abstract* > {
public:
    virtual Abstract* create() const;
};

template< typename Abstract, typename Concrete >
Abstract* BasicFactory< Abstract*, Concrete* >::create() const {
    return new Concrete;
}

//Make a BasicFactory (with the given abstract and concrete types) that adds itself
//to the given manager under the given key when it has constructed itself.
#define BS_MAKE_FACTORY( abstractClassName, concreteClassName, manager, key ) \
    class concreteClassName##_Factory : public Brimstone::BasicFactory< abstractClassName*, concreteClassName* > { \
    public:\
        concreteClassName##_Factory() { manager.add( key, *this ); }\
    \
    } concreteClassName##_FactoryInst{};
}





#endif //BS_FACTORY_BASICFACTORY_HPP
