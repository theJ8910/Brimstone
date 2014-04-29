/*
util/Cast.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines two custom casts:
        * implicit_cast
        * universal_cast.

    Contains notes on standard C++ casts.
*/

#ifndef BS_UTIL_CAST_HPP
#define BS_UTIL_CAST_HPP




namespace Brimstone {
namespace Private {

template< typename From_t, typename To_t >
union UniversalUnion{
    From_t  from;
    To_t    to;
};

}

/*
Casts "from" from From_t to To_t if it is possible to do so implicitly.
This mostly just serves as a readability aid to indicate implicit conversions.
Usage:
    implicit_cast< To_t >( from )
*/
template< typename To_t, typename From_t >
inline To_t implicit_cast( From_t from ) {
    return from;
}

/*
Casts "from" from From_t to To_t in an unsafe manner.
Both From_t and To_t must be the same size, or invoking universal_cast will fail with a compile error.
Usage:
    universal_cast< To_t >( from );
*/
template< typename To_t, typename From_t >
inline To_t universal_cast( From_t from ) {
    Private::UniversalUnion< From_t, To_t > u;

    //The type we're casting from needs to be the same size as the type we're casting to.
    //Furthermore, the union of the two types must have the same size as well.
    //If they don't, this indicates the compiler uses an odd representation for unions,
    //which could interfere with the cast.
    typedef int Error_NoUniversalCast[
        sizeof( From_t ) == sizeof( u ) && sizeof( From_t ) == sizeof( To_t )
        ?  1 
        : -1
    ];

    u.from = from;
    return u.to;
}

/*
Here are some details on standard casts, along with a description of what they do:

dynamic_cast:
    Usage:
        dynamic_cast< DesiredType* >( &obj )
        dynamic_cast< DesiredType& >( obj )
        dynamic_cast< void* >( &obj )
    Restrictions:
        * Both DesiredType and obj type must be pointers or references, and these must be to objects, not primitives (bool, int, etc).
        * DesiredType must be related to the obj's type via inhertience (e.g. parent, child, etc ).
    Description:
        Part of the C++'s RTTI (Run Time Type Information) system.

        Lets say you have three classes, A, B, and C.
        C inherits from B, and B inherits from A:
            A
            |
            B
            |
            C

        This means that:
            All A objects count as A objects.
            All B objects count as A objects and B objects.
            All C objects count as A objects, B objects, and C objects.

        Now, lets say we have the following pointer:
            B* bObj = someFunction()

        Since bObj is a B*, we know that bObj is definitely both an A object and a B object. It could also possibly be a C object.
        We won't know what type of object it actually is until runtime, though.

        Lets say we wanted to cast the B* to an A*. We can do this like so:
            A* aObj = dynamic_cast< A* >( bObj );

        This is called "upcasting" (because we're casting upward in the inheritence hierarchy, i.e. to a parent class).
        Upcasting with dynamic_cast is the same as casting with static_cast; i.e. it does not carry a performance penalty.

        dynamic_cast can also "downcast" (cast downwards in the inheritence hierarchy, i.e. to a child class).
        This can only be done safely if bObj is a "C" object though - again, something we won't know until runtime.
        This means that at runtime, dynamic_cast needs to check what type of object bObj actually is.
        We can downcast from a B* to a C* like so:
            C* cObj = dynamic_cast< C* >( bObj );

        If bObj is NOT a "C" object, dynamic_cast will return nullptr.
        Otherwise, it returns a pointer to the "C" data of the object (which normally is no different than the address you gave it,
        but isn't always; e.g. because of multiple inheritence).

        Lets say you have three other classes, X, Y, and Z.
        Z inherits from both X and Y:
            X   Y
             \ /
              Z
        
        This means that all Z objects count as both X and Y objects.

        Lets say you have the following pointer:
        X* xObj;

        Since xObj is an X*, we know that xObj is definitely an X object.
        It could also possibly be a Z object, however.
        If xObj is a Z object, that would also mean that it is a Y object as well (since Z objects are both X and Y).

        With this in mind, it is possible to do the following:
            Y* yObj = dynamic_cast< Y* >( xObj );

        This is referred to as a "cross-cast". It's essentially the same as doing the following:
            Z* zObj = dynamic_cast< Z* >( xObj );   //Downcast
            Y* yObj = nullptr;
            if( zObj != nullptr )
                yObj = dynamic_cast< Y* >( zObj );  //Upcast

        Lastly, there's a special case dynamic_cast can be used.
        dynamic_cast<void*>( obj ) returns a void* containing the base address of the object.
        This is really only useful in the case of multiple inheritence, however.

        IMPORTANT: Downcasting and crosscasting with dynamic_cast carries a performance penalty.
        This penalty varies from compiler to compiler because of varying implementations.

static_cast:
    Usage:
        static_cast< DesiredType >( obj )
    Restrictions:
        * Both DesiredType and obj type must be pointers or references, and these must be to objects, not primitives (bool, int, etc).
        * DesiredType must be related to the obj's type via inheritence (e.g. parent, child, etc ).
    Description:
        Same as dynamic_cast, but does not perform runtime checks when downcasting / crosscasting.
        This makes it typesafe when upcasting, but not when downcasting or crosscasting.

reinterpret_cast:
    Usage:
        reinterpret_cast< DesiredType >( value )
    Restrictions:
        * Both DesiredType and value must be either pointers or integral types.
    Description:
        Allows you to perform the following conversions:
            * Any Pointer to Any Pointer
            * Any Pointer to Integer
            * Integer to Any Pointer

        Since this allows conversions between unrelated types, reinterpret_cast
        is inherently non-typesafe. It should be used with care.

        Most often this is used for reinterpreting some object as an array of bytes:
            Object* ptr = &obj;
            char* bytes = reinterpret_cast<char*>( obj );

        Or vice versa:
            char* bytes = byteArray;
            Object* ptr = reinterpret_cast<char*>( bytes );

const_cast:
    Usage:
        const_cast< DesiredType >( value )          //Where value is of type: const DesiredType
        const_cast< const DesiredType >( value )    //Where value is of type: DesiredType
    Restrictions:
        * value's type and DesiredType must be the same (ignoring whether they are const or not).
    Description:
        Turns a const value into a non-const value, or a non-const value into a const value.
        This can potentially be unsafe because it allows modification of values that a programmer
        may not have expected to be modified.

C-style cast:
    Usage:
        (Type) value;
    Restrictions:
        * Conversion must be defined
    Description:
        Explicitly converts a value from one type to another (e.g. int to float).
        As with reinterpret_cast, Pointers can be cast from any type to any other type, making C-style casts inherently non-typesafe.
        Additionally, C-style casts are harder to search for in source code (since there is no identifying keyword such as "reinterpret_cast").

        In general, avoid C-style casts and use the other types of casts when possible.
*/

}




#endif //BS_UTIL_CAST_HPP