/*
linux/LinuxThreadLocal.hpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    See LinuxThreadLocal.hpp for more information.
*/




//Includes
#include "LinuxThreadLocal.hpp"     //Class header
#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone {
namespace Private {

LinuxThreadLocal::LinuxThreadLocal() {
    if( pthread_key_create( &m_key, nullptr ) )
        throw Exception( "pthread_key_create" );
}

LinuxThreadLocal::~LinuxThreadLocal() {
    if( pthread_key_delete( m_key ) )
        throw Exception( "pthread_key_delete" );
}

void  LinuxThreadLocal::set( void* const value ) {
    if( pthread_setspecific( m_key, value ) )
        throw Exception( "pthread_set_specific" );
}

void* LinuxThreadLocal::get() const {
    //Can't do error detection here.
    //If m_key is invalid, pthread_get_specific() returns nullptr.
    //However, it's also possible that m_key is valid, and is bound to a value of nullptr
    return pthread_getspecific( m_key );
}

}
}
