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
        template< class X, class XFn_t >
        inline static GenericClassPtr convert( X* pThis, XFn_t fn, GenericMethodPtr& fnOut ) {
            static_assert( N != DELEGATE_POINTER_SIZE, "Unsupported method pointer size" );
            return nullptr;
        }
    };

    //Single-inheritance
    template<>
    struct SimplifyMethod< DELEGATE_POINTER_SIZE > {
        template< class X, class XFn_t >
        inline static GenericClassPtr convert( X* pThis, XFn_t fn, GenericMethodPtr& fnOut ) {
            fnOut = reinterpret_cast< GenericMethodPtr >( fn );
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
        void                bindMethod( X* pThis, XFn_t fn );

        template< typename X, class XFn_t >
        void                bindConstMethod( const X* pThis, XFn_t fn );

        template< typename Delegate_t, typename Invoker_t, typename Fn_t >
        void                bindStaticMethod( Delegate_t* pDelegate, Invoker_t invoker, Fn_t fn );

        GenericClassPtr     getThis() const;
        Method_t            getMethod() const;
        StaticMethod_t      getStaticMethod() const;

        template< typename, typename >
        friend bool operator ==( const Closure< Method_t, StaticMethod_t >& left, const Closure< Method_t, StaticMethod_t >& right );

        template< typename, typename >
        friend bool operator !=( const Closure< Method_t, StaticMethod_t >& left, const Closure< Method_t, StaticMethod_t >& right );

    private:
        GenericClassPtr     m_this;
        GenericMethodPtr    m_method;
#ifndef BS_DELEGATE_POINTER_HACK
        GenericFunctionPtr  m_staticMethod;
#endif //BS_DELEGATE_POINTER_HACK
    };

    template< typename Method_t, typename StaticMethod_t >
    template< typename X, class XFn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindMethod( X* pThis, XFn_t fn ) {
        m_this = SimplifyMethod< sizeof( fn ) >::convert( pThis, fn, m_method );
#ifndef BS_DELEGATE_POINTER_HACK
        m_staticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    template< typename X, class XFn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindConstMethod( const X* pThis, XFn_t fn ) {
        m_this = SimplifyMethod< sizeof( fn ) >::convert( const_cast< X* >( pThis ), fn, m_method );
#ifndef BS_DELEGATE_POINTER_HACK
        m_staticMethod = nullptr;
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    template< typename Delegate_t, typename Invoker_t, typename Fn_t >
    inline void Closure< Method_t, StaticMethod_t >::bindStaticMethod( Delegate_t* pDelegate, Invoker_t invoker, Fn_t fn ) {
#ifdef BS_DELEGATE_POINTER_HACK
        static_assert( sizeof( StaticMethod_t ) == sizeof( this ), "Can't pointer hack" );
        SimplifyMethod< sizeof( invoker ) >::convert( pDelegate, invoker, m_method );
        m_this = universal_cast< GenericClassPtr >( fn );
#else //BS_DELEGATE_POINTER_HACK
        m_this          = SimplifyMethod< sizeof( invoker ) >::convert( pDelegate, invoker, m_method );
        m_staticMethod  = reinterpret_cast< GenericFunctionPtr >( fn );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    inline GenericClassPtr Closure< Method_t, StaticMethod_t >::getThis() const {
        return m_this;
    }

    template< typename Method_t, typename StaticMethod_t >
    inline Method_t Closure< Method_t, StaticMethod_t >::getMethod() const {
        return reinterpret_cast< Method_t >( m_method );
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
        static_assert( sizeof( StaticMethod_t ) == sizeof( this ), "Can't pointer hack" );
        return universal_cast< StaticMethod_t >( this );
#else  //BS_DELEGATE_POINTER_HACK
        return reinterpret_cast< StaticMethod_t >( m_staticMethod );
#endif //BS_DELEGATE_POINTER_HACK
    }

    template< typename Method_t, typename StaticMethod_t >
    bool operator ==( const Closure< Method_t, StaticMethod_t >& left, const Closure< Method_t, StaticMethod_t >& right ) {
        return ( left.m_this            == right.m_this         &&
                 left.m_method          == right.m_method
    #ifndef BS_DELEGATE_POINTER_HACK
                                                                &&
                 left.m_staticMethod    == right.m_staticMethod
    #endif
               );
    }

    template< typename Method_t, typename StaticMethod_t >
    bool operator !=( const Closure< Method_t, StaticMethod_t >& left, const Closure< Method_t, StaticMethod_t >& right ) {
        return ( left.m_this            != right.m_this         ||
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
template< typename Return_t, typename... Args >
class Delegate< Return_t ( Args... ) > {
public:
    Delegate();

    //Bind an object + a method of its class
    template< typename T, typename BaseClass_t >
    Delegate( T* ptr, Return_t (BaseClass_t::*method)( Args... ) );

    //Bind an object + a const method of its class
    template< typename T, typename BaseClass_t >
    Delegate( const T* ptr, Return_t (BaseClass_t::*constMethod)( Args... ) const );

    //Bind static method / normal function
    Delegate(Return_t(*staticFunction)(Args...));

    template< typename T, typename BaseClass_t >
    void bind( T* ptr, Return_t (BaseClass_t::*fn)( Args... ) );

    template< typename T, typename BaseClass_t >
    void bind( const T* ptr, Return_t (BaseClass_t::*fn)( Args... ) const );

    void bind( Return_t (*staticFunction)( Args... ) );

    Return_t operator()( Args... args ) const;

    template< typename Signature >
    friend bool operator ==( const Delegate< Signature >& left, const Delegate< Signature >& right );

    template< typename Signature >
    friend bool operator !=( const Delegate< Signature >& left, const Delegate< Signature >& right );
private:
    Return_t staticInvoker( Args... args );
private:
    typedef Return_t (Private::GenericClass::*Method_t)( Args... );
    typedef Return_t (*StaticMethod_t)( Args... );
    Private::Closure< Method_t, StaticMethod_t >    m_closure;
};

template< typename Return_t, typename... Args >
Delegate< Return_t ( Args... ) >::Delegate() {
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
Delegate< Return_t ( Args... ) >::Delegate( T* ptr, Return_t (BaseClass_t::*method)( Args... ) ) {
    bind( ptr, method );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
Delegate< Return_t ( Args... ) >::Delegate( const T* ptr, Return_t (BaseClass_t::*constMethod)( Args... ) const ) {
    bind( ptr, constMethod );
}

template< typename Return_t, typename... Args >
Delegate< Return_t ( Args... ) >::Delegate( Return_t (*staticFunction)( Args... ) ) {
    bind( staticFunction );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
inline void Delegate< Return_t ( Args... ) >::bind( T* ptr, Return_t (BaseClass_t::*method)( Args... ) ) {
    m_closure.bindMethod( ptr, method );
}

template< typename Return_t, typename... Args >
template< typename T, typename BaseClass_t >
inline void Delegate< Return_t ( Args... ) >::bind( const T* ptr, Return_t (BaseClass_t::*constMethod)( Args... ) const ) {
    m_closure.bindConstMethod( ptr, constMethod );
}

template< typename Return_t, typename... Args >
inline void Delegate< Return_t ( Args... ) >::bind( Return_t (*staticFunction)( Args... ) ) {
    m_closure.bindStaticMethod( this, &Delegate::staticInvoker, staticFunction );
}

template< typename Return_t, typename... Args >
Return_t Delegate< Return_t ( Args... ) >::operator()( Args... args ) const {
    return ( ( m_closure.getThis() )->*( m_closure.getMethod() ) )( args... );
}

template< typename Signature >
bool operator ==( const Delegate< Signature >& left, const Delegate< Signature >& right ) {
    return left.m_closure == right.m_closure;
}

template< typename Signature >
bool operator !=( const Delegate< Signature >& left, const Delegate< Signature >& right ) {
    return left.m_closure != right.m_closure;
}

template< typename Return_t, typename... Args >
Return_t Delegate< Return_t ( Args... ) >::staticInvoker( Args... args ) {
    return ( *( m_closure.getStaticMethod() ) )( args... );
}

template< typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( Return_t (*staticFunction)( Args... ) ) {
    return Delegate< Return_t ( Args... ) >( staticFunction );
}

template< typename T, typename BaseClass, typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( T* pThis, Return_t (BaseClass::*method)( Args... ) ) {
    return Delegate< Return_t ( Args... ) >( pThis, method );
}

template< typename T, typename BaseClass, typename Return_t, typename... Args >
inline Delegate< Return_t ( Args... ) > bind( const T* pThis, Return_t (BaseClass::*constMethod)( Args... ) const ) {
    return Delegate< Return_t ( Args... ) >( pThis, constMethod );
}

}

#endif //BS_SIGNALS_DELEGATE_HPP
