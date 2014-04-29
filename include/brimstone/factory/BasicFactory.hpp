/*
factory/BasicFactory.hpp
-----------------------
Copyright (c) 2014, theJ89

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

template< typename Abstract_t, typename Concrete_t >
class BasicFactory : public IFactory< Abstract_t > {
public:
    virtual Abstract_t create() const;
};

template< typename Abstract_t, typename Concrete_t >
Abstract_t BasicFactory< Abstract_t, Concrete_t >::create() const {
    return Concrete_t();
}

//Specialization for pointer types
template< typename Abstract_t, typename Concrete_t >
class BasicFactory< Abstract_t*, Concrete_t* > : public IFactory< Abstract_t* > {
public:
    virtual Abstract_t* create() const;
};

template< typename Abstract_t, typename Concrete_t >
Abstract_t* BasicFactory< Abstract_t*, Concrete_t* >::create() const {
    return new Concrete_t;
}

//Make a BasicFactory (with the given abstract and concrete types) that adds itself
//to the given manager under the given key when it has constructed itself.
#define BS_MAKE_FACTORY( abstractClassName, concreteClassName, manager, key ) \
    class concreteClassName##_Factory : public Brimstone::BasicFactory< abstractClassName*, concreteClassName* > { \
    public:\
        concreteClassName##_Factory() { manager##.add( key, *this ); }\
    \
    } concreteClassName##_FactoryInst{};
}





#endif //BS_FACTORY_BASICFACTORY_HPP