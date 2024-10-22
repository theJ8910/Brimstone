/*
util/ThreadLocal.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    This file contains classes to make using thread-local storage (TLS) easier.

    Every thread has its own TLS. TLS can be thought of as an array of void pointers,
    and a thread-local variable can be thought of as an index in this array.
    Whenever a thread reads from or writes to a thread-local variable, it is
    actually reading from / writing to that index in that thread's TLS.

    UntypedThreadLocal, a wrapper class around a platform-dependent implementation
    of a thread-local storage, is defined here. It is not type-safe; you must manually
    cast to and from void*. Use the .get() and .set() methods to accomplish reads and writes, respectively.

    ThreadLocal, a type-safe wrapper class around an UntypedThreadLocal, is also defined here.
    ThreadLocal is a template class taking a parameter T.
    A ThreadLocal object allows you to assign values of type T, and implicit conversions to T.
    T must be the same size as a void* (e.g. same size as std::size_t; typically 4 bytes when compiling
    for x86, or 8 bytes when compiling for x64).
*/
#ifndef BS_UTIL_THREADLOCAL_HPP
#define BS_UTIL_THREADLOCAL_HPP




//Includes
#include <type_traits>  //std::integral_constant




namespace Brimstone::Private {




#if defined( BS_BUILD_WINDOWS )
using ThreadLocalImpl = class WindowsThreadLocal;
#elif defined( BS_BUILD_LINUX )
using ThreadLocalImpl = class LinuxThreadLocal;
#endif

template< typename T >
struct SameSizeAsVoidPtr {
    using value = std::integral_constant< bool, sizeof(T) == sizeof(void*) >;
};




} //namespace Brimstone




namespace Brimstone {




//set and get use void*
class UntypedThreadLocal {
public:
    UntypedThreadLocal();
    ~UntypedThreadLocal();
    void set( void* const value );
    void* get() const;
    UntypedThreadLocal& operator =( void* const value );
    operator void*() const;
private:
    Private::ThreadLocalImpl* m_impl;
};



//Template class wrapping UntypedThreadLocal
template< typename T >
class ThreadLocal {
public:
    void set( const T value );
    T get() const;
    ThreadLocal& operator =( const T value );
    operator T() const;
private:
    UntypedThreadLocal m_tl;
};

template< typename T >
void ThreadLocal<T>::set( const T value ) {
    m_tl.set( reinterpret_cast< void* >( value ) );
}

template< typename T >
T ThreadLocal<T>::get() const {
    return reinterpret_cast< T >( m_tl.get() );
}

template< typename T >
ThreadLocal<T>& ThreadLocal<T>::operator =( const T value ) {
    set( value );
    return *this;
}

template< typename T >
ThreadLocal<T>::operator T() const {
    return get();
}




} //namespace Brimstone




#endif //BS_UTIL_THREADLOCAL_HPP
