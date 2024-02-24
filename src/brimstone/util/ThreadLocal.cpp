/*
util/ThreadLocal.cpp
--------------------
Copyright (c) 2024, theJ89

Description:
    See ThreadLocal.hpp for more information.
*/




//Includes
#include <brimstone/util/ThreadLocal.hpp>       //Class header

//Brimstone::Private::ThreadLocalImpl
#if defined( BS_BUILD_WINDOWS )
#include "../windows/WindowsThreadLocal.hpp"
#elif defined( BS_BUILD_LINUX )
#include "../linux/LinuxThreadLocal.hpp"
#endif




namespace Brimstone {

UntypedThreadLocal::UntypedThreadLocal() : m_impl( nullptr ) {
    m_impl = new Private::ThreadLocalImpl();
}

UntypedThreadLocal::~UntypedThreadLocal() {
    if( m_impl != nullptr )
        delete m_impl;
}

void UntypedThreadLocal::set( void* const value ) {
    return m_impl->set( value );
}

void* UntypedThreadLocal::get() const {
    return m_impl->get();
}

UntypedThreadLocal& UntypedThreadLocal::operator =( void* const value ) {
    set( value );
    return *this;
}

UntypedThreadLocal::operator void*() const {
    return get();
}

}
