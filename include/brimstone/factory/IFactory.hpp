/*
factory/IFactory.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines an abstract factory interface, IFactory.
    Part of the engine's implementation of the Abstract Factory pattern.
*/

#ifndef BS_FACTORY_IFACTORY_HPP
#define BS_FACTORY_IFACTORY_HPP




//Factory interface template
template< typename Abstract >
class IFactory {
public:
    virtual Abstract create() const = 0;
};




#endif //BS_FACTORY_IFACTORY_HPP