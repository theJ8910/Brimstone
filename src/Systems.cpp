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

void AbstractSystem::wait() {
    //If the system is in use by another thread, the system's mutex will be acquired by that thread
    //Wait to acquire the system's mutex then immediately release it
    std::lock_guard< std::mutex > l( m_mutex );
}

std::recursive_mutex Systems::m_systemsMutex;
void Systems::add( SystemType type ) {
    LockGuard l( m_systemsMutex );
    //System already added
    if( m_systemsByType.count( type ) > 0 )
        return;

    //Create an instance of the system, make sure returned pointer isn't null
    ISystem* system = getFactoryManager().create( type );
    if( system == nullptr )
        throw NullPointerException();

    //Load any dependencies that haven't loaded yet
    for( auto dependencyType : system->getDependencies() )
        add( dependencyType );

    //Map the created system to it's type, record the load order
    m_systemsByType.emplace( type, system );
    m_systemsByLoadOrder.push_back( system );
}

void Systems::start() {
    LockGuard l( m_systemsMutex );
    for( auto it : m_systemsByLoadOrder )
        it->start();
    join();
}

void Systems::stop() {
    LockGuard l( m_systemsMutex );
    while( !m_systemsByLoadOrder.empty() ) {
        auto system = m_systemsByLoadOrder.back();
        m_systemsByLoadOrder.pop_back();

        system->stop();
    }
}

void Systems::frame() {
    LockGuard l( m_systemsMutex );

    //Run every system's pre-frame operations
    for( auto it : m_systemsByLoadOrder )
        it->preframe();
    join();

    //Run every system's frame operations
    for( auto it : m_systemsByLoadOrder )
        it->frame();
    join();

    //Run every system's post-frame operations
    for( auto it : m_systemsByLoadOrder )
        it->postframe();
    join();
}

void Systems::join() {
    LockGuard l( m_systemsMutex );
    for( auto it : m_systemsByLoadOrder )
        it->wait();
}

FactoryManager< SystemType, ISystem* >& Systems::getFactoryManager() {
    static FactoryManager< SystemType, ISystem*> systemFactoryManager;
    return systemFactoryManager;
}

}