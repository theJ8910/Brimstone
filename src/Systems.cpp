/*
Systems.cpp
-----------------------
Copyright (c) 2014, theJ89
Based on code written by Zachary Blystone.

Description:
    See Systems.hpp for more information.
*/




//Includes
#include <brimstone/Systems.hpp>




namespace Brimstone {

//Static initializers
Systems::SystemsMap_t   Systems::m_acSystemsByType;
Systems::SystemsStack_t Systems::m_acSystemsByLoadOrder;

const AbstractSystem::DependencySet_t& AbstractSystem::getDependencies() {
    return m_aeDependencies;
}

void AbstractSystem::addDependency( SystemType eType ) {
    m_aeDependencies.insert( eType );
}

void Systems::add( SystemType eType ) {
    SystemTypeSet_t aeDependencyChain;
    add( eType, aeDependencyChain );
}

void Systems::add( SystemType eType, SystemTypeSet_t& aeDependencyChain ) {
    //System already added
    if( m_acSystemsByType.count( eType ) > 0 )
        return;

    //Create an instance of the system, make sure returned pointer isn't null
    ISystem* pcSystem = getFactoryManager().create( eType );
    if( pcSystem == nullptr )
        throw NullPointerException();

    //Load any dependencies that haven't loaded yet
    for( auto eDependencyType : pcSystem->getDependencies() ) {
        if( aeDependencyChain.count( eDependencyType ) > 0 )
            throw CircularDependencyException();

        aeDependencyChain.emplace( eType );
        add( eDependencyType, aeDependencyChain );
        aeDependencyChain.erase( eType );
    }

    //Map the created system to it's type, record the load order
    m_acSystemsByType.emplace( eType, pcSystem );
    m_acSystemsByLoadOrder.push_back( pcSystem );
}

void Systems::startAll() {
    for( auto it : m_acSystemsByType ) {
        it.second->start();
    }
}

void Systems::stopAll() {
    while( !m_acSystemsByLoadOrder.empty() ) {
        auto pcSystem = m_acSystemsByLoadOrder.back();
        m_acSystemsByLoadOrder.pop_back();

        pcSystem->stop();
    }
}

FactoryManager< SystemType, ISystem* >& Systems::getFactoryManager() {
    static FactoryManager< SystemType, ISystem*> cSystemFactoryManager;
    return cSystemFactoryManager;
}

}