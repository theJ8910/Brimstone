/*
util/RefType.hpp
----------------
Copyright (c) 2024, theJ89

Description:
    Utility for template classes that allows them to automatically determine
    whether something should be passed by value or passed by reference.

    RefType<T> consists of two typedefs:  RefType<T>::ref, and  RefType<T>::const_ref.
    If T is an object:
        ref is:       T&
        const_ref is: const T&
    If T is a primitive type or pointer:
        ref is:       T
        const_ref is: const T

    Sample usage:
    #include <brimstone/util/RefType.hpp>

    template< typename T >
    class SomeClass {
    private:
        typedef typename RefType<T>::ref       TRef;
        typedef typename RefType<T>::const_ref TConstRef;
    public:
        void SomeMethod( TConstRef param ) {
        };
    };
*/
#ifndef BS_UTIL_REFTYPE_HPP
#define BS_UTIL_REFTYPE_HPP




//Includes
#include <cstdint>      //std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t




namespace Brimstone {

template< typename T >
class RefType {
public:
    typedef       T  ref;
    typedef const T& const_ref;
};

//Macro for defining a specialization for a primitive type
#define MAKE_PRIMITIVE_REFTYPE_SPEC( type ) \
    template<>\
    class RefType< type > {\
    public:\
        typedef       type ref;\
        typedef const type const_ref;\
    };

//Specializations for primitive types
MAKE_PRIMITIVE_REFTYPE_SPEC( bool          );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::int8_t   );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::uint8_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::int16_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::uint16_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::int32_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::uint32_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::int64_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( std::uint64_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( float         );
MAKE_PRIMITIVE_REFTYPE_SPEC( double        );

//This macro is no longer necessary after this point
#undef MAKE_PRIMITIVE_REFTYPE_SPEC

//Specialization for pointer types
template< typename T >
class RefType< T* > {
public:
    typedef T*       ref;
    typedef T* const const_ref;
};

template< typename T >
class RefType< const T* > {
public:
    typedef const T*       ref;
    typedef const T* const const_ref;
};

}




#endif //BS_UTIL_REFTYPE_HPP
