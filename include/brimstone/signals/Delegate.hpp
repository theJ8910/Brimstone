/*
signals/Delegate.hpp
-----------------------
Copyright (c) 2014, theJ89
Based off of Don Clugston's FastDelegate implementation.
http://www.codeproject.com/cpp/FastDelegate.asp

Description:
    Delegates can contain a reference to either a method or static method at runtime.
    This is heavily based off of Don Clugston's FastDelegate implementation. The main differences are:
    * Not quite as optimized
    * Not as portable in terms of compilers it will work on
    * No support for methods from multiple inheritance and virtual inheritance classes.
    * Uses C++11 variadic templates to support any number of arguments, reducing the amount of necessary code in this file.
*/

#ifndef BS_SIGNALS_DELEGATE_HPP
#define BS_SIGNALS_DELEGATE_HPP




//Includes
#include <brimstone/util/Cast.hpp>      //universal_cast




//Defines
//If method pointers are the same size as data pointers on this compiler,
//you can reduce the # of pointers in the Closure from 3 to 2.
#define BS_DELEGATE_POINTER_HACK



namespace Brimstone {

namespace Private {
#if defined( BS_BUILD_WINDOWS )
    class __single_inheritance GenericClass;
#elif defined( BS_BUILD_LINUX )
    class GenericClass;
#endif
    typedef GenericClass* GenericClassPtr;
    typedef void (GenericClass::*GenericMethodPtr)();

#ifndef BS_DELEGATE_POINTER_HACK
    typedef void (*GenericFunctionPtr)();
#endif //BS_DELEGATE_POINTER_HACK

    const int DELEGATE_POINTER_SIZE = sizeof( GenericMethodPtr );

    template< int N >
    struct SimplifyMethod {
        template< class X, class XFn >
        inline static GenericClassPtr convert( X* self, XFn fn, GenericMethodPtr& fnOut ) {
            static_assert( N != DELEGATE_POINTER_SIZE, "Unsupported method pointer size" );
            return nullptr;
        }
    };

    //Single-inheritance
    template<>
    struct SimplifyMethod< DELEGATE_POINTER_SIZE > {
        template< class X, class XFn >
        inline static GenericClassPtr convert( X* self, XFn fn, GenericMethodPtr& fnOut ) {
            fnOut = reinterpret_cast< GenericMethodPtr >( fn );
            return reinterpret_cast< GenericClassPtr >( self );
        }
    };

    //Method is expected to be of the form:
    //    Return GenericClass::(*)( Arg1, Arg2, ... )
    //StaticMethod is expected to be of the form:
    //    Return (*)( Arg1, Arg2, ... )
    template< typename Method, typename StaticMethod >
    class Closure {
    public:
        template< typename X, class XFn >
        void                bindMethod( X* self, XFn fn );

        template< typename X, class XFn >
        void                bindConstMethod( const X* self, XFn fn );

        template< typename Delegate, typename Invoker, typename Fn >
        void                bindStaticMethod( Delegate* dlgt, Invoker invoker, Fn fn );

        GenericClassPtr     getSelf() const;
        Method              getMethod() const;
        StaticMethod        getStaticMethod() const;
    public:
        GenericClassPtr     m_self;
        GenericMethodPtr    m_method;
#ifndef BS_DELEGATE_POINTER_HACK
        GenericFunctionPtr  m_staticMethod;
#endif //BS_DELEGATE_POINTER_HACK
    };

    template< typename Method, typename StaticMethod >
    template< typename X, class XFn >
    inline void Closure< Method, StaticMethod >::bindMethod( X* self, XFn fn ) {
        m_self = SimplifyMethod< sizeof( fn ) >::convert( self, fn, m_method );
#ifndef BS_DELEGATE_POINTER_HACK
        m_staticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method, typename StaticMethod >
    template< typename X, class XFn >
    inline void Closure< Method, StaticMethod >::bindConstMethod( const X* self, XFn fn ) {
        m_self = SimplifyMethod< sizeof( fn ) >::convert( const_cast< X* >( self ), fn, m_method );
#ifndef BS_DELEGATE_POINTER_HACK
        m_staticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method, typename StaticMethod >
    template< typename Delegate, typename Invoker, typename Fn >
    inline void Closure< Method, StaticMethod >::bindStaticMethod( Delegate* dlgt, Invoker invoker, Fn fn ) {
#ifdef BS_DELEGATE_POINTER_HACK
        static_assert( sizeof( StaticMethod ) == sizeof( this ), "Can't pointer hack" );
        SimplifyMethod< sizeof( invoker ) >::convert( dlgt, invoker, m_method );
        m_self = universal_cast< GenericClassPtr >( fn );
#else //BS_DELEGATE_POINTER_HACK
        m_self          = SimplifyMethod< sizeof( invoker ) >::convert( dlgt, invoker, m_method );
        m_staticMethod  = reinterpret_cast< GenericFunctionPtr >( fn );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method, typename StaticMethod >
    inline GenericClassPtr Closure< Method, StaticMethod >::getSelf() const {
        return m_self;
    }

    template< typename Method, typename StaticMethod >
    inline Method Closure< Method, StaticMethod >::getMethod() const {
        return reinterpret_cast< Method >( m_method );
    }

    template< typename Method, typename StaticMethod >
    inline StaticMethod Closure< Method, StaticMethod >::getStaticMethod() const {
#ifdef BS_DELEGATE_POINTER_HACK
        //HACK: When a static method is bound:
        //    self    = the static function
        //    pMethod = the staticInvoker method of the Delegate class
        //Invoking a Delegate performs:
        //    ( closure.getSelf() )->( closure.getMethod() )()
        //In other words, it runs the staticInvoker() method, passing the static function as "this"!
        //    staticInvoker() calls ( closure.getStaticMethod() )().
        //    Since "closure" is the only object in Delegate, &closure == this == the static method.
        //    So we just reinterpret "this" as being a function pointer instead of a data pointer and return it.
        static_assert( sizeof( StaticMethod ) == sizeof( this ), "Can't pointer hack" );
        return universal_cast< StaticMethod >( this );
#else  //BS_DELEGATE_POINTER_HACK
        return reinterpret_cast< StaticMethod >( m_staticMethod );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method, typename StaticMethod >
    bool operator ==( const Closure< Method, StaticMethod >& left, const Closure< Method, StaticMethod >& right ) {
        return ( left.m_self            == right.m_self         &&
                 left.m_method          == right.m_method
    #ifndef BS_DELEGATE_POINTER_HACK
                                                                &&
                 left.m_staticMethod    == right.m_staticMethod
    #endif
               );
    }

    template< typename Method, typename StaticMethod >
    bool operator !=( const Closure< Method, StaticMethod >& left, const Closure< Method, StaticMethod >& right ) {
        return ( left.m_self            != right.m_self         ||
                 left.m_method          != right.m_method
    #ifndef BS_DELEGATE_POINTER_HACK
                                                                ||
                 left.m_staticMethod    != right.m_staticMethod
    #endif
               );
    }
}

/*
Delegate template definition.
Use the specialized version that takes a function signature instead
*/
template< typename Signature >
class Delegate;

/*
Specialized version of the delegate template that takes a function signature.
Delegates can be bound to either methods, static methods, or functors
*/
template< typename Return, typename... Args >
class Delegate< Return ( Args... ) > {
public:
    Delegate();

    //Bind an object + a method of its class
    template< typename T, typename BaseClass >
    Delegate( T* ptr, Return (BaseClass::*method)( Args... ) );

    //Bind an object + a const method of its class
    template< typename T, typename BaseClass >
    Delegate( const T* ptr, Return (BaseClass::*constMethod)( Args... ) const );

    //Bind static method / normal function
    Delegate( Return (*staticFunction)( Args... ) );

    template< typename T, typename BaseClass >
    void bind( T* ptr, Return (BaseClass::*fn)( Args... ) );

    template< typename T, typename BaseClass >
    void bind( const T* ptr, Return (BaseClass::*fn)( Args... ) const );

    void bind( Return (*staticFunction)( Args... ) );

    Return operator()( Args... args ) const;

    template< typename Signature >
    friend bool operator ==( const Delegate< Signature >& left, const Delegate< Signature >& right );

    template< typename Signature >
    friend bool operator !=( const Delegate< Signature >& left, const Delegate< Signature >& right );
private:
    Return staticInvoker( Args... args );
private:
    typedef Return (Private::GenericClass::*Method)( Args... );
    typedef Return (*StaticMethod)( Args... );
    Private::Closure< Method, StaticMethod >    m_closure;
};

template< typename Return, typename... Args >
Delegate< Return ( Args... ) >::Delegate() {
}

template< typename Return, typename... Args >
template< typename T, typename BaseClass >
Delegate< Return ( Args... ) >::Delegate( T* ptr, Return (BaseClass::*method)( Args... ) ) {
    bind( ptr, method );
}

template< typename Return, typename... Args >
template< typename T, typename BaseClass >
Delegate< Return ( Args... ) >::Delegate( const T* ptr, Return (BaseClass::*constMethod)( Args... ) const ) {
    bind( ptr, constMethod );
}

template< typename Return, typename... Args >
Delegate< Return ( Args... ) >::Delegate( Return (*staticFunction)( Args... ) ) {
    bind( staticFunction );
}

template< typename Return, typename... Args >
template< typename T, typename BaseClass >
inline void Delegate< Return ( Args... ) >::bind( T* ptr, Return (BaseClass::*method)( Args... ) ) {
    m_closure.bindMethod( ptr, method );
}

template< typename Return, typename... Args >
template< typename T, typename BaseClass >
inline void Delegate< Return ( Args... ) >::bind( const T* ptr, Return (BaseClass::*constMethod)( Args... ) const ) {
    m_closure.bindConstMethod( ptr, constMethod );
}

template< typename Return, typename... Args >
inline void Delegate< Return ( Args... ) >::bind( Return (*staticFunction)( Args... ) ) {
    m_closure.bindStaticMethod( this, &Delegate::staticInvoker, staticFunction );
}

template< typename Return, typename... Args >
Return Delegate< Return ( Args... ) >::operator()( Args... args ) const {
    return ( ( m_closure.getSelf() )->*( m_closure.getMethod() ) )( args... );
}

template< typename Signature >
bool operator ==( const Delegate< Signature >& left, const Delegate< Signature >& right ) {
    return left.m_closure == right.m_closure;
}

template< typename Signature >
bool operator !=( const Delegate< Signature >& left, const Delegate< Signature >& right ) {
    return left.m_closure != right.m_closure;
}

template< typename Return, typename... Args >
Return Delegate< Return ( Args... ) >::staticInvoker( Args... args ) {
    return ( *( m_closure.getStaticMethod() ) )( args... );
}

template< typename Return, typename... Args >
inline Delegate< Return ( Args... ) > bind( Return (*staticFunction)( Args... ) ) {
    return Delegate< Return ( Args... ) >( staticFunction );
}

template< typename T, typename BaseClass, typename Return, typename... Args >
inline Delegate< Return ( Args... ) > bind( T* self, Return (BaseClass::*method)( Args... ) ) {
    return Delegate< Return ( Args... ) >( self, method );
}

template< typename T, typename BaseClass, typename Return, typename... Args >
inline Delegate< Return ( Args... ) > bind( const T* self, Return (BaseClass::*constMethod)( Args... ) const ) {
    return Delegate< Return ( Args... ) >( self, constMethod );
}

}

#endif //BS_SIGNALS_DELEGATE_HPP
