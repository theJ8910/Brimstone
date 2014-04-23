/*
Delegate.hpp
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

#ifndef BS_DELEGATE_HPP
#define BS_DELEGATE_HPP




//Includes
#include <type_traits>
#include <brimstone/util/Cast.hpp>      //universal_cast




//Defines
//If method pointers are the same size as data pointers on this compiler,
//you can reduce the # of pointers in the Closure from 3 to 2.
#define BS_DELEGATE_POINTER_HACK  



namespace Brimstone {

namespace Private {
    class __single_inheritance GenericClass;
    typedef GenericClass* GenericClassPtr;
    typedef void (GenericClass::*GenericMethodPtr)();

#ifndef BS_DELEGATE_POINTER_HACK
    typedef void (*GenericFunctionPtr)();
#endif //BS_DELEGATE_POINTER_HACK

    const int DELEGATE_POINTER_SIZE = sizeof( GenericMethodPtr );

    template< int N >
    struct SimplifyMethod {
        template< class X, class XFn_t >
        inline static GenericClassPtr convert( X* pThis, XFn_t pfn, GenericMethodPtr& pfnOut ) {
            typedef char ERROR_UnsupportedMethodPointerSize[ N-100 ];
            return nullptr;
        }
    };

    //Single-inheritance
    template<>
    struct SimplifyMethod< DELEGATE_POINTER_SIZE > {
        template< class X, class XFn_t >
        inline static GenericClassPtr convert( X* pThis, XFn_t pfn, GenericMethodPtr& pfnOut ) {
            pfnOut = reinterpret_cast< GenericMethodPtr >( pfn );
            return reinterpret_cast< GenericClassPtr >( pThis );
        }
    };

    //Method_t is expected to be of the form:
    //    Return_t GenericClass::(*)( Arg1, Arg2, ... )
    //StaticMethod_t is expected to be of the form:
    //    Return_t (*)( Arg1, Arg2, ... )
    template< typename Method_t, typename StaticMethod_t >
    class Closure {
    public:
        template< typename X, class XFn_t >
        void                bindMethod( X* pThis, XFn_t pfn );

        template< typename X, class XFn_t >
        void                bindConstMethod( const X* pThis, XFn_t pfn );

        template< typename Delegate_t, typename Invoker_t, typename Fn_t >
        void                bindStaticMethod( Delegate_t* pcDelegate, Invoker_t pfnInvoker, Fn_t pfn );

        GenericClassPtr     getThis() const;
        Method_t            getMethod() const;
        StaticMethod_t      getStaticMethod() const;

        template< typename Method_t, typename StaticMethod_t >
        friend bool operator ==( const Closure< Method_t, StaticMethod_t >& cLeft, const Closure< Method_t, StaticMethod_t >& cRight );

        template< typename Method_t, typename StaticMethod_t >
        friend bool operator !=( const Closure< Method_t, StaticMethod_t >& cLeft, const Closure< Method_t, StaticMethod_t >& cRight );

    private:
        GenericClassPtr     m_pcThis;
        GenericMethodPtr    m_pfnMethod;
#ifndef BS_DELEGATE_POINTER_HACK
        GenericFunctionPtr  m_pfnStaticMethod;
#endif //BS_DELEGATE_POINTER_HACK
    };

    template< typename Method_t, typename StaticMethod_t >
    template< typename X, class XFn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindMethod( X* pcThis, XFn_t pfn ) {
        m_pcThis = SimplifyMethod< sizeof( pfn ) >::convert( pcThis, pfn, m_pfnMethod );
#ifndef BS_DELEGATE_POINTER_HACK
        m_pfnStaticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    template< typename X, class XFn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindConstMethod( const X* pThis, XFn_t pfn ) {
        m_pcThis = SimplifyMethod< sizeof( pfn ) >::convert( const_cast< X* >( pThis ), pfn, m_pfnMethod );
#ifndef BS_DELEGATE_POINTER_HACK
        m_pfnStaticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    template< typename Delegate_t, typename Invoker_t, typename Fn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindStaticMethod( Delegate_t* pcDelegate, Invoker_t pfnInvoker, Fn_t pfn ) {
#ifdef BS_DELEGATE_POINTER_HACK
        typedef int ERROR_CantPointerHack[ sizeof( StaticMethod_t ) == sizeof( this ) ? 1 : -1 ];
        SimplifyMethod< sizeof( pfnInvoker ) >::convert( pcDelegate, pfnInvoker, m_pfnMethod );
        m_pcThis = universal_cast< GenericClassPtr >( pfn );
#else //BS_DELEGATE_POINTER_HACK
        m_pcThis          = SimplifyMethod< sizeof( pfnInvoker ) >::convert( pcDelegate, pfnInvoker, m_pfnMethod );
        m_pfnStaticMethod = reinterpret_cast< GenericFunctionPtr >( pfn );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    inline GenericClassPtr Closure< Method_t, StaticMethod_t >::getThis() const {
        return m_pcThis;
    }

    template< typename Method_t, typename StaticMethod_t >
    inline Method_t Closure< Method_t, StaticMethod_t >::getMethod() const {
        return reinterpret_cast< Method_t >( m_pfnMethod );
    }

    template< typename Method_t, typename StaticMethod_t >
    inline StaticMethod_t Closure< Method_t, StaticMethod_t >::getStaticMethod() const {
#ifdef BS_DELEGATE_POINTER_HACK
        //HACK: When a static method is bound:
        //    pThis   = the static function
        //    pMethod = the staticInvoker method of the Delegate class
        //Invoking a Delegate performs:
        //    ( closure.getThis() )->( closure.getMethod() )()
        //In other words, it runs the staticInvoker() method, passing the static function as "this"!
        //    staticInvoker() calls ( closure.getStaticMethod() )().
        //    Since "closure" is the only object in Delegate, &closure == this == the static method.
        //    So we just reinterpret "this" as being a function pointer instead of a data pointer and return it.
        typedef int ERROR_CantPointerHack[ sizeof( StaticMethod_t ) == sizeof( this ) ? 1 : -1 ];
        return universal_cast< StaticMethod_t >( this );
#else  //BS_DELEGATE_POINTER_HACK
        return reinterpret_cast< StaticMethod_t >( m_pfnStaticMethod );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    bool operator ==( const Closure< Method_t, StaticMethod_t >& cLeft, const Closure< Method_t, StaticMethod_t >& cRight ) {
        return ( cLeft.m_pcThis            == cRight.m_pcThis           &&
                 cLeft.m_pfnMethod         == cRight.m_pfnMethod
    #ifndef BS_DELEGATE_POINTER_HACK
                                                                        &&
                 cLeft.m_pfnStaticMethod   == cRight.m_pfnStaticMethod
    #endif
               );
    }

    template< typename Method_t, typename StaticMethod_t >
    bool operator !=( const Closure< Method_t, StaticMethod_t >& cLeft, const Closure< Method_t, StaticMethod_t >& cRight ) {
        return ( cLeft.m_pcThis            != cRight.m_pcThis           ||
                 cLeft.m_pfnMethod         != cRight.m_pfnMethod
    #ifndef BS_DELEGATE_POINTER_HACK
                                                                        ||
                 cLeft.m_pfnStaticMethod   != cRight.m_pfnStaticMethod
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
template< typename Return_t, typename... Args >
class Delegate< Return_t ( Args... ) > {
public:
    Delegate();

    //Bind an object + a method of its class
    template< typename T, typename BaseClass_t >
    Delegate( T* ptr, Return_t (BaseClass_t::*fn)( Args... ) );

    //Bind an object + a const method of its class
    template< typename T, typename BaseClass_t >
    Delegate( const T* ptr, Return_t (BaseClass_t::*fn)( Args... ) const );

    //Bind static method / normal function
    Delegate( Return_t (*fn)( Args... ) );

    template< typename T, typename BaseClass_t >
    void bind( T* ptr, Return_t (BaseClass_t::*fn)( Args... ) );

    template< typename T, typename BaseClass_t >
    void bind( const T* ptr, Return_t (BaseClass_t::*fn)( Args... ) const );

    void bind( Return_t (*fn)( Args... ) );

    Return_t operator()( Args... args ) const;

    template< typename Signature >
    friend bool operator ==( const Delegate< Signature >& cLeft, const Delegate< Signature >& cRight );

    template< typename Signature >
    friend bool operator !=( const Delegate< Signature >& cLeft, const Delegate< Signature >& cRight );
private:
    Return_t staticInvoker( Args... args );
private:
    typedef Return_t (Private::GenericClass::*Method_t)( Args... );
    typedef Return_t (*StaticMethod_t)( Args... );
    Private::Closure< Method_t, StaticMethod_t >    m_cClosure;
};

template< typename Return_t, typename... Args >
Delegate< Return_t ( Args... ) >::Delegate() {
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
Delegate< Return_t ( Args... ) >::Delegate( T* ptr, Return_t (BaseClass_t::*fn)( Args... ) ) {
    bind( ptr, fn );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
Delegate< Return_t ( Args... ) >::Delegate( const T* ptr, Return_t (BaseClass_t::*fn)( Args... ) const ) {
    bind( ptr, fn );
}

template< typename Return_t, typename... Args >
Delegate< Return_t ( Args... ) >::Delegate( Return_t (*fn)( Args... ) ) {
    bind( fn );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
inline void Delegate< Return_t ( Args... ) >::bind( T* ptr, Return_t (BaseClass_t::*fn)( Args... ) ) {
    m_cClosure.bindMethod( ptr, fn );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
inline void Delegate< Return_t ( Args... ) >::bind( const T* ptr, Return_t (BaseClass_t::*fn)( Args... ) const ) {
    m_cClosure.bindConstMethod( ptr, fn );
}

template< typename Return_t, typename... Args >
inline void Delegate< Return_t ( Args... ) >::bind( Return_t (*fn)( Args... ) ) {
    m_cClosure.bindStaticMethod( this, &Delegate::staticInvoker, fn );
}

template< typename Return_t, typename... Args >
Return_t Delegate< Return_t ( Args... ) >::operator()( Args... args ) const {
    return ( ( m_cClosure.getThis() )->*( m_cClosure.getMethod() ) )( args... );
}

template< typename Signature >
bool operator ==( const Delegate< Signature >& cLeft, const Delegate< Signature >& cRight ) {
    return cLeft.m_cClosure == cRight.m_cClosure;
}

template< typename Signature >
bool operator !=( const Delegate< Signature >& cLeft, const Delegate< Signature >& cRight ) {
    return cLeft.m_cClosure != cRight.m_cClosure;
}

template< typename Return_t, typename... Args >
Return_t Delegate< Return_t ( Args... ) >::staticInvoker( Args... args ) {
    return ( *( m_cClosure.getStaticMethod() ) )( args... );
}

template< typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( Return_t (*fnStatic)( Args... ) ) {
    return Delegate< Return_t ( Args... ) >( fnStatic );
}

template< typename T, typename BaseClass, typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( T* pcThis, Return_t (BaseClass::*fnMethod)( Args... ) ) {
    return Delegate< Return_t ( Args... ) >( pcThis, fnMethod );
}

template< typename T, typename BaseClass, typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( const T* pcThis, Return_t (BaseClass::*fnConstMethod)( Args... ) const ) {
    return Delegate< Return_t ( Args... ) >( pcThis, fnConstMethod );
}

}

#endif //BS_DELEGATE_HPP