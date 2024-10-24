/*
factory/FactoryManager.hpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    Defines the FactoryManager class.
    Internally, a FactoryManager class stores a map of Key => IFactory< AbstractType >,
    where Key and AbstractType are specified as template parameters.

    FactoryManager provides a simple interface to add and remove factories and
    create objects from registered factories by supplying a key.
*/
#ifndef BS_FACTORY_FACTORYMANAGER_HPP
#define BS_FACTORY_FACTORYMANAGER_HPP




//Includes
#include <unordered_map>                   //std::unordered_map

#include <brimstone/factory/IFactory.hpp>  //Brimstone::IFactory
#include <brimstone/Exception.hpp>         //Brimstone::NoSuchElementException
#include <brimstone/util/RefType.hpp>      //Brimstone::RefType




namespace Brimstone {




//Factory manager template
//Maps keys (of type Key) to factories (that create objects of type Abstract)
//Call manager.create( key ) to create an object from a managed factory with that key
template< typename Key, typename Abstract >
class FactoryManager {
private:
    using KeyRef      = typename RefType< Key >::ref;
    using KeyConstRef = typename RefType< Key >::const_ref;
    using Map         = std::unordered_map< Key, const IFactory< Abstract >& >;
    using MapPair     = typename Map::value_type;

private:
    Map m_map;
public:
    void add( KeyConstRef key, const IFactory< Abstract >& factory );
    void remove( KeyConstRef key );

    Abstract create( KeyConstRef key ) const;
};




template< typename Key, typename Abstract >
void FactoryManager< Key, Abstract >::add( KeyConstRef key, const IFactory< Abstract >& factory ) {
    m_map.emplace( key, factory );
}

template< typename Key, typename Abstract >
void FactoryManager< Key, Abstract >::remove( KeyConstRef key ) {
    m_map.erase( key );
}

template< typename Key, typename Abstract >
Abstract FactoryManager< Key, Abstract >::create( KeyConstRef key ) const {
    auto it = m_map.find( key );
    if( it == m_map.end() )
        throw NoSuchElementException();

    return it->second.create();
}




} //namespace Brimstone




#endif //BS_FACTORY_FACTORYMANAGER_HPP
