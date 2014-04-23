/*
FactoryManager.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines the FactoryManager class.
    Internally, a FactoryManager class stores a map of Key => IFactory< AbstractType >,
    where Key and AbstractType are specified as template parameters.

    FactoryManager provides a simple interface to add and remove factories and
    create objects from registered factories by supplying a key.
*/
#ifndef BS_FACTORYMANAGER_HPP
#define BS_FACTORYMANAGER_HPP




//Includes
#include <unordered_map>                    //std::unordered_map
#include <brimstone/factory/IFactory.hpp>   //IFactory
#include <brimstone/Exception.hpp>          //NoSuchElementException
#include <brimstone/util/RefType.hpp>       //RefType




namespace Brimstone {



//Factory manager template
//Maps keys (of type Key_t) to factories (that create objects of type Abstract_t)
//Call manager.create( key ) to create an object from a managed factory with that key
template< typename Key_t, typename Abstract_t >
class FactoryManager {
private:
    typedef typename RefType<Key_t>::ref                                KeyRef_t;
    typedef typename RefType<Key_t>::const_ref                          KeyConstRef_t;
    typedef std::unordered_map< Key_t, const IFactory< Abstract_t >& >  Map_t;
    typedef typename Map_t::value_type                                  MapPair_t;

private:
    Map_t m_cMap;
public:
    void add( KeyConstRef_t xKey, const IFactory< Abstract_t >& cFactory );
    void remove( KeyConstRef_t xKey );

    Abstract_t create( KeyConstRef_t xKey ) const;
};

template< typename Key_t, typename Abstract_t >
void FactoryManager< Key_t, Abstract_t >::add( KeyConstRef_t xKey, const IFactory< Abstract_t >& cFactory ) {
    m_cMap.insert( MapPair_t( xKey, cFactory ) );
}

template< typename Key_t, typename Abstract_t >
void FactoryManager< Key_t, Abstract_t >::remove( KeyConstRef_t xKey ) {
    m_cMap.erase( xKey );
}

template< typename Key_t, typename Abstract_t >
Abstract_t FactoryManager< Key_t, Abstract_t >::create( KeyConstRef_t xKey ) const {
    auto it = m_cMap.find( xKey );
    if( it == m_cMap.end() )
        throw NoSuchElementException();

    return it->second.create();
}




}




#endif //BS_FACTORYMANAGER_HPP