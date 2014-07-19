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
Systems::SystemsMap     Systems::m_systemsByType;
Systems::SystemsStack   Systems::m_systemsByLoadOrder;

const AbstractSystem::DependencySet& AbstractSystem::getDependencies() {
    return m_dependencies;
}

void AbstractSystem::addDependency( SystemType type ) {
    m_dependencies.insert( type );
}

void Systems::add( SystemType type ) {
    SystemTypeSet dependencyChain;
    add( type, dependencyChain );
}

void Systems::add( SystemType type, SystemTypeSet& dependencyChain ) {
    //System already added
    if( m_systemsByType.count( type ) > 0 )
        return;

    //Create an instance of the system, make sure returned pointer isn't null
    ISystem* system = getFactoryManager().create( type );
    if( system == nullptr )
        throw NullPointerException();

    //Load any dependencies that haven't loaded yet
    for( auto dependencyType : system->getDependencies() ) {
        if( dependencyChain.count( dependencyType ) > 0 )
            throw CircularDependencyException();

        dependencyChain.emplace( type );
        add( dependencyType, dependencyChain );
        dependencyChain.erase( type );
    }

    //Map the created system to it's type, record the load order
    m_systemsByType.emplace( type, system );
    m_systemsByLoadOrder.push_back( system );
}

void Systems::startAll() {
    for( auto it : m_systemsByType ) {
        it.second->start();
    }
}

void Systems::stopAll() {
    while( !m_systemsByLoadOrder.empty() ) {
        auto pcSystem = m_systemsByLoadOrder.back();
        m_systemsByLoadOrder.pop_back();

        pcSystem->stop();
    }
}

FactoryManager< SystemType, ISystem* >& Systems::getFactoryManager() {
    static FactoryManager< SystemType, ISystem*> cSystemFactoryManager;
    return cSystemFactoryManager;
}

}