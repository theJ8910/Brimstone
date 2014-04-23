#ifndef BS_EXCEPTION_FACTORY_HPP
#define BS_EXCEPTION_FACTORY_HPP




//Includes
#include <unordered_map>    //std::unordered_map
#include "Exception.hpp"    //NoSuchElementException, UnexpectedResultException
#include "RefType.hpp"      //RefType



//Macros
//Makes a simple exception factory class whose .throwException() method instantiates a.Similar to a Factory, but instead of allocating and returning a new object,
//throws an exception
#define BS_MAKE_EXCEPTION_FACTORY( exceptionClassName, manager, key ) \
    class exceptionClassName##_ExceptionFactory : public IExceptionFactory {\
    public:\
        exceptionClassName##_ExceptionFactory() {\
            manager.add( key, *this );\
        }\
        virtual void throwException() const {\
            throw exceptionClassName();\
        }\
    } exceptionClassName##_ExceptionFactoryInstance;


namespace Brimstone {

//Exception factory interface
class IExceptionFactory {
public:
    virtual void throwException() const = 0;
};

//Exception factory manager template
//Maps keys (of type Key_t) to exception factories
//Call manager.throwException( key ) to throw an exception from a managed factory with that key
template< typename Key_t >
class ExceptionFactoryManager {
private:
    typedef typename RefType<Key_t>::ref                                    KeyRef_t;
    typedef typename RefType<Key_t>::const_ref                              KeyConstRef_t;
    typedef          std::unordered_map< Key_t, const IExceptionFactory& >  Map_t;
    typedef typename Map_t::value_type                                      MapPair_t;
private:
    Map_t m_cMap;
public:
    void add( KeyConstRef_t _Key, const IExceptionFactory& cExceptionFactory );
    void remove( KeyConstRef_t _Key );

    void throwException( KeyConstRef_t _Key ) const;
};

template< typename Key_t >
void ExceptionFactoryManager< Key_t >::add( KeyConstRef_t _Key, const IExceptionFactory& cFactory ) {
    m_cMap.insert( MapPair_t( _Key, cFactory ) );
}

template< typename Key_t >
void ExceptionFactoryManager< Key_t >::remove( KeyConstRef_t _Key ) {
    m_cMap.erase( _Key );
}

template< typename Key_t >
void ExceptionFactoryManager< Key_t >::throwException( KeyConstRef_t _Key ) const {
    auto it = m_cMap.find( _Key );
    if( it == m_cMap.end() )
        throw NoSuchElementException();

    it->second.throwException();

    //Just in case an exception isn't thrown above, we make sure one is thrown here
    throw UnexpectedResultException();
}

}


#endif //BS_EXCEPTION_FACTORY_HPP