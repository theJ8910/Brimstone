/*
util/RefType.hpp
----------------
Copyright (c) 2024, theJ89

Description:
    Utility for template classes that allows them to automatically determine
    whether something should be passed by value or passed by reference.

    RefType<T> consists of two aliases:  RefType<T>::ref, and  RefType<T>::const_ref.
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
        using TRef      = typename RefType<T>::ref;
        using TConstRef = typename RefType<T>::const_ref;
    public:
        void SomeMethod( TConstRef param ) {
        };
    };
*/
#ifndef BS_UTIL_REFTYPE_HPP
#define BS_UTIL_REFTYPE_HPP




//Includes
#include <cstdint>  //std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t




//Macros
//Macro for defining a specialization for a primitive type
#define MAKE_PRIMITIVE_REFTYPE_SPEC( type ) \
    template<>                              \
    class RefType< type > {                 \
    public:                                 \
        using ref       = type;             \
        using const_ref = const type;       \
    };




namespace Brimstone {




template< typename T >
class RefType {
public:
    using ref       = T&;
    using const_ref = const T& const_ref;
};

//Specialization for pointer types
template< typename T >
class RefType< T* > {
public:
    using ref       = T*;
    using const_ref = T* const;
};

template< typename T >
class RefType< const T* > {
public:
    using ref       = const T*;
    using const_ref = const T* const;
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




} //namespace Brimstone




#endif //BS_UTIL_REFTYPE_HPP
